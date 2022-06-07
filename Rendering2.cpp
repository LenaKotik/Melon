#include "Melon.hpp"

void Melon::RenderedObject2D::BeginDraw(Window* win)
{
	Matrix4 model(1.0f);
	model = model.Scale(Vector3(Scale.x, Scale.y, 1.0f));
	model = model.Rotate(Rotation, Vector3(0.0f, 0.0f, 1.0f));
	model = model.Translate(Vector3(Position.x, Position.y, 0.0f));

	Camera2D* cam = (Camera2D*)win->MainCamera;

	Matrix4 ortho(1.0f); // TODO: implement orthographic projection

	Shader_.Use();
	Shader_.SetMatrix4(model, "model");
	Shader_.SetMatrix4(cam->GetView(), "view");
	Shader_.SetMatrix4(ortho, "projection");
}
void Melon::RenderedObject2D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
}
Melon::Shape2D::Shape2D(Mesh m) : //              must be changed
	RenderedObject2D(ResourceLoader::LoadShader("Projection.vert", "Color.frag"), m),
	Color_(1, 1, 1, 1) {}

void Melon::Shape2D::Draw(Window* win)
{
	BeginDraw(win);
	Shader_.SetColor(Color_, "Color");
	Renderer_.Draw();
}

Melon::Shape2D::~Shape2D()
{
	Delete();
}

Melon::Sprite2D::Sprite2D(Texture * tex) : 
	RenderedObject2D(ResourceLoader::LoadShader("Projection.vert", "Texture.frag"), Helpers::Meshes::Quad()),
	Texture_(*tex) {}

void Melon::Sprite2D::Draw(Window* win)
{
	BeginDraw(win);
	Texture_.Bind();
	Renderer_.Draw();
}

Melon::Sprite2D::~Sprite2D()
{
	Delete();
	Texture_.Delete();
}

void Melon::RenderedObject3D::BeginDraw(Window* win)
{
	Matrix4 model(1.0f);
	model = model.Scale(Scale);
	model = model.Rotate(1.0f, Rotation);
	model = model.Translate(Position);

	Camera3D* cam = (Camera3D *)win->MainCamera;
	Matrix4 persp = Matrix4::Perspective(cam->FOV, win->GetAspect(), 0.1f, 100.0f);

	Shader_.Use();
	Shader_.SetMatrix4(model, "model");
	Shader_.SetMatrix4(cam->GetView(), "view");
	Shader_.SetMatrix4(persp, "projection");
}

void Melon::RenderedObject3D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
}

Melon::Shape3D::Shape3D(Mesh m) : RenderedObject3D(ResourceLoader::LoadShader("Projection.vert", "Color.frag"), m),
                                  Color_(1, 1, 1, 1) {}

void Melon::Shape3D::Draw(Window* win)
{
	BeginDraw(win);
	Shader_.SetColor(Color_, "Color");
	Renderer_.Draw();
}

Melon::Shape3D::~Shape3D()
{
	Delete();
}

Melon::TexturedMesh::TexturedMesh(Texture* tex, Mesh m) : RenderedObject3D(ResourceLoader::LoadShader("Projection.vert", "Texture.frag"), m),
                                                          Texture_(*tex){}

void Melon::TexturedMesh::Draw(Window* win)
{
	BeginDraw(win);
	Texture_.Bind();
	Renderer_.Draw();
}

Melon::TexturedMesh::~TexturedMesh()
{
	Delete();
	Texture_.Delete();
}
