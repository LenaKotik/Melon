#include "Melon.hpp"

Melon::RenderedObject2D* Melon::Helpers::Objects2D::Shape(Melon::Mesh m)
{
	Shader* s = Helpers::ShaderLib::LoadBasic(ShaderLoadOptions(Renderer::Position3D, false, false));
	if (!s) return 0;
	return new RenderedObject2D(s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D));
}
Melon::RenderedObject2D* Melon::Helpers::Objects2D::Sprite()
{
	Shader* s = Helpers::ShaderLib::LoadBasic(ShaderLoadOptions((Renderer::VertexAttributesConfig)(Renderer::Position3D|Renderer::TextureCoords), false, false ));
	if (!s) return 0;
	Mesh m = Melon::Helpers::Meshes::Quad();
	return new RenderedObject2D(s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::TextureCoords));
}
Melon::RenderedObject3D* Melon::Helpers::Objects3D::Shape(Melon::Mesh m)
{
	Shader* s = Helpers::ShaderLib::LoadBasic(ShaderLoadOptions(Renderer::Position3D, false, false ));
	if (!s) return 0;
	return new RenderedObject3D(s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D));
}

Melon::RenderedObject3D* Melon::Helpers::Objects3D::TexturedShape(Melon::Mesh m)
{
	Shader* s = Helpers::ShaderLib::LoadBasic(ShaderLoadOptions((Renderer::VertexAttributesConfig)(Renderer::Position3D|Renderer::TextureCoords), false, false ));
	if (!s) return 0;
	return new RenderedObject3D(s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::TextureCoords | Renderer::Normal));
}