void movement3D(Window* win, Camera3D* cam, float delta)
{
	static MouseOffsetController ctrl(win);
	KeyPressVector3Controller mov(win);
	PitchYaw2DirectionController dir;
	static float pitch = 0.0f, yaw = 90.0f;
	static bool cur = false;
	static bool firstPass = true;
	const float CamSpeed = 2.0f;
	const float sensitivity = 0.1f;

	if (win->IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		win->SetCursor(cur);
		cur = !cur;
	}

	Vector3 movement = Vector3(mov.Value()).Normalize() * delta * CamSpeed;
	
	cam->T.Position += cam->T.Direction * movement.z;
	cam->T.Position += cam->T.GetUpDirection() * movement.y;
	cam->T.Position += cam->T.GetRightDirection() * movement.x;

	Vector2 offset = Vector2(ctrl.Value()) * sensitivity;

	pitch += offset.y;
	yaw += offset.x;
	pitch = clamp(pitch, -89.0f, 89.0f);

	cam->T.Direction = dir.Value({ deg2rad(pitch), deg2rad(yaw) });
	//cam->T.Rotation = Rotator::FromDirection(dir.Value({ deg2rad(pitch), deg2rad(yaw) }));
	//cam->T.Rotation = Rotator::FromEulerVector(Vector3(pitch, -yaw, 0.0f));
	//std::cout << cam->Direction.x << " " << cam->Direction.y << " " << cam->Direction.z << "\n";
}