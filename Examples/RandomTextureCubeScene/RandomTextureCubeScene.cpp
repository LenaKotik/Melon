#include "Melon.hpp"

using namespace Melon;
#include "Examples/movement3d.h"

int main()
{
	Window* win = Windowing::Init(800, 800, "Box World", true);
	if (!win) return -1;
	win->SetCursor(false);

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, (SourceDir + "/Examples/RandomTextureCubeScene/box.jpg").c_str())) return -1;
	Texture texture(td);

	Camera3D cam;
	win->MainCamera = &cam;

	RenderedObject3D tm = *Helpers::Objects3D::Shape(Helpers::Meshes::Cube());
	tm.Graphics->SetTexture(texture);
	DynamicVector3Array pos;
	DynamicArray<Rotator> rot;

	for (int i = 0; i < 500; i++)
	{
		pos.PushBack(-Vector3(rand() % 50 - 25, rand() % 50 - 25, rand() % 50 - 25));
		rot.PushBack(Rotator(rand()%7-3, Vector3(rand() % 25-10,rand()%25-10,rand()%25-10)));
	}
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		//printf("FPS:%d\n", (int)roundf(1 / delta));
		movement3D(win, delta);

		win->Clear(Color::FromBytes(255, 69, 69, 255), true);
		for (int i = 0; i < 500; i++)
		{
			tm.T.Position = pos[i];
			tm.T.Rotation = rot[i];
			tm.Draw(win);
		}
		win->Flip();
		Windowing::PollEvents();
	}


	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}
