#include "Melon.hpp"

using namespace Melon;

const float CamSpeed = 2.0f;
const float sensitivity = 0.1f;
bool cur = false;
Vector2 PitchYaw(0.0f);

void movement3D(Window* win)
{
	static Vector2 lastPos;
	static bool firstMouse = true;

	if (firstMouse)
	{
		firstMouse = false;
		lastPos = win->GetMousePosition();
	}

	float delta = Time::GetDelta();

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
void movement2D(Window* win)
{
	float delta = Time::GetDelta();

	Camera2D* cam = (Camera2D*)win->MainCamera;

	if (win->IsKeyPressed(GLFW_KEY_D))
		cam->Position += Vector2(1, 0) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_A))
		cam->Position -= Vector2(1, 0) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_W))
		cam->Position += Vector2(0, 1) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_S))
		cam->Position -= Vector2(0, 1) * CamSpeed * delta;
}

int RandomTextureCubesScene()
{
	Window* win = Windowing::Init(800, 800, "Tsibulya", true);
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
		movement3D(win);

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
	Window* win = Windowing::Init(800, 800, "Tsibulya", false);
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
		movement2D(win);

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
	Window* win = Windowing::Init(800, 800, "Tsibulya", true);
	if (!win) return -1;

	win->SetCursor(false);

	Mesh m = Helpers::Meshes::Cube();
	Shape3D shape(m);
	shape.Color_ = Color::FromBytes(163, 9, 193, 255);
	shape.Shader_ = *ResourceLoader::LoadShader("Projection.vert", "Phong.frag");

	Shape3D light(m);
	light.Color_ = Color::FromBytes(79, 255, 251, 255);
	light.Position = Vector3(-1, -2, -2);

	Camera3D cam;
	//cam.Position = Vector3(5, 3, 3);

	win->MainCamera = &cam;

	while (!win->ShouldClose())
	{
		movement3D(win);
		win->Clear(Color::FromBytes(29, 29, 29, 255), true);

		shape.Shader_.Use();
		shape.Shader_.SetColor(light.Color_, "LightColor");
		shape.Draw(win);

		light.Draw(win);

		Windowing::PollEvents();
		win->Flip();
	}
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
}

int main()
{
	return LightingScene();
	return Simple2DScene();
    return RandomTextureCubesScene();
}