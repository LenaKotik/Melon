#include "Melon.hpp"

int main()
{
	Window* win = Windowing::Init(800, 800, "Geometry Shader", true);
	if (!win) return -1;

	win->SetCursor(false);

	Mesh m = Helpers::Meshes::Cube();
	RenderedObject3D shape = *Helpers::Objects3D::Shape(m);
	shape.Graphics->SetColor(Color::FromBytes(251, 71, 71, 255),0);
	m.PrimitiveType = GL_POINTS;

	RenderedObject3D points = *Helpers::Objects3D::Shape(m);

	Shader* geom = Helpers::ShaderLib::LoadGeom("Point2Square");
	if (!geom) return -1;
	points.Shader_ = *geom;
	points.Graphics->SetColor(Color::FromBytes(251, 71, 71, 255),0);

	Camera3D cam;

	win->MainCamera = &cam;

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		printf("FPS:%d\n", (int)roundf(1 / delta));
		movement3D(win, delta);

		win->Clear(Color::FromBytes(29, 29, 29, 255), true);

		shape.Draw(win);
		points.Shader_.Use();
		points.Shader_.SetFloat(0.2, "scale");
		points.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	points.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}

