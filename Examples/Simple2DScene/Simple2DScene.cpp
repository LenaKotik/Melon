#include "Melon.hpp"

using namespace Melon;

int main()
{
	Window* win = Windowing::Init(800, 800, "2D Crazyness", false);
	if (!win) return -1;
	
	RenderedObject2D shape = *Helpers::Objects2D::Shape(Helpers::Meshes::Quad());
	RenderedObject2D shape2 = *Helpers::Objects2D::Shape(Helpers::Meshes::Circle(40));
	RenderedObject2D sprite = *Helpers::Objects2D::Sprite();

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, (SourceDir + "/Examples/Simple2DScene/melon.png").c_str())) return -1;

	shape.Graphics->SetColor(Color::FromBytes(251, 71, 71, 255));
	shape2.Graphics->SetColor(Color::FromBytes(32, 217, 36, 255));
	sprite.Graphics->SetTexture(Texture(td));

	sprite.T.Scale = Vector2(2, 1);

	Camera2D cam;
	win->MainCamera = &cam;
	cam.Scale = 2;

	const float camSpeed = 2.0f;
	KeyPressVector2Controller movement(win);

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		printf("FPS:%d\n", (int)roundf(1 / delta));
		
		cam.Position += Vector2(movement.Value()) * delta * camSpeed;

		win->Clear(Color::FromBytes(42, 255, 190, 255), false);

	    shape.T.Rotation = Time::GetTime();
		shape2.T.Position = Vector2(cosf(Time::GetTime()), sinf(Time::GetTime()));
		sprite.T.Position =-Vector2(cosf(Time::GetTime()), sinf(Time::GetTime()));

		shape.Draw(win);
		shape2.Draw(win);
		sprite.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}
