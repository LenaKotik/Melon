#include "Melon.hpp"

void Melon::Event::Invoke(EventArgs* args)
{
	args->Sender = this;
	for (EventListener* subscriber : subcribers)
	{
		if (!subscriber) return; // if null, the rest is probably null too
		subscriber->Callback(args);
	}
}

void Melon::Event::operator+=(EventListener* subscriber)
{
	subcribers.PushBack(subscriber);
}
Melon::FixedFloatArray<2> Melon::KeyPressVector2Controller::Value()
{
	FixedFloatArray<2> vector;
	vector[0] = win->IsKeyPressed(KeyRight) - win->IsKeyPressed(KeyLeft);
	vector[1] = win->IsKeyPressed(KeyUp) - win->IsKeyPressed(KeyDown);
	return vector;
}
Melon::FixedFloatArray<3> Melon::KeyPressVector3Controller::Value()
{
	FixedFloatArray<3> vector;
	vector[0] = win->IsKeyPressed(KeyRight) - win->IsKeyPressed(KeyLeft);
	vector[1] = win->IsKeyPressed(KeyUp) - win->IsKeyPressed(KeyDown);
	vector[2] = win->IsKeyPressed(KeyForward) - win->IsKeyPressed(KeyBackward);
	return vector;
}
Melon::FixedFloatArray<2> Melon::MouseOffsetController::Value()
{
	Vector2 nowPos = win->GetMousePosition();
	if (first)
	{
		first = false;
		lastPos = nowPos;
		return { 0, 0 };
	}
	Vector2 offset = nowPos - lastPos;
	lastPos = nowPos;
	return { offset.x, -offset.y };
}
Melon::FixedFloatArray<3> Melon::PitchYaw2DirectionController::Value(FixedFloatArray<2> pitchYaw)
{
	Vector3 result;
	result.x = cos(pitchYaw[0]) * cos(pitchYaw[1]);
	result.y = sin(pitchYaw[0]);
	result.z = cos(pitchYaw[0]) * sin(pitchYaw[1]);
	result = result.Normalize();
	return { result.x, result.y, result.z };
}