#include "Melon.hpp"

int main()
{
	Window* win = Windowing::Init(800, 800, "Will It Blend?", false);
	if (!win) return -1;
	if (!Windowing::InitFreetype()) return -1;

	Mesh quad = Helpers::Meshes::Quad();

	RenderedObject2D* object1 = Helpers::Objects2D::Shape(quad);
	if (!object1) return -1;
	object1->Graphics->SetColor(Color::FromBytes(0,0,255,200));
	object1->T.Position = Vector2(0.25f);

	RenderedObject2D* object2 = Helpers::Objects2D::Shape(quad);
	if (!object2) return -1;
	object2->Graphics->SetColor(Color::FromBytes(0,255,0,200));
	object2->T.Position = Vector2(-0.25f);

	Camera2D cam;
	win->MainCamera = &cam;

	Time::GetDelta();
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();

		object1->Graphics->SetColor(Color::FromBytes(0,0,255, 100*(sinf(Time::GetTime())+1)));

		win->Clear(Color::FromBytes(255,0,0),false);
		object1->Draw(win);
		object2->Draw(win);
		win->Flip();
		Windowing::PollEvents();
	}
	object1->Delete();
	object2->Delete();
	win->Delete();
	Windowing::Terminate();
	return 0;
}