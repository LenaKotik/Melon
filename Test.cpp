#include "Melon.hpp"

using namespace Melon;

const float CamSpeed = 2.0f;
const float sensitivity = 0.1f;
bool cur = false;
Vector2 PitchYaw(0.0f);

void movement3D(Window* win, float delta)
{
	static Vector2 lastPos;
	static bool firstMouse = true;

	if (firstMouse)
	{
		firstMouse = false;
		lastPos = win->GetMousePosition();
	}

	if (win->IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		win->SetCursor(cur);
		cur = !cur;
	}
	Camera3D* cam = (Camera3D*)win->MainCamera;

	if (win->IsKeyPressed(GLFW_KEY_A))
		cam->Position -= cam->Direction.Cross(cam->Up).Normalize() * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_D))
		cam->Position += cam->Direction.Cross(cam->Up).Normalize() * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_S))
		cam->Position -= cam->Direction * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_W))
		cam->Position += cam->Direction * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || win->IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
		cam->Position -= cam->Up * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_SPACE))
		cam->Position += cam->Up * CamSpeed * delta;

	Vector2 offset = (win->GetMousePosition() - lastPos) * sensitivity;
	lastPos = win->GetMousePosition();


	PitchYaw.x += -offset.y;
	PitchYaw.y += offset.x;
	PitchYaw.x = clamp(PitchYaw.x, -89.0f, 89.0f);

	cam->Direction.x = cos(deg2rad(PitchYaw.y)) * cos(deg2rad(PitchYaw.x));
	cam->Direction.y = sin(deg2rad(PitchYaw.x));
	cam->Direction.z = sin(deg2rad(PitchYaw.y)) * cos(deg2rad(PitchYaw.x));
	cam->Direction = cam->Direction.Normalize();
}
void movement2D(Window* win, float delta)
{
	Camera2D* cam = (Camera2D*)win->MainCamera;

	if (win->IsKeyPressed(GLFW_KEY_D))
		cam->Position += Vector2(1, 0) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_A))
		cam->Position -= Vector2(1, 0) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_W))
		cam->Position += Vector2(0, 1) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_S))
		cam->Position -= Vector2(0, 1) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_SPACE))
		cam->Zoom += CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || win->IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
		cam->Zoom -= CamSpeed * delta;
}

int RandomTextureCubesScene()
{
	Window* win = Windowing::Init(800, 800, "Cubes", true);
	if (!win) return -1;
	win->SetCursor(false);

	Texture* texture = ResourceLoader::LoadTexture("grass.jpeg");
	if (!texture) return -1;

	Camera3D cam;
	cam.Position = Vector3(0.0f, 0.0f, 3.0f);
	win->MainCamera = &cam;

	TexturedMesh tm(texture, Helpers::Meshes::Cube());

	DynamicVector3Array pos;

	for (int i = 0; i < 100; i++)
	{
		pos.push_back(-Vector3(rand() % 25 - 10, rand() % 25 - 10, rand() % 25 - 10));
	}

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		printf("FPS:%d\n", (int)roundf(1 / delta));
		movement3D(win, delta);

		win->Clear(Color::FromBytes(255, 69, 69, 255), true);
		for (int i = 0; i < 100; i++)
		{
			tm.Position = pos[i];
			tm.Draw(win);
		}
		win->Flip();
		Windowing::PollEvents();
	}
	Windowing::DestroyWindow(win);
	Windowing::Terminate();

	return 0;
}
int Simple2DScene()
{
	Window* win = Windowing::Init(800, 800, "2D Crazyness", false);
	if (!win) return -1;
	
	Shape2D shape(Helpers::Meshes::Quad());
	Shape2D shape2(Helpers::Meshes::Circle(40));
	Sprite2D sprite(ResourceLoader::LoadTexture("melon.png"));

	shape.Color_ = Color::FromBytes(251, 71, 71, 255);
	shape2.Color_ = Color::FromBytes(32, 217, 36, 255);
	sprite.Scale = Vector2(2, 1);

	Camera2D cam;
	win->MainCamera = &cam;

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		printf("FPS:%d\n", (int)roundf(1 / delta));
		movement2D(win, delta);

		win->Clear(Color::FromBytes(42, 255, 190, 255), false);

	    shape.Rotation = Time::GetTime();
		shape2.Position = Vector2(cosf(Time::GetTime()), sinf(Time::GetTime()));
		sprite.Position =-Vector2(cosf(Time::GetTime()), sinf(Time::GetTime()));

		shape.Draw(win);
		shape2.Draw(win);
		sprite.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}
int LightingScene()
{
	Window* win = Windowing::Init(800, 800, "Phong Lighting", true);
	if (!win) return -1;

	win->SetCursor(false);

	Mesh m = Helpers::Meshes::Cube();
	Shape3D shape(m);

	m.PrimitiveType = GL_POINTS;
	Shape3D normals(m);

	Shader* normalGeom = ResourceLoader::LoadShader("Normal.vert", "GColor.frag", "NormalDisplay.geom");
	if (!normalGeom) return -1;
	normals.Shader_ = *normalGeom;

	shape.Color_ = Color::FromBytes(163, 9, 193, 255);
	shape.Shader_ = *ResourceLoader::LoadShader("LightingProjection.vert", "Phong.frag");

	m.PrimitiveType = GL_TRIANGLES;
	Shape3D light(m);
	light.Color_ = Color::FromBytes(255, 0, 251, 255);
	light.Position = Vector3(0, 1, -2);

	Camera3D cam;
	cam.Position = Vector3(0, 0.5, 1);

	win->MainCamera = &cam;

	normals.Shader_.Use();
	normals.Shader_.SetColor(Color::FromBytes(249, 255, 28, 255), "LineColor");
	normals.Shader_.SetFloat(0.2f, "length_");

	shape.Shader_.Use();
	shape.Shader_.SetInt(3, "Brightness");
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		printf("FPS:%d\n", (int)roundf(1 / delta));
		movement3D(win, delta);

		win->Clear(Color::FromBytes(29, 29, 29, 255), true);

		shape.Shader_.Use();
		shape.Shader_.SetColor(light.Color_, "LightColor");
		shape.Shader_.SetVector3(light.Position, "LightPosition");
		shape.Shader_.SetVector3(cam.Position, "CameraPosition");
		shape.Draw(win);
		normals.Draw(win);
		light.Draw(win);

		Windowing::PollEvents();
		win->Flip();
	}
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
}
int GeometryShaderScene()
{
	Window* win = Windowing::Init(800, 800, "Geometry Shader", true);
	if (!win) return -1;

	Mesh m = Helpers::Meshes::Cube();
	m.SetColor(Color::FromBytes(251, 71, 71, 255));
	Shape3D shape(m);
	m.PrimitiveType = GL_POINTS;

	Shape3D points(m);

	Shader* geom = ResourceLoader::LoadShader("ShapeProjection.vert", "GColor.frag", "Point2Square.geom");
	if (!geom) return -1;
	points.Shader_ = *geom;

	Camera3D cam;

	win->MainCamera = &cam;

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		printf("FPS:%d\n", (int)roundf(1 / delta));
		movement3D(win, delta);

		win->Clear(Color::FromBytes(29, 29, 29, 255), true);

		shape.Draw(win);
		points.Shader_.Use();
		points.Shader_.SetFloat(0.2, "radius");
		points.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	points.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}
int KinematicBody2DScene()
{
	Mesh circleMesh = Helpers::Meshes::Circle(40);
	const int N = 2;
	const float containerRadius = 4.0f;
	const float circleRadius = 0.2;
	const float scaleFactor = 0.2;
	const int steps = 1;

	Window* win = Windowing::Init(800, 800, "Kinematic Body 2D", false);
	if (!win) return -1;

	win->MainCamera = new Camera2D();

	Shape2D bodyRenderer(circleMesh);

	DynamicArray<KinematicBody2D> bodies(N);
	CollisionSolver2D solver;

	for (int i = 0; i < N; i++)
	{
		bodies[i].Collider_ = new CircleCollider2D(circleRadius);
		bodies[i].Position = Vector2((rand() % 5 - 3, rand() % 5 - 3));
	}
	Time::MaxFrameRate = 120.0f;
	Time::GetDelta();
	while (!win->ShouldClose())
	{
		if (!Time::FrameRateLimitSatisfied()) continue;
		float delta = Time::GetDelta();
		//delta /= 4.0f;
		float stepDelta = delta / (float)steps;
		//printf("FPS:%d\n", (int)roundf(1 / delta));
		movement2D(win, delta);

		for (int s = 0; s < steps; s++)
		for (int i = 0; i < N; i++)
		{
			bodies[i].Acceleration += Vector2(0, -0.01f);
			bodies[i].Update(stepDelta);
			CircleCollider2D A = *static_cast<CircleCollider2D*>(bodies[i].Collider_);
			if (bodies[i].Position.MagnitudeSqr() > powf(containerRadius - A.Radius, 2))
			{
				float dist = bodies[i].Position.Magnitude();
				Vector2 norm = -(bodies[i].Position * (1 / dist));
				bodies[i].Position += norm * (dist - (containerRadius - A.Radius));
			}
			for (int j = 0; j < N; j++) if (i != j)
				solver.Solve(A, *static_cast<CircleCollider2D*>(bodies[j].Collider_), &bodies[i].Position, &bodies[j].Position);
		}
		win->Clear(Color::FromBytes(31, 31, 31, 255), false);

		bodyRenderer.Scale = containerRadius * 2 * scaleFactor;
		bodyRenderer.Color_ = Color::FromBytes(55, 55, 55, 255);
		bodyRenderer.Position = 0;
		bodyRenderer.Draw(win);
	    bodyRenderer.Color_ = Color::FromBytes(77, 102, 255, 255);

		for (int i = 0; i < N; i++)
		{
			float rad = static_cast<CircleCollider2D*>(bodies[i].Collider_)->Radius;

			bodyRenderer.Scale = rad * 2 * scaleFactor;
			bodyRenderer.Position = bodies[i].Position * scaleFactor;
			bodyRenderer.Draw(win);
		}

		win->Flip();
		Windowing::PollEvents();
	}
	bodyRenderer.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}
int ModelImportScene()
{
	return 0;
}

int main()
{
    return RandomTextureCubesScene();
	return LightingScene();
	return GeometryShaderScene();
	return Simple2DScene();
	return ModelImportScene();
	return KinematicBody2DScene();
}