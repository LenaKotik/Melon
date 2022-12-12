#include "Melon.hpp"

void Melon::RenderedObject2D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
	Material_.Delete();
}

void Melon::RenderedObject2D::Draw(Window* win)
{
	Shader_.Use();

	Camera* cam = win->MainCamera; // abstract it

	Matrix4 ortho = Matrix4::Ortho(win->GetAspect(), 0, 100);

	SetGeometry(win);
	Shader_.SetMatrix4(cam->GetView(), "view"); // this 
	Shader_.SetMatrix4(ortho, "projection");
	
	SetGraphics(win);
	
	Renderer_.Draw();
}

void Melon::RenderedObject2D::SetGraphics(Window* win)
{
	Shader_.SetMaterial(Material_, "material");
}

void Melon::RenderedObject2D::SetGeometry(Window* win)
{
	Shader_.SetMatrix4(T.TransformationFrom(), "model");
}


void Melon::RenderedObject3D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
}

void Melon::RenderedObject3D::Draw(Window* win)
{
	Shader_.Use();
	SetGeometry(win);
	Camera3D* cam = (Camera3D*)win->MainCamera; // this too
	Matrix4 persp = Matrix4::Perspective(cam->FOV, win->GetAspect(), 0.1f, 100.0f);
	Shader_.SetMatrix4(cam->GetView(), "view");
	Shader_.SetMatrix4(persp, "projection");
	SetGraphics(win);
	Renderer_.Draw();
}

void Melon::RenderedObject3D::SetGraphics(Window* win)
{
	Shader_.SetMaterial(Material_, "material");
}

void Melon::RenderedObject3D::SetGeometry(Window* win)
{
	Shader_.SetMatrix4(T.TransformationFrom(), "model");
}

void Melon::Skybox::SetGraphics(Window* win)
{
	Shader_.SetCubeMap(CubeMap_,"cube");
}

void Melon::Skybox::Draw(Window* win)
{
	glDepthMask(false);
	Shader_.Use();
	SetGeometry(win);
	
	Camera3D* cam = (Camera3D*)win->MainCamera; // this too
	Matrix4 persp = Matrix4::Perspective(cam->FOV, win->GetAspect(), 0.1f, 100.0f);
	
	Matrix4 view = cam->GetView();

	for (int i = 0; i < 3; i++)
	{
		view.Value[3][i] = 0;
		view.Value[i][3] = 0;
	}
	view.Value[3][3] = 1;

	Shader_.SetMatrix4(view, "view");
	Shader_.SetMatrix4(persp, "projection");
	
	SetGraphics(win);
	
	Renderer_.Draw();
	glDepthMask(true);
}
