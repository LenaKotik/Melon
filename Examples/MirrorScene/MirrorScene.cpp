#include <Melon.hpp>

using namespace Melon;

#include "Examples/movement3d.h"

void DrawPlayer(RenderTarget* t, RenderedObject3D* player, RenderedObject3D* eyes)
{
	player->Draw(t);
	eyes->T.Position = Vector3(0.25f, 0.25f, 0.555f);
	eyes->Draw(t);
	eyes->T.Position = Vector3(-0.25f, 0.25f, 0.555f);
	eyes->Draw(t);
}

int main() // TODO: Fix this!!
{
	Window* win = Windowing::Init(800, 800, "It's You!", true);
	if (!win) return -1;
	win->SetCursor(false);

	Mesh cube = Helpers::Meshes::Cube();
	Mesh quad = Helpers::Meshes::Quad();

	RenderedObject3D* player = Helpers::Objects3D::Shape(cube);
	if (!player) return -1;
	player->Graphics->SetColor(Color::FromBytes(240, 26, 51));
	//player->T.Position = Vector3(0.0f, 0.0f, -1.0f);
	player->T.Rotation = Rotator(Pi, Vector3::PY());

	RenderedObject3D* eyes = Helpers::Objects3D::ColoredShape(cube);
	if (!eyes) return -1;
	eyes->Graphics->SetColor(Color::FromBytes(50, 203, 250));
	eyes->T.Parent = &player->T;
	eyes->T.Scale = 0.15f;

	RenderedObject3D* mirror_frame = Helpers::Objects3D::ColoredShape(quad);
	if (!mirror_frame) return -1;
	mirror_frame->Graphics->SetColor(Color::FromBytes(140, 38, 7));
	//mirror_frame->Graphics->SetColor(Color::FromBytes(36, 36, 36));
	mirror_frame->T.Position = Vector3(0.0f, -0.5f, 2.0f);
	mirror_frame->T.Rotation = Rotator(Pi, Vector3::PY());
	mirror_frame->T.Scale = 3.0f;

	RenderedObject3D* mirror = Helpers::Objects3D::Shape(quad);
	if (!mirror) return -1;
	mirror->T.Parent = &mirror_frame->T;
	mirror->Graphics->SetColor(Color::FromBytes(195, 2, 224));
	mirror->T.Position.z = 0.001f; // z-fighting thing
	mirror->T.Scale = 0.9f;

	Shader* effect = Helpers::ShaderLib::LoadBasic("PostProcessing/Mirror");
	if (!effect) return -1;
	mirror->Shader_ = *effect;

	const FixedArray<String, 6> cubemap_names = { "px", "nx", "py", "ny", "pz", "nz" };
	FixedArray<TextureData, 6> cubemap_data;

	ResourceLoader::flipYTextures = false;
	for (int i = 0; i < 6; i++)
		if (!ResourceLoader::LoadTextureData(&cubemap_data[i], (SourceDir + "/Examples/MirrorScene/skybox/" + cubemap_names[i] + ".jpg").c_str()))
			std::cout << "Could not load texture data: " + cubemap_names[i];

	CubeMap cubemap(cubemap_data);

	Skybox* skybox = SkyboxFactory::Create(cubemap);
	
	FrameBuffer* mirror_fb = FrameBufferFactory::GetBasic(win->GetSize());
	if (!mirror_fb) return -1;

	mirror->Graphics->SetTexture(mirror_fb->GetColorAttachment(0));

	Camera3D cam;
	win->MainCamera = &cam;
	cam.FOV = 90.0f;
	cam.T.Position.z = -3.0f;

	player->T.Parent = &cam.T;
	//mirror_frame->T.Parent = &cam.T;
	
	Camera3D mirror_cam;
	mirror_fb->MainCamera = &mirror_cam;
	mirror_cam.T.Parent = &mirror->T;
	mirror_cam.FOV = 90.0f;

	std::cout << "FOV: " << cam.FOV << "; " << mirror_cam.FOV << std::endl;
	Time::GetDelta();
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();

		movement3D(win, &cam, delta);

		mirror_fb->Bind();
		mirror_fb->Clear(Color::FromBytes(89, 79, 79));

		skybox->Draw(mirror_fb);
		DrawPlayer(mirror_fb, player, eyes);

		win->Bind();
		win->Clear(Color::FromBytes(89, 79, 79)); // second pass

		skybox->Draw(win);
		//DrawPlayer(win, player, eyes);

		mirror_frame->Draw(win);
		mirror->Draw(win);

		win->Flip();

		Windowing::PollEvents();
	}
	player->Delete();
	eyes->Delete();
	mirror_frame->Delete();
	mirror->Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}