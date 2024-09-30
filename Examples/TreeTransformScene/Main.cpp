#include "Melon.hpp"

int main()
{
	Window* win = Windowing::Init(800, 800, "Cube", true);
	if (!win) return -1;
	win->SetCursor(0);

	Mesh m = Helpers::Meshes::Cube();

	RenderedObject3D* cube = Helpers::Objects3D::Shape(m);
	if (!cube) return -1;
	cube->Graphics->SetColor(Color::FromBytes(200,60,60,255));

	RenderedObject3D* bar = Helpers::Objects3D::Shape(m);
	if (!bar) return -1;
	bar->Graphics->SetColor(Color::FromBytes(70, 200, 70, 255));
	
	ChildTransform3D bar_t;
	bar_t.Parent = &cube->T;
	bar->Transform = (ShaderTransform3D*)&bar_t;

	bar->T.Position = Vector3(0, 2, 0);
	bar->T.Scale = Vector3(0.5, 3, 0.5);

	Camera3D cam;
	win->MainCamera = (Camera*)&cam;

	Time::GetDelta();
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		movement3D(win, delta);

		cube->T.Rotation = Rotator(2*(Time::GetTime()),Vector3(1,0,0));
		bar->T.Position = Vector3(0, sin(2 * Time::GetTime()), 0);

		win->Clear(Color::FromBytes(20, 20, 50, 255), true);
		cube->Draw(win);
		bar->Draw(win);
		win->Flip();
		Windowing::PollEvents();
	}
	cube->Delete();
	bar->Delete();
	win->Delete();
	Windowing::Terminate();
	return 0;
}
