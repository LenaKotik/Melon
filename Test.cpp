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
	win->MainCamera = &cam;

	RenderedObject3D tm = *Helpers::Objects3D::TexturedShape(Helpers::Meshes::Cube());
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

	shape.Graphics->SetColor(Color::FromBytes(251, 71, 71, 255));
	shape2.Graphics->SetColor(Color::FromBytes(32, 217, 36, 255));
	sprite.Graphics->SetTexture(Texture(td));

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
	RenderedObject3DBuilder builder;

	builder.SetRenderer(m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::Normal));
	builder.SetShader(s);
	builder.SetGraphics((ShaderGraphics*)new MaterialGraphics);
	builder.SetTransform3D((ShaderTransform3D*)new DefaultTransform3D);

	RenderedObject3D shape = *builder.Get();

	m.PrimitiveType = GL_POINTS;
	Shader* normalGeom = Helpers::ShaderLib::LoadGeom("NormalDisplay");
	if (!normalGeom) return -1;

	builder.SetRenderer(m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::Normal));
	builder.SetShader(normalGeom);
	builder.SetGraphics((ShaderGraphics*)new ColorGraphics);
	RenderedObject3D normals = *builder.Get();


	shape.Graphics->SetMaterial(Helpers::Materials::Gold());

	m.PrimitiveType = GL_TRIANGLES;
	RenderedObject3D light = *Helpers::Objects3D::Shape(m);
	Color lightColor = Color::FromBytes(255, 255, 255, 255);
	light.Graphics->SetColor(lightColor);
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
		shape.Shader_.SetColor(lightColor, "LightColor");
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
	shape.Graphics->SetColor(Color::FromBytes(251, 71, 71, 255),0);
	m.PrimitiveType = GL_POINTS;

	RenderedObject3D points = *Helpers::Objects3D::Shape(m);

	Shader* geom = Helpers::ShaderLib::LoadGeom("Point2Square");
	if (!geom) return -1;
	points.Shader_ = *geom;
	points.Graphics->SetColor(Color::FromBytes(251, 71, 71, 255),0);

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
		points.Shader_.SetFloat(0.2, "scale");
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
	circle.Graphics->SetColor(Color::FromBytes(243, 28, 28, 255));
	
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
	DynamicIntArray I = { 1, 3, 5, 7 };
	I.Insert(2, 1);
	I.Insert(4, 3);
	for (int i : I)
		std::cout << i << ' ';
	return 0;
}
int SpinScene()
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
int TreeScene()
{
	Window* win = Windowing::Init(800, 800, "Cube", true);
	if (!win) return -1;
	win->SetCursor(0);

	Mesh m = Helpers::Meshes::Cube();

	RenderedObject3D* cube = Helpers::Objects3D::Shape(m);
	if (!cube) return -1;
	cube->Graphics->SetColor(Color::FromBytes(200,60,60,255));

	RenderedObject3D* bar = Helpers::Objects3D::Shape(m);
	if (!bar) return -1;
	
	ChildTransform3D bar_t;
	bar_t.Parent = &cube->T;
	bar->Transform = (ShaderTransform3D*)&bar_t;

	bar->Graphics->SetColor(Color::FromBytes(70, 200, 70, 255));
	bar->T.Position = Vector3(0, 2, 0);
	bar->T.Scale = Vector3(0.5, 3, 0.5);

	Camera3D cam;
	win->MainCamera = (Camera*)&cam;

	Time::GetDelta();
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();
		movement3D(win, delta);

		cube->T.Rotation = Rotator(2*(Time::GetTime()),Vector3(1,0,0));
		bar->T.Position = Vector3(0, sin(2 * Time::GetTime()), 0);

		win->Clear(Color::FromBytes(20, 20, 50, 255), true);
		cube->Draw(win);
		bar->Draw(win);
		win->Flip();
		Windowing::PollEvents();
	}
	cube->Delete();
	bar->Delete();
	win->Delete();
	Windowing::Terminate();
	return 0;
}
int AnimationScene()
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
	pos_t.Add({ Vector2(-0.8,0), 1.5 });
	
	for (Keyframe<Vector2> p : pos_t.keyframes)
		std::cerr << p.value.x << " " << p.value.y << "  ";

	anim.Add(pos_t);
	anim.Add(scale_t);
	
	Camera2D cam;
	win->MainCamera = (Camera*)&cam;

	anim.loop = true;
	anim.Play();

	while (!win->ShouldClose())
	{
		//std::cout << shape->T.Position.x << " " << shape->T.Position.y << "\n";
		win->Clear(Color::FromBytes(83, 197, 60, 255),false);
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

int main()
{
	return AnimationScene();
	return TreeScene();
	return ConsoleTest();
	return LightingScene();
	return GeometryShaderScene();
	return SpinScene();
	return Simple2DScene();
    return RandomTextureCubesScene();
	//return ModelImportScene();
	//return SoundScene();
}