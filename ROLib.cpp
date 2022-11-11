#include "Melon.hpp"

Melon::RenderedObject2D* Melon::Helpers::Objects2D::Shape(Melon::Color c)
{
	Shader* s = Melon::ResourceLoader::LoadShader("ShapeProjection.vert", "Color.frag");
	if (!s) return 0;
	Mesh m = Melon::Helpers::Meshes::Quad();
	return new RenderedObject2D(s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::Color));
}