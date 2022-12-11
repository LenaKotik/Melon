#include "Melon.hpp"

Melon::Color Melon::Color::FromBytes(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	return Color((float)(r) / 255.0f, (float)(g) / 255.0f, (float)(b) / 255.0f, (float)(a) / 255.0f);
}

Melon::Color Melon::Color::operator+(Color oth)
{
	return Color(R+oth.R, G+oth.G, B+oth.B,min(A+oth.A, 1.0f));
}

Melon::Color Melon::Color::operator*(float scalar)
{
	return Color(R*scalar, G*scalar, B*scalar, A);
}

float Melon::Time::lastDeltaCall = 0.0f;
float Melon::Time::lastLimitedFrame = 0.0f;
float Melon::Time::MaxFrameRate = 0.0f;

Melon::DynamicArray<Melon::Timer*> Melon::Timer::all;

void Melon::Timer::update_all()
{
	for (Timer* timer : all)
	{
		if (!timer) return;
		timer->update();
	}
}

void Melon::Timer::update()
{
	if (!running || GetTimeLeft() > 0) return; // check for timeout
	Timeout.Invoke(new EventArgs()); // callback
	running = Loop; // if we loop, start again
	if (running) start_time += wait_time; // and set new interval
}

Melon::Timer::Timer()
{
	all.PushBack(this);
}

void Melon::Timer::Start(float time)
{
	start_time = Time::GetTime();
	wait_time = time;
	running = true;
}

float Melon::Timer::GetTime()
{
	return wait_time;
}

float Melon::Timer::GetTimeLeft()
{
	return wait_time - (Time::GetTime() - start_time);
}

void Melon::Timer::Wait()
{
	while (running) update_all();
}

void Melon::Timer::Stop()
{
	running = false;
}

bool Melon::Timer::isRunning()
{
	return running;
}

bool Melon::Time::FrameRateLimitSatisfied()
{
	float time = GetTime();
	if (time - lastLimitedFrame >= 1.0f / MaxFrameRate)
	{
		lastLimitedFrame = GetTime();
		return true;
	}
	return false;
}

float Melon::Time::GetTime()
{
	return glfwGetTime();
}

float Melon::Time::GetDelta()
{
	float delta = (GetTime() - lastDeltaCall);
	lastDeltaCall = GetTime();
	return delta;
}