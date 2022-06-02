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
	
	if (win->IsKeyPressed(GLFW_KEY_A))
		Camera::Position -= Camera::Direction.Cross(Camera::Up).Normalize() * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_D))
		Camera::Position += Camera::Direction.Cross(Camera::Up).Normalize() * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_S))
		Camera::Position -= Camera::Direction * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_W))
		Camera::Position += Camera::Direction * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || win->IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
		Camera::Position -= Camera::Up * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_SPACE))
		Camera::Position += Camera::Up * CamSpeed * delta;

	Vector2 offset = (win->GetMousePosition() - lastPos) * sensitivity;
	lastPos = win->GetMousePosition();


	PitchYaw.x += -offset.y;
	PitchYaw.y += offset.x;
	PitchYaw.x = clamp(PitchYaw.x, -89.0f, 89.0f);

	Camera::Direction.x = cos(deg2rad(PitchYaw.y)) * cos(deg2rad(PitchYaw.x));
	Camera::Direction.y = sin(deg2rad(PitchYaw.x));
	Camera::Direction.z = sin(deg2rad(PitchYaw.y)) * cos(deg2rad(PitchYaw.x));
	Camera::Direction = Camera::Direction.Normalize();
}
void movement2D(Window* win)
{
	float delta = Time::GetDelta();

	if (win->IsKeyPressed(GLFW_KEY_D))
		Camera::Position += Camera::Direction.Cross(Camera::Up).Normalize() * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_A))
		Camera::Position -= Camera::Direction.Cross(Camera::Up).Normalize() * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_W))
		Camera::Position += Camera::Up * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_S))
		Camera::Position -= Camera::Up * CamSpeed * delta;
}

int RandomTextureCubesScene()
{
	Window* win = Windowing::Init(800, 800, "Tsibulya", true);
	if (!win) return -1;

	Mesh m = Helpers::Meshes::Cube();

	win->SetCursor(false);

	Shader* shader = ResourceLoader::LoadShader("Projection.vert", "Texture.frag");
	if (!shader) return -1;
	Texture* texture = ResourceLoader::LoadTexture("grass.jpeg");
	if (!texture) return -1;

	Camera::Position = Vector3(0.0f, 0.0f, 3.0f);

	Renderer r(&m);

	DynamicVector3Array pos;

	for (int i = 0; i < 100; i++)
	{
		pos.push_back(-Vector3(rand() % 25 - 10, rand() % 25 - 10, rand() % 25 - 10));
	}

	while (!win->ShouldClose())
	{
		movement3D(win);

		Matrix4 persp = Matrix4::Perspective(deg2rad(45.0f), win->GetAspect(), 0.1f, 100.0f);
		Matrix4 view(1.0f);
		view = Camera::GetView({0, 1, 0});

		win->Clear(Color::FromBytes(255, 69, 69, 255), true);
		for (int i = 0; i < 100; i++)
		{
			Matrix4 trans(1.0f);
			trans = trans.Translate(pos[i]);
			shader->Use();
			shader->SetMatrix4(trans, "model");
			shader->SetMatrix4(view, "view");
			shader->SetMatrix4(persp, "projection");
			texture->Bind();
			r.Draw(GL_TRIANGLES);
		}
		win->Flip();
		Windowing::PollEvents();
	}
	delete shader;
	delete texture;
	r.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();

	return 0;
}
int Simple2DScene()
{
	Window* win = Windowing::Init(800, 800, "Tsibulya", false);
	if (!win) return -1;

	Mesh q = Helpers::Meshes::Circle(40);
	q.SetColor(Color::FromBytes(255, 39, 68, 255));

	Shader* shader = ResourceLoader::LoadShader("Projection.vert", "Color.frag");
	if (!shader) return -1;

	Renderer r(&q);

	while (!win->ShouldClose())
	{
		movement2D(win);

		win->Clear(Color::FromBytes(42, 255, 190, 255), false);

		Matrix4 trans(1.0f);

		shader->Use();
		shader->SetMatrix4(trans, "model");
		shader->SetMatrix4(Camera::GetView({0, 1, 0}), "view");
		shader->SetMatrix4(Matrix4(1.0f), "projection");
		r.Draw(GL_TRIANGLE_FAN);

		win->Flip();
		Windowing::PollEvents();
	}
	delete shader;
	r.Delete();
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
	m.SetColor(Color::FromBytes(243, 255, 39, 255));

	Shader* shader = ResourceLoader::LoadShader("Projection.vert", "Color.frag");
	if (!shader) return -1;

	Renderer r(&m);

	Matrix4 proj = Matrix4::Perspective(deg2rad(45.0f), win->GetAspect(), 0.1f, 100.0f);

	while (!win->ShouldClose())
	{
		Matrix4 model(1.0f);
		//model = model.Rotate(sin(Time::GetTime()), Vector3(0, 1, 0));
		movement3D(win);
		win->Clear(Color::FromBytes(0, 112, 111, 255), true);

		shader->Use();
		shader->SetMatrix4(model, "model");
		shader->SetMatrix4(Camera::GetView({0, 1, 0}), "view");
		shader->SetMatrix4(proj, "projection");
		r.Draw(GL_TRIANGLES);

		Windowing::PollEvents();
		win->Flip();
	}
	delete shader;
	r.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
}

int main()
{
	return LightingScene();
	//return Simple2DScene();
	//return RandomTextureCubesScene();
}