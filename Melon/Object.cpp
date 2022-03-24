#include "Melon.hpp"

Melon::Object::Object(Shader sh, Material mat, Mesh mesh) : shader(sh), material(mat), mesh(mesh), stride(0)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.verticies.size() * sizeof GLfloat, mesh.verticies.data(), GL_STATIC_DRAW);

	if (mesh.isIndexed)
	{
		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indecies.size() * sizeof GLint, mesh.indecies.data(), GL_STATIC_DRAW);
	}

	for (GLint i = 0; i < mesh.attribs.size(); i++)
	{
		stride += mesh.attribs[i];
	}
	GLint offset = 0;
	for (GLint i = 0; i < mesh.attribs.size(); i++)
	{
		offset += mesh.attribs[i];
		glVertexAttribIPointer(i, mesh.attribs[i], GL_FLOAT, stride * sizeof GLfloat, (void*)offset);
	}

	//glBindVertexArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
Melon::Object::~Object()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}
void Melon::Object::Draw()
{
	glBindVertexArray(_vao);

	glUseProgram(shader._ptr);

	// set uniforms ...

	shader.SetFloat(glfwGetTime(), "TIME");

	shader.SetMaterial(material);

	if (mesh.isIndexed)
		glDrawElements((GLenum)mesh.type, mesh.indecies.size(), GL_UNSIGNED_INT, NULL);
	else
		glDrawArrays((GLenum)mesh.type, 0, mesh.verticies.size() / stride);

	//glBindVertexArray(0);
}