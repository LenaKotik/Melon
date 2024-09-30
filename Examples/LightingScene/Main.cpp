#include "Melon.hpp"

int main()
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
