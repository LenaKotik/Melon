#include "Melon.hpp"

using namespace Melon;

int main()
{
	Window* win = Windowing::Init(800, 800, "Text", false);
	if (!win) return -1;
	if (!Windowing::InitFreetype()) return -1;

	RenderedObject2D* sprite = Helpers::Objects2D::Sprite();
	if (!sprite) return -1;

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, (SourceDir+"/Examples/TextScene/melon.png").c_str())) return -1;

	sprite->Graphics->SetTexture(Texture(td));
	//sprite->T.Position = Vector2(0.5f,-0.5f);

	Font font;
	if (!ResourceLoader::LoadFont(&font, (SourceDir+"/Examples/TextScene/arialmt.ttf").c_str())) return -1;
	font.PreloadGlyphs(Font::ASCII);

	sprite->Shader_ = *Helpers::ShaderLib::LoadBasic("Text");
	sprite->Shader_.Use();
	sprite->Shader_.SetTexture(font.GetGlyph('a').texture, "glyph");
	sprite->Shader_.SetColor(Color::FromBytes(0, 0, 0),"TextColor");
	sprite->T.Rotation = Pi; // cringe

	RenderedText* text = Helpers::Text::Default(&font);
	text->Color_ = Color::FromBytes(0, 0, 0);
	text->Text = "Hello, world!";
	text->T.Position = Vector2(400, 400);

	Camera2D cam;
	win->MainCamera = &cam;
	cam.T.Scale = 5.0f;

	const float camSpeed = 2.0f;
	KeyPressVector2Controller movement(win);
	Time::GetDelta();
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		cam.T.Position +=  Vector2(movement.Value()) * delta * camSpeed;

		win->Clear(Color::FromBytes(255, 255, 255), false);
		sprite->Draw(win);
		text->Draw(win);
		win->Flip();
		Windowing::PollEvents();
	}
	sprite->Delete();
	text->Delete();
	font.Delete();
	win->Delete();
	Windowing::Terminate();
	return 0;
}
