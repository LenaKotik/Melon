#include "Melon.hpp"

using namespace Melon;

void movement3D(Window* win, float delta)
{
	static MouseOffsetController ctrl(win);
	KeyPressVector3Controller mov(win);
	PitchYaw2DirectionController dir;
	static float pitch, yaw;
	static bool cur = false;
	const float CamSpeed = 2.0f;
	const float sensitivity = 0.1f;

	if (win->IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		win->SetCursor(cur);
		cur = !cur;
	}
	Camera3D* cam = (Camera3D*)win->MainCamera;

	Vector3 movement = Vector3(mov.Value()) * delta * CamSpeed;
	
	cam->Position += cam->Direction * movement.z;
	cam->Position += cam->Up * movement.y;
	cam->Position += cam->Right * movement.x;

	Vector2 offset = Vector2(ctrl.Value()) * sensitivity;

	pitch += offset.y;
	yaw += offset.x;
	pitch = clamp(pitch, -89.0f, 89.0f);

	cam->SetDirection(dir.Value({ deg2rad(pitch), deg2rad(yaw) }));
}

int RandomTextureCubesScene()
{
	Window* win = Windowing::Init(800, 800, "Box World", true);
	if (!win) return -1;
	win->SetCursor(false);

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, "box.jpg")) return -1;
	Texture texture(td);

	Camera3D cam;
	cam.Position = Vector3(0.0f, 0.0f, 3.0f);
	win->MainCamera = &cam;

	RenderedObject3D tm = *Helpers::Objects3D::TexturedShape(Helpers::Meshes::Cube());
	tm.Material_.Albedo = texture;

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
		printf("FPS:%d\n", (int)roundf(1 / delta));
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
int Simple2DScene()
{
	Window* win = Windowing::Init(800, 800, "2D Crazyness", false);
	if (!win) return -1;
	
	RenderedObject2D shape = *Helpers::Objects2D::Shape(Helpers::Meshes::Quad());
	RenderedObject2D shape2 = *Helpers::Objects2D::Shape(Helpers::Meshes::Circle(40));
	RenderedObject2D sprite = *Helpers::Objects2D::Sprite();

	TextureData td;
	if (!ResourceLoader::LoadTextureData(&td, "melon.png")) return -1;

	shape.Material_.Albedo = Color::FromBytes(251, 71, 71, 255);
	shape2.Material_.Albedo = Color::FromBytes(32, 217, 36, 255);
	sprite.Material_.Albedo = Texture(td);

	sprite.T.Scale = Vector2(2, 1);

	Camera2D cam;
	win->MainCamera = &cam;

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
int LightingScene()
{
	Window* win = Windowing::Init(800, 800, "Phong Lighting", true);
	if (!win) return -1;

	win->SetCursor(false);

	Mesh m = Helpers::Meshes::Cube();
	Shader* s = Helpers::ShaderLib::LoadBasic(Helpers::ShaderLoadOptions((Renderer::VertexAttributesConfig)(Renderer::Position3D|Renderer::Normal), false, true));
	RenderedObject3D shape(s,m,(Renderer::VertexAttributesConfig)(Renderer::Position3D|Renderer::Normal));

	m.PrimitiveType = GL_POINTS;
	Shader* normalGeom = Helpers::ShaderLib::LoadGeom("NormalDisplay");
	if (!normalGeom) return -1;
	RenderedObject3D normals(normalGeom, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::Normal));;

	shape.Material_ = Helpers::Materials::Gold();

	m.PrimitiveType = GL_TRIANGLES;
	RenderedObject3D light = *Helpers::Objects3D::Shape(m);
	light.Material_.Albedo = Color::FromBytes(255, 255, 255, 255);
	light.T.Position = Vector3(0, 1, -2);

	Camera3D cam;
	cam.Position = Vector3(0, 0.5, 1);

	win->MainCamera = &cam;

	normals.Shader_.Use();
	normals.Shader_.SetColor(Color::FromBytes(249, 255, 28, 255), "LineColor");
	normals.Shader_.SetFloat(0.2f, "length_");

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		//printf("FPS:%d\n", (int)roundf(1 / delta));
		movement3D(win, delta);

		win->Clear(Color::FromBytes(29, 29, 29, 255), true);

		shape.Shader_.Use();
		shape.Shader_.SetColor(light.Material_.Albedo.Solid, "LightColor");
		shape.Shader_.SetVector3(light.T.Position, "LightPosition");
		shape.Shader_.SetVector3(cam.Position, "CameraPosition");
		shape.Draw(win);
		normals.Draw(win);
		light.Draw(win);

		Windowing::PollEvents();
		win->Flip();
	}
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
}
int GeometryShaderScene()
{
	Window* win = Windowing::Init(800, 800, "Geometry Shader", true);
	if (!win) return -1;

	win->SetCursor(false);

	Mesh m = Helpers::Meshes::Cube();
	RenderedObject3D shape = *Helpers::Objects3D::Shape(m);
	shape.Material_.Albedo = Color::FromBytes(251, 71, 71, 255);
	m.PrimitiveType = GL_POINTS;

	RenderedObject3D points = *Helpers::Objects3D::Shape(m);

	Shader* geom = Helpers::ShaderLib::LoadGeom("Point2Square");
	if (!geom) return -1;
	points.Shader_ = *geom;
	points.Material_.Albedo = Color::FromBytes(251, 71, 71, 255);

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
		points.Shader_.SetFloat(0.2, "radius");
		points.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	points.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}
int ModelImportScene()
{
	return 0;
}
int SoundScene()
{

class TimeoutEventListener : EventListener
{
	void Callback(EventArgs*) override
	{
		std::cout << "time out!\n";
	}
};
	Window * win = Windowing::Init(800, 800, "Sounds", false);
	if (!win) return -1;

	AudioDevice* audio = Windowing::OpenAudioDevice("OpenAL soft");
	if (!audio) return -1;

	AudioBuffer buffer;
	ResourceLoader::LoadAudio(&buffer, "me.wav");
	
	RenderedObject2D circle = *Helpers::Objects2D::Shape(Helpers::Meshes::Circle(40));
	circle.Material_.Albedo = Color::FromBytes(243, 28, 28, 255);
	
	Camera2D cam;
	win->MainCamera = &cam;
	
	Timer playback_timer;

	AudioSource src;
	src.Play(&buffer);

	playback_timer.Start(2.0f);
	TimeoutEventListener lstner;
	playback_timer.Timeout += (EventListener*)&lstner;

	std::cout << audio->GetName();

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();

		if (playback_timer.isRunning())std::cout << Time::GetTime() << '\n';

		win->Clear(Color::FromBytes(255, 255, 255, 255), false);

		circle.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	src.Delete();
	buffer.Delete();
	circle.Delete();
	Windowing::Terminate();
	return 0;
}
int ConsoleTest()
{
	Vector3 a(0), b(0);
	CoordinateSystem3D coord;
	coord.Position = { 2,4,4 };
	a = coord.TransformationTo().Transform(a);
	std::cout << a.x << ' ' << a.y << ' ' << a.z << '\n';
	b = coord.TransformationFrom().Transform(b);
	std::cout << b.x << ' ' << b.y << ' ' << b.z << '\n';
	return 0;
}
int SpinScene()
{
	FixedArray<String, 6> skybox_names = { "px", "nx", "py", "ny", "pz", "nz" };

	Window* win = Windowing::Init(800, 800, "Speeeeeeeeen", true);
	if (!win) return - 1;

	TextureData grassTex;
	if (!ResourceLoader::LoadTextureData(&grassTex, "grass.jpeg")) return -1;

	FixedArray<TextureData, 6> skybox_data;
	for (int i = 0; i < 6; i++)
	{
		ResourceLoader::LoadTextureData(&skybox_data[i], ("cubemap2/" + skybox_names[i] + ".png").c_str());
	}

	CubeMap skybox_tex(skybox_data);

	Mesh cubeMesh = Helpers::Meshes::Cube();

	MappedCube* skybox = new MappedCube();
	skybox->CubeMap_ = skybox_tex;

	RenderedObject3D* cube = Helpers::Objects3D::TexturedShape(cubeMesh);
	if (!cube) return -1;
	
	Texture grass(grassTex);
	cube->Material_.Albedo = grass;
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
		glDepthMask(false);
		skybox->Draw(win);
		glDepthMask(true);
		
		cube->Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	
	cube->Delete();
	win->Delete();
	Windowing::Terminate();

	return 0;
}

int main()
{
	return SpinScene();
    return RandomTextureCubesScene();
	return Simple2DScene();
	return SoundScene();
	return LightingScene();
	return GeometryShaderScene();
	return ConsoleTest();
	return ModelImportScene();
}