#include <Melon.hpp>

using namespace Melon;

#include "Examples/movement3d.h"

int main()
{
	Window* win = Windowing::Init(800, 800, "I Smoke Weed", true);
	if (!win) return -1;
	//win->Maximize();

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, (SourceDir+"/Examples/PostProcessingScene/container.jpg").c_str()))
		return -1;

	Mesh m1 = Helpers::Meshes::Cube();
	Mesh m2 = Helpers::Meshes::Quad();
	
	RenderedObject3D* box = Helpers::Objects3D::TexturedShape(m1);
	if (!box) return -1;
	Texture box_t(td);
	box->Graphics->SetTexture(&box_t);

	RenderedObject3D* plane = Helpers::Objects3D::ColoredShape(m2);
	if (!plane) return -1;
	plane->Graphics->SetColor(Color::FromBytes(3, 171, 140));

	RenderedObject2D* screen_quad = Helpers::Objects2D::Sprite();
	if (!screen_quad) return -1;

	//Shader* postProcEffect = Helpers::ShaderLib::LoadBasic("PostProcessing/Empty");
	//Shader* postProcEffect = Helpers::ShaderLib::LoadBasic("PostProcessing/Negative");
	//Shader* postProcEffect = Helpers::ShaderLib::LoadBasic("PostProcessing/Grayscale");
	//Shader* postProcEffect = Helpers::ShaderLib::LoadBasic("PostProcessing/FlyVision");
	Shader* postProcEffect = Helpers::ShaderLib::LoadBasic("PostProcessing/Kernel");
	if (!postProcEffect) return -1;
	screen_quad->Shader_ = *postProcEffect;

	FrameBuffer* fb = FrameBufferFactory::GetBasic(win->GetSize());
	Camera3D fb_cam;
	Camera2D main_cam;
	fb->MainCamera = &fb_cam;
	win->MainCamera = &main_cam;

	float kernel1[9] = {
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	};
	float kernel2[9] = {
		1.0 / 16.0,	2.0 / 16.0,	1.0 / 16.0,
		2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
	};
	float kernel3[9] = {
		1,  1, 1,
		1, -8, 1,
		1,  1, 1
	};
	
	std::cout << "Aspect: " << fb->GetAspect() << "\n";

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		movement3D(win, &fb_cam, delta);

		fb->Bind(); // first pass
		Windowing::SetDepth(true);
		fb->Clear(Color::FromBytes(7, 68, 94));

		plane->T.Position = Vector3(0.0f, -1.5f -0.001f, -3.0f); // -0.001f To prevent Z-fighting
		plane->T.Rotation = Rotator(Pi / 2.0f, Vector3::PX());
		plane->T.Scale = 10.0f;
		plane->Draw(fb);
	
		box->T.Position = Vector3(0.5f, -1.0f, -2.0f);
		box->Draw(fb);
		
		box->T.Position = Vector3(-1.0f, -1.0f, -3.0f);
		box->Draw(fb);

		box->T.Position = Vector3(0.0f, -1.0f, -4.5f);
		box->Draw(fb);

		Texture* first_pass = fb->GetColorAttachment(0);
		screen_quad->Graphics->SetTexture(first_pass);
		screen_quad->T.Scale = 2.0f;

		win->Bind(); // second pass
		Windowing::SetDepth(false);
		win->Clear(Color::FromBytes(66, 31, 31));

		screen_quad->Shader_.Use();
		screen_quad->Shader_.SetFloat(1.0f / 400.0f, "offset");
		screen_quad->Shader_.SetFloatArray(kernel1, 9, "kernel");
		screen_quad->Draw(win);

		win->Flip();

		Windowing::PollEvents();
	}
	
	box->Delete();
	box_t.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
}