#include "Melon.hpp"

void Melon::RenderedObject2D::BeginDraw(Window* win)
{
	Matrix4 model(1.0f);
	model = model.Scale(Vector3(Scale.x, Scale.y, 1.0f));
	model = model.Rotate(Rotation, Vector3(0.0f, 0.0f, 1.0f));
	model = model.Translate(Vector3(Position.x, Position.y, 0.0f));

	Camera2D* cam = (Camera2D*)win->MainCamera;

	Matrix4 ortho = Matrix4::Ortho(win->GetAspect(), 0, 100); // TODO: implement orthographic projection

	Shader_.Use();
	Shader_.SetMatrix4(model, "model");
	Shader_.SetMatrix4(cam->GetView(), "view");
	Shader_.SetMatrix4(ortho, "projection");

	Texture_.Bind();
	Shader_.SetColor(Color_, "Color");
}
void Melon::RenderedObject2D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
	Texture_.Delete();
}

void Melon::RenderedObject2D::Draw(Window* win)
{
	BeginDraw(win);
	Shader_.Use();
	Renderer_.Draw();
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

	Texture_.Bind();
	Shader_.SetColor(Color_, "Color");
}

void Melon::RenderedObject3D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
}

void Melon::RenderedObject3D::Draw(Window* win)
{
	BeginDraw(win);
	Shader_.Use();
	Renderer_.Draw();
}
