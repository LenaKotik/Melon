#include "Melon.hpp"

using namespace Melon;

int main()
{

	DynamicVertexArray verticies = {
		Vertex(Vector3(-0.5f, 0.5f, 0.0f), Color(61.0f / 255.0f, 90.0f / 255.0f, 249.0f / 255.0f, 1.0f), Vector2(0.0f, 1.0f)),	// top left  
		Vertex(Vector3( 0.5f, 0.5f, 0.0f), Color(61.0f / 255.0f, 90.0f / 255.0f, 249.0f / 255.0f, 1.0f), Vector2(1.0f, 1.0f)),	// top right 
		Vertex(Vector3(-0.5f,-0.5f, 0.0f), Color(61.0f / 255.0f, 90.0f / 255.0f, 249.0f / 255.0f, 1.0f), Vector2(0.0f, 0.0f)),	// bottom left
		Vertex(Vector3( 0.5f,-0.5f, 0.0f), Color(61.0f / 255.0f, 90.0f / 255.0f, 249.0f / 255.0f, 1.0f), Vector2(1.0f, 0.0f)),	// bottom right
	};
	DynamicUIntArray indecies = {
		0, 2, 1,
		2, 3, 1,
	};

	Window* win = Windowing::Init(800, 600, "Tsibulya");
	if (!win) return -1;
	
	Shader* shader = ResourceLoader::LoadShader("Shader.vert", "Shader.frag");
	if (!shader) return -1;
	
	Texture* texture = ResourceLoader::LoadTexture("melon.png");
	if (!texture) return -1;

	Renderer r(verticies, indecies, true);

	while (!win->ShouldClose())
	{
		if (win->IsKeyPressed(GLFW_KEY_ESCAPE))
			win->Close();
		win->Clear({ 1, 69.0f / 255.0f, 69.0f / 255.0f, 1.0f });
		shader->Use();
		texture->Bind();
		r.Draw(GL_TRIANGLES);
		win->Flip();
		Windowing::PollEvents();
	}
	delete shader;
	delete texture;
	r.Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}