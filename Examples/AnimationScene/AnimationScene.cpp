#include "Melon.hpp"

using namespace Melon;

int main()
{
	Window* win = Windowing::Init(800, 800, "Animation", false);
	if (!win) return -1;

	Mesh m = Helpers::Meshes::Quad();

	RenderedObject2D* shape = Helpers::Objects2D::Shape(m);
	shape->Graphics->SetColor(Color::FromBytes(174, 60, 197, 255));
	
	Animation<Vector2> anim;
	InterpolationTrack<Vector2> scale_t, pos_t;
	scale_t.Add({Vector2(0.5,0.5), 0});
	scale_t.Add({Vector2(1,1), 0.5 });
	scale_t.Add({Vector2(0.5, 0.5), 1 });

	pos_t.Add({Vector2(-0.8,0), 0 });
	pos_t.Add({Vector2(0, 0.5), 0.5 });
	pos_t.Add({Vector2(0.8, 0), 1 });
	pos_t.Add({Vector2(-0.8,0), 1.5 });
	
	for (Keyframe<Vector2> p : pos_t.keyframes)
		std::cerr << p.value.x << " " << p.value.y << "  ";

	anim.Add(pos_t);
	anim.Add(scale_t);
	
	Camera2D cam;
	win->MainCamera = (Camera*)&cam;

	anim.loop = true;
	//anim.backward = true;
	anim.Play();
	
	while (!win->ShouldClose())
	{
		//std::cout << shape->T.Position.x << " " << shape->T.Position.y << "\n";
		win->Clear(Color::FromBytes(83, 197, 60, 255));
		shape->T.Position = anim[0];
		shape->T.Scale = anim[1];
		shape->Draw(win);
		win->Flip();
		Windowing::PollEvents();
	}
	shape->Delete();
	win->Delete();
	Windowing::Terminate();
	return 0;
}