#include "Melon.hpp"

void Melon::Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Melon::Texture::Delete()
{
	glDeleteTextures(1, &this->handle);
}

Melon::Texture::~Texture()
{
	this->Delete();
}

void Melon::Shader::Use()
{
	glUseProgram(this->handle);
}

void Melon::Shader::Delete()
{
	glDeleteProgram(this->handle);
}

Melon::Shader::~Shader()
{
	if (this->ready)
		this->Delete();
}
Melon::Renderer::Renderer(DynamicVertexArray verticies, DynamicUIntArray indecies, bool is_indexed) : indexed(is_indexed)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint buffers[2];

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, buffers);
	GLuint VBO = buffers[0], EBO = buffers[1];

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), verticies.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(unsigned int), indecies.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, Color_)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, TextureCoords)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Melon::Renderer::Draw(GLenum mode)
{
	glBindVertexArray(VAO);
	if (indexed)
		glDrawElements(mode, 6, GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(mode, 0, 4); 
}

void Melon::Renderer::Delete()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if (indexed)
		glDeleteBuffers(1, &EBO);
}

Melon::Renderer::~Renderer()
{
	this->Delete();
}
