#include "Melon.hpp"

void Melon::RenderedObject2D::BeginDraw(Window* win)
{
	Matrix4 model = T.TransformationFrom();

	Camera* cam = win->MainCamera; // abstract it

	Matrix4 ortho = Matrix4::Ortho(win->GetAspect(), 0, 100); // TODO: implement orthographic projection

	Shader_.Use();
	Shader_.SetMatrix4(model, "model");
	Shader_.SetMatrix4(cam->GetView(), "view"); // this 
	Shader_.SetMatrix4(ortho, "projection");

	Shader_.SetMaterial(Material_, "material");
}
void Melon::RenderedObject2D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
	Material_.Delete();
}

void Melon::RenderedObject2D::Draw(Window* win)
{
	BeginDraw(win);
	Shader_.Use();
	Renderer_.Draw();
}

void Melon::RenderedObject3D::BeginDraw(Window* win)
{

	Matrix4 model = T.TransformationFrom();

	Camera3D* cam = (Camera3D *)win->MainCamera; // this too
	Matrix4 persp = Matrix4::Perspective(cam->FOV, win->GetAspect(), 0.1f, 100.0f);

	Shader_.Use();
	Shader_.SetMatrix4(model, "model");
	Shader_.SetMatrix4(cam->GetView(), "view");
	Shader_.SetMatrix4(persp, "projection");

	Shader_.SetMaterial(Material_, "material");
}

void Melon::RenderedObject3D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
}

void Melon::RenderedObject3D::Draw(Window* win)
{
	BeginDraw(win);
	Renderer_.Draw();
}
