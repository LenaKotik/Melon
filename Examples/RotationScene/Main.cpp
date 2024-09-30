#include "Melon.hpp"

int main()
{
	Window* win = Windowing::Init(800, 800, "Roteto", true);
	if (!win) return -1;
	win->SetCursor(0);

	RenderedObject3D* box = Helpers::Objects3D::TexturedShape(Helpers::Meshes::Cube());
	if (!box) return -1;
	
	Shader* box_sh = Helpers::ShaderLib::LoadBasic("Border");
	if (!box_sh) return -1;

	BorderGraphics box_g;
	box_g.SetColor(Color::FromBytes(21, 67, 96),1);
	box_g.SetWidth(0.1f);

	box->Shader_ = *box_sh;
	box->Graphics = (ShaderGraphics*)&box_g;

	box->Graphics->SetColor(Color::FromBytes(250, 47, 47),0);

	Camera3D cam;
	win->MainCamera = (Camera*)&cam;
	cam.Position = Vector3(-3, 0, 0);

	Animation<Rotator> anim_rot;
	InterpolationTrack<Rotator> track_rot;

	track_rot.Add({Rotator(0,Vector3(0.2,0.5,0.3)),0.0f });
	track_rot.Add({Rotator(2*Pi,Vector3(0.2,0.5,0.3)),2.0f });

	anim_rot.Add(track_rot);
	anim_rot.loop = true;

	Animation<Color> anim_col;
	InterpolationTrack<Color> track_col;

	track_col.Add({Color::FromBytes(250, 47, 47),0.0f });
	track_col.Add({Color::FromBytes(47, 250, 47),0.5f });
	track_col.Add({Color::FromBytes(47, 47, 250),1.0f });
	track_col.Add({Color::FromBytes(250, 47, 47),1.5f });

	anim_col.Add(track_col);
	anim_col.loop = true;

	anim_rot.Play();
	anim_col.Play();

	Time::GetDelta();
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		printf("FPS: %d\n", (int)roundf(1.0f / delta));
		movement3D(win, delta);

		box->T.Rotation = anim_rot[0];
		box->Graphics->SetColor(anim_col[0]);

		win->Clear(Color::FromBytes(255,255,255),true);
		box->Draw(win);
		win->Flip();
		Windowing::PollEvents();
	}
	box->Delete();
	win->Delete();
	Windowing::Terminate();
	return 0;
}
