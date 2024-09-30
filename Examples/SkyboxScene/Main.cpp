#include "Melon.hpp"

int main()
{
	FixedArray<String, 6> skybox_names = { "px", "nx", "py", "ny", "pz", "nz" };

	Window* win = Windowing::Init(800, 800, "Speeeeeeeeen", true);
	if (!win) return - 1;

	TextureData grassTex;
	if (!ResourceLoader::LoadTextureData(&grassTex, "grass.jpeg")) return -1;
	ResourceLoader::flipYTextures = false;
	FixedArray<TextureData, 6> skybox_data;
	for (int i = 0; i < 6; i++)
	{
		ResourceLoader::LoadTextureData(&skybox_data[i], ("skybox2/" + skybox_names[i] + ".jpg").c_str());
	}
	ResourceLoader::flipYTextures = true;

	CubeMap skybox_tex(skybox_data);

	Mesh cubeMesh = Helpers::Meshes::Cube();

	Skybox* skybox = SkyboxFactory::Create(skybox_tex);

	RenderedObject3D* cube = Helpers::Objects3D::TexturedShape(cubeMesh);
	if (!cube) return -1;
	
	Texture grass(grassTex);
	TextureGraphics* cube_g = (TextureGraphics*)cube->Graphics;
	cube_g->Texture_ = grass;
	cube->T.Position.x = 2;
	cube->T.Rotation.Axis = Vector3(0.2, 0.5, 0.5);
	
	Camera3D cam;

	win->MainCamera = &cam;
	
	win->SetCursor(false);

	const float rotSpeed = 2.0f;

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		movement3D(win, delta);
		
		cube->T.Rotation.Angle += delta * rotSpeed;

		win->Clear(Color::FromBytes(0, 0, 0, 255), true);
		skybox->Draw(win);
		
		cube->Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	
	cube->Delete();
	win->Delete();
	Windowing::Terminate();

	return 0;
}
