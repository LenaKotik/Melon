#include "Melon.hpp"

using namespace Melon;

void movement3D(Window* win, float delta)
{
	static MouseOffsetController ctrl(win);
	KeyPressVector3Controller mov(win);
	PitchYaw2DirectionController dir;
	static float pitch, yaw;
	static bool cur = false;
	static bool firstPass = true;
	const float CamSpeed = 2.0f;
	const float sensitivity = 0.1f;

	if (win->IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		win->SetCursor(cur);
		cur = !cur;
	}
	Camera3D* cam = (Camera3D*)win->MainCamera;

	Vector3 movement = Vector3(mov.Value()) * delta * CamSpeed;
	
	cam->Position += cam->Direction * movement.z;
	cam->Position += cam->Up * movement.y;
	cam->Position += cam->Right * movement.x;

	Vector2 offset = Vector2(ctrl.Value()) * sensitivity;

	pitch += offset.y;
	yaw += offset.x;
	pitch = clamp(pitch, -89.0f, 89.0f);

	cam->SetDirection(dir.Value({ deg2rad(pitch), deg2rad(yaw) }));
	//std::cout << cam->Direction.x << " " << cam->Direction.y << " " << cam->Direction.z << "\n";
}