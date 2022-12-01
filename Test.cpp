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

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, "grass.jpeg")) return -1;
	Texture texture(td);

	Camera3D cam;
	cam.Position = Vector3(0.0f, 0.0f, 3.0f);
	win->MainCamera = &cam;

	RenderedObject3D tm = *Helpers::Objects3D::TexturedShape(Helpers::Meshes::Cube());
	tm.Material_.Albedo = texture;

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
	
	RenderedObject2D shape = *Helpers::Objects2D::Shape(Helpers::Meshes::Circle(40));
	RenderedObject2D shape2 = *Helpers::Objects2D::Shape(Helpers::Meshes::Quad());
	RenderedObject2D sprite = *Helpers::Objects2D::Sprite();

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, "melon.png")) return -1;

	shape.Material_.Albedo = Color::FromBytes(251, 71, 71, 255);
	shape2.Material_.Albedo = Color::FromBytes(32, 217, 36, 255);
	sprite.Material_.Albedo = Texture(td);

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
	RenderedObject3D shape = *Helpers::Objects3D::Shape(m);

	m.PrimitiveType = GL_POINTS;
	RenderedObject3D normals = *Helpers::Objects3D::Shape(m);

	Shader* normalGeom = Helpers::ShaderLib::LoadGeom("NormalDisplay");
	if (!normalGeom) return -1;
	normals.Shader_ = *normalGeom;

	shape.Material_.Albedo = Color::FromBytes(163, 9, 193, 255);
	shape.Shader_ = *Helpers::ShaderLib::LoadBasic(Helpers::ShaderLoadOptions((Renderer::VertexAttributesConfig)(Renderer::Position3D|Renderer::Normal), false, true));

	m.PrimitiveType = GL_TRIANGLES;
	RenderedObject3D light = *Helpers::Objects3D::Shape(m);
	light.Material_.Albedo = Color::FromBytes(255, 0, 251, 255);
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
		//printf("FPS:%d\n", (int)roundf(1 / delta));
		movement3D(win, delta);

		win->Clear(Color::FromBytes(29, 29, 29, 255), true);

		shape.Shader_.Use();
		shape.Shader_.SetColor(light.Material_.Albedo.Solid, "LightColor");
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

	win->SetCursor(false);

	Mesh m = Helpers::Meshes::Cube();
	//m.SetColor(Color());
	RenderedObject3D shape = *Helpers::Objects3D::Shape(m);
	shape.Material_.Albedo = Color::FromBytes(251, 71, 71, 255);
	m.PrimitiveType = GL_POINTS;

	RenderedObject3D points = *Helpers::Objects3D::Shape(m);

	Shader* geom = Helpers::ShaderLib::LoadGeom("Point2Square");
	if (!geom) return -1;
	points.Shader_ = *geom;
	points.Material_.Albedo = Color::FromBytes(251, 71, 71, 255);

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

	RenderedObject2D bodyRenderer = *Helpers::Objects2D::Shape(circleMesh);

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
		bodyRenderer.Material_.Albedo = Color::FromBytes(55, 55, 55, 255);
		bodyRenderer.Position = 0;
		bodyRenderer.Draw(win);
	    bodyRenderer.Material_.Albedo = Color::FromBytes(77, 102, 255, 255);

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
int SoundScene()
{
	Window * win = Windowing::Init(800, 800, "Sounds", false);
	if (!win) return -1;

	AudioDevice* audio = Windowing::OpenAudioDevice();
	if (!audio) return -1;

	RenderedObject2D circle = *Helpers::Objects2D::Shape(Helpers::Meshes::Circle(40));
	circle.Material_.Albedo = Color::FromBytes(243, 28, 28, 255);

	SoundBuffer buffer;
	ResourceLoader::LoadAudio(&buffer, "me.wav");
	
	Camera2D cam;
	win->MainCamera = &cam;

	while (!win->ShouldClose())
	{
		//float delta = Time::GetDelta();

		win->Clear(Color::FromBytes(255, 255, 255, 255), false);

		circle.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	circle.Delete();
	Windowing::Terminate();
	return 0;
}

int main()
{
	return SoundScene();
	return Simple2DScene();
	return LightingScene();
	return GeometryShaderScene();
    return RandomTextureCubesScene();
	return KinematicBody2DScene();
	return ModelImportScene();
}