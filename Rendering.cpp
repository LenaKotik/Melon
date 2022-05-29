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
void Melon::Shader::SetFloat(float v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniform1f(l, v);
}
void Melon::Shader::SetInt(int v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniform1i(l, v);
}
void Melon::Shader::SetBool(bool v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniform1i(l, v);
}
void Melon::Shader::SetVector2(Vector2 v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniform2f(l, v.x, v.y);
}
void Melon::Shader::SetVector3(Vector3 v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniform3f(l, v.x, v.y, v.z);
}
void Melon::Shader::SetMatrix4(Matrix4 v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniformMatrix4fv(l, 1, false, glm::value_ptr(v));
}
Melon::Renderer::Renderer(Mesh* mesh) : indexed(mesh->is_indexed), indC(mesh->indecies.size()), vertC(mesh->verticies.size())
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->verticies.size() * sizeof(Vertex), mesh->verticies.data(), GL_STATIC_DRAW);

	if (indexed)
	{
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indecies.size() * sizeof(unsigned int), mesh->indecies.data(), GL_STATIC_DRAW);
	}


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
		glDrawElements(mode, indC, GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(mode, 0, vertC); 
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

void Melon::Mesh::SetColor(Melon::Color c)
{
	for (int i = 0; i < verticies.size(); i++)
	{
		verticies[i].Color_ = c;
	}
}