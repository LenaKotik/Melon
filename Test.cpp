#include "Melon.hpp"

using namespace Melon;

GLfloat verticies[] = {
	//     Pos					Tex 
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,				// top left  
	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,				// top right 
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,				// bottom left
	 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,				// bottom right
};
GLuint indecies[] = {
	0, 2, 1,
	2, 3, 1,
};

void init(GLuint* VAO)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint buffers[2];
	
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glGenBuffers(2, buffers);
	GLuint VBO = buffers[0], EBO = buffers[1];

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, 0, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



void renderLoop(Window* win, Texture* t, Shader* s, unsigned int v)
{
	win->Clear({ 1, 69.0f / 255.0f, 69.0f / 255.0f, 1.0f });

	s->Use();
	t->Bind();
	glBindVertexArray(v);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	
	win->Flip();
}

int main()
{
	GLuint VAO1;

	Window* win = Windowing::Init(800, 600, "Tsibulya");
	if (!win) return -1;
	
	init(&VAO1);
	
	Shader* shader = ResourceLoader::LoadShader("Shader.vert", "Shader.frag");
	if (!shader) return -1;
	
	Texture* texture = ResourceLoader::LoadTexture("melon.png");
	if (!texture) return -1;

	while (!win->ShouldClose())
	{
		if (win->IsKeyPressed(GLFW_KEY_ESCAPE))
			win->Close();
		win->MakeActive();
		renderLoop(win, texture, shader, VAO1);
		Windowing::PollEvents();
	}
	delete shader;
	delete texture;
	glDeleteVertexArrays(1, &VAO1);
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}