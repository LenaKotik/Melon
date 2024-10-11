#include "Melon.hpp"

using namespace Melon;

#include "Examples/movement3d.h"

int main()
{
	const FixedArray<String, 6> cubemap_names = { "px", "nx", "py", "ny", "pz", "nz" };

	Window* win = Windowing::Init(800, 800, "Speeeeeeeeen", true);
	if (!win) return - 1;

	ResourceLoader::flipYTextures = false;
	FixedArray<TextureData, 6> grass_data;
	FixedArray<TextureData, 6> skybox_data;
	for (int i = 0; i < 6; i++)
	{
		ResourceLoader::LoadTextureData(&grass_data[i], (SourceDir + "/Examples/SkyboxScene/grass/" + cubemap_names[i] + ".png").c_str());
		grass_data[i].filtering_mode = GL_NEAREST;
		ResourceLoader::LoadTextureData(&skybox_data[i], (SourceDir + "/Examples/SkyboxScene/skybox/" + cubemap_names[i] + ".jpg").c_str());
	}
	ResourceLoader::flipYTextures = true;

	CubeMap skybox_tex(skybox_data);
	CubeMap grass_tex(grass_data);

	Skybox* skybox = SkyboxFactory::Create(skybox_tex);
	
	Mesh cube_mesh = Helpers::Meshes::Cube();
	RenderedObject3D* cube = Helpers::Objects3D::MappedCube(cube_mesh);
	if (!cube) return -1;
	
	((CubeMapGraphics*)cube->Graphics)->SetCubeMap(&grass_tex);
	cube->T.Position.z = -2;
	cube->T.Rotation.Axis = Vector3(0.2, 0.9, 0.2).Normalize();
	
	Camera3D cam;
	cam.T.Position = Vector3(0.0f, 0.0f, 2.0f);
	win->MainCamera = &cam;
	cam.T.Parent = &cube->T;
	win->SetCursor(false);
	win->Maximize();

	const float rotSpeed = 2.0f;

	//std::cout << __LINE__ << ": " << glGetError() << std::endl;
	while (!win->ShouldClose())
	{
		std::cout << "dir: "		<< cam.T.Direction.x << " " << cam.T.Direction.y << " " << cam.T.Direction.z << std::endl;
		std::cout << "right: "	<< cam.T.GetRightDirection().x << " " << cam.T.GetRightDirection().y << " " << cam.T.GetRightDirection().z << std::endl;
		std::cout << "up: "		<<cam.T.GetUpDirection().x << " " << cam.T.GetUpDirection().y << " " << cam.T.GetUpDirection().z << std::endl << std::endl << std::endl;
		float delta = Time::GetDelta();
		movement3D(win, &cam, delta);
		
		cube->T.Rotation.Angle += delta * rotSpeed;

		win->Clear(Color::FromBytes(0, 0, 0, 255));
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
