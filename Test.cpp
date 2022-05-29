#include "Melon.hpp"

using namespace Melon;

const float CamSpeed = 5.0f;
const float sensitivity = 0.1f;
bool cur = false;
Vector2 PitchYaw(0.0f);

void input(Window* win)
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
		Camera::Position -= glm::normalize(glm::cross(Camera::Direction, Camera::Up)) * CamSpeed * delta;
	if (win->IsKeyPressed(GLFW_KEY_D))
		Camera::Position += glm::normalize(glm::cross(Camera::Direction, Camera::Up)) * CamSpeed * delta;
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

	PitchYaw.x = glm::clamp(PitchYaw.x, -89.0f, 89.0f);

	Camera::Direction.x = cos(deg2rad(PitchYaw.y)) * cos(deg2rad(PitchYaw.x));
	Camera::Direction.y = sin(deg2rad(PitchYaw.x));
	Camera::Direction.z = sin(deg2rad(PitchYaw.y)) * cos(deg2rad(PitchYaw.x));
	Camera::Direction = glm::normalize(Camera::Direction);
}

int main()
{
	Mesh m = Helpers::Meshes::Cube();

	Window* win = Windowing::Init(800, 800, "Tsibulya", true);
	if (!win) return -1;

	win->SetCursor(false);
	
	Shader* shader = ResourceLoader::LoadShader("Shader.vert", "Shader.frag");
	if (!shader) return -1;
	Texture* texture = ResourceLoader::LoadTexture("grass.jpeg");
	if (!texture) return -1;

	Camera::Position = Vector3(0.0f, 0.0f, 3.0f);
	

	Renderer r(&m);

	DynamicVector3Array pos;

	for (int i = 0; i < 100; i++)
	{
		pos.push_back(-Vector3(rand() % 25-10, rand() % 25-10, rand() % 25-10));
	}

	while (!win->ShouldClose())
	{
		input(win);

		Matrix4 persp = glm::perspective(deg2rad(45.0f), win->GetAspect(), 0.1f, 100.0f);

		Matrix4 view(1.0f);
		view = glm::lookAt(Camera::Position, Camera::Position + Camera::Direction, Camera::Up);

		win->Clear(Color::FromBytes(255, 69, 69, 255), true);
		for (int i = 0; i < 100; i++)
		{
			Matrix4 trans(1.0f);
			trans = glm::translate(trans, pos[i]);
			shader->Use();
			shader->SetMatrix4(trans, "model");
			shader->SetMatrix4(view, "view");
			shader->SetMatrix4(persp, "perspective");
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