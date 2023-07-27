#include "Melon.hpp"


bool Melon::RenderedObject3DBuilder::SetRenderer(Melon::Mesh mesh, Melon::Renderer::VertexAttributesConfig a)
{
	m = &mesh;
	vac = a;
	state |= 1;
	return 1;
}
bool Melon::RenderedObject3DBuilder::SetShader(Melon::Shader* s)
{
	if (!s) return 0;
	sh = s;
	state |= 2;
	return 1;
}
bool Melon::RenderedObject3DBuilder::SetGraphics(Melon::ShaderGraphics* g)
{
	if (!g) return 0;
	gr = g;
	state |= 4;
	return 1;
}
bool Melon::RenderedObject3DBuilder::SetTransform3D(Melon::ShaderTransform3D* t)
{
	if (!t) return 0;
	tr = t;
	state |= 8;
	return 1;
}
bool Melon::RenderedObject3DBuilder::Done()
{
	return state & (1 + 2 + 4 + 8);
}
Melon::RenderedObject3D* Melon::RenderedObject3DBuilder::Get()
{
	if (!Done()) return 0;
	RenderedObject3D* r = new RenderedObject3D(sh, m, vac);
	r->Graphics = gr;
	r->Transform = tr;
	return r;
}
bool Melon::RenderedObject2DBuilder::SetRenderer(Melon::Mesh mesh, Melon::Renderer::VertexAttributesConfig a)
{
	m = &mesh;
	vac = a;
	state |= 1;
	return 1;
}
bool Melon::RenderedObject2DBuilder::SetShader(Melon::Shader* s)
{
	if (!s) return 0;
	sh = s;
	state |= 2;
	return 1;
}
bool Melon::RenderedObject2DBuilder::SetGraphics(Melon::ShaderGraphics* g)
{
	if (!g) return 0;
	gr = g;
	state |= 4;
	return 1;
}
bool Melon::RenderedObject2DBuilder::SetTransform2D(Melon::ShaderTransform2D* t)
{
	if (!t) return 0;
	tr = t;
	state |= 8;
	return 1;
}
bool Melon::RenderedObject2DBuilder::Done()
{
	return state & (1 + 2 + 4 + 8);
}
Melon::RenderedObject2D* Melon::RenderedObject2DBuilder::Get()
{
	if (!Done()) return 0;
	RenderedObject2D* r = new RenderedObject2D(sh, m, vac);
	r->Graphics = gr;
	r->Transform = tr;
	return r;
}

Melon::RenderedObject2D* Melon::Helpers::Objects2D::Shape(Melon::Mesh m)
{
	RenderedObject2DBuilder b;
	b.SetRenderer(m, Renderer::Position3D);
	b.SetShader(
		Helpers::ShaderLib::LoadBasic(ShaderLoadOptions(Renderer::Position3D, false, false)));
	b.SetGraphics((ShaderGraphics*)(new MaterialGraphics));
	b.SetTransform2D((ShaderTransform2D*)(new DefaultTransform2D));
	return b.Get();
}
Melon::RenderedObject2D* Melon::Helpers::Objects2D::Sprite()
{
	Renderer::VertexAttributesConfig conf = (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::TextureCoords);
	RenderedObject2DBuilder b;
	b.SetRenderer(Melon::Helpers::Meshes::Quad(), conf);
	b.SetShader(
		Helpers::ShaderLib::LoadBasic(ShaderLoadOptions(conf, false, false )));
	b.SetGraphics((ShaderGraphics*)(new TextureGraphics));
	b.SetTransform2D((ShaderTransform2D*)(new DefaultTransform2D));
	return b.Get();
}
Melon::RenderedObject3D* Melon::Helpers::Objects3D::Shape(Melon::Mesh m)
{
	RenderedObject3DBuilder b;
	b.SetRenderer(m, Renderer::Position3D);
	b.SetShader(
		Helpers::ShaderLib::LoadBasic(ShaderLoadOptions(Renderer::Position3D, false, false )));
	b.SetGraphics((ShaderGraphics*)(new MaterialGraphics));
	b.SetTransform3D((ShaderTransform3D*)(new DefaultTransform3D));
	return b.Get();
}

Melon::RenderedObject3D* Melon::Helpers::Objects3D::TexturedShape(Melon::Mesh m)
{
	Renderer::VertexAttributesConfig conf = (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::TextureCoords);
	RenderedObject3DBuilder b;
	b.SetRenderer(m, conf);
	b.SetShader(
		Helpers::ShaderLib::LoadBasic(ShaderLoadOptions(conf, false, false )));
	b.SetGraphics((ShaderGraphics*)(new TextureGraphics));
	b.SetTransform3D((ShaderTransform3D*)(new DefaultTransform3D));
	return b.Get();
}