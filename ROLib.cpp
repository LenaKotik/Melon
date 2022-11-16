#include "Melon.hpp"

Melon::RenderedObject2D* Melon::Helpers::Objects2D::Shape(Melon::Mesh m)
{
	IShader* s = Melon::ResourceLoader::LoadShader("ShapeProjection.vert", "Color.frag");
	if (!s) return 0;
	return new RenderedObject2D((Shader*)s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::Color));
}
Melon::RenderedObject2D* Melon::Helpers::Objects2D::Sprite()
{
	IShader* s = Melon::ResourceLoader::LoadShader("TextureProjection.vert", "Texture.frag");
	if (!s) return 0;
	Mesh m = Melon::Helpers::Meshes::Quad();
	return new RenderedObject2D((Shader*)s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::TextureCoords));
}
Melon::RenderedObject3D* Melon::Helpers::Objects3D::Shape(Melon::Mesh m)
{
	IShader* s = Melon::ResourceLoader::LoadShader("ShapeProjection.vert", "Color.frag");
	if (!s) return 0;
	return new RenderedObject3D((Shader*)s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::Color | Renderer::Normal));
}

Melon::RenderedObject3D* Melon::Helpers::Objects3D::TexturedShape(Melon::Mesh m)
{
	IShader* s = Melon::ResourceLoader::LoadShader("TextureProjection.vert", "Texture.frag");
	if (!s) return 0;
	return new RenderedObject3D((Shader*)s, m, (Renderer::VertexAttributesConfig)(Renderer::Position3D | Renderer::TextureCoords | Renderer::Normal));
}