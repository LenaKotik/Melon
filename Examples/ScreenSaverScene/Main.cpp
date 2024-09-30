#include "Melon.hpp"

int main()
{
	Window* win = Windowing::Init(800, 800, "Satisfactory", false);
	if (!win) return -1;

	RenderedObject2D* shape = Helpers::Objects2D::Shape(Helpers::Meshes::Quad());
	if (!shape) return -1;
	shape->Graphics->SetColor(Color::FromBytes(250, 10, 10));
	shape->T.Scale = Vector2(0.3f);

	Camera2D cam;
	win->MainCamera = &cam;

	srand(time(0));

	float angle = deg2rad(float(rand() % 360));
	Vector2 velocity(cosf(angle),sinf(angle));
	
	glfwMaximizeWindow(win->handle);

	win->Clear(Color::FromBytes(255, 255, 255), false);
	win->Flip();
	win->Clear(Color::FromBytes(255, 255, 255), false);
	
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		//printf("FPS: %d \n", (int)roundf(1.0f / delta));

		shape->T.Position += velocity*delta;
		
		if (abs(shape->T.Position.x) >= (2.1f - 0.3f))
		{
			velocity.x = copysignf(velocity.x,-shape->T.Position.x);
			
			shape->Graphics->SetColor(Color::FromBytes(rand() % 256, rand() % 256, rand() % 256));
			velocity *= 1.01f;
		}
		if (abs(shape->T.Position.y) >= (1.2f - 0.3f))
		{
			velocity.y = copysignf(velocity.y, -shape->T.Position.y);
			
			shape->Graphics->SetColor(Color::FromBytes(rand() % 256, rand() % 256, rand() % 256));
			velocity *= 1.01f;
		}

		//win->Clear(Color::FromBytes(255, 255, 255), false);
		shape->Draw(win);
		win->Flip();

		Windowing::PollEvents();
	}
	shape->Delete();
	win->Delete();
	Windowing::Terminate();
	return 0;
}
