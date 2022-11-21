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

GLint Melon::TextureUnitManager::GetMaxTextureUnits()
{
	return MaxUnits;
}

GLbyte Melon::TextureUnitManager::Add(Texture& t) // pass by reference because copying would be unnecessery
{
	// if texture overflow happens, we just wrap around, later should make this behavior configurable
	if (cur >= MaxUnits) cur = 0; 
	glActiveTexture(GL_TEXTURE0 + cur);
	t.Bind();
	units[cur] = t.handle;
	return cur++;
}

void Melon::TextureUnitManager::Clear()
{
	cur = 0;
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
void Melon::Shader::SetColor(Color v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniform4f(l, v.R, v.G, v.B, v.A);
}
void Melon::Shader::SetMatrix4(Matrix4 v, const char* name)
{
	GLint l = glGetUniformLocation(handle, name);
	glUniformMatrix4fv(l, 1, false, (const float*)v.Transpose().Value);
}
void Melon::Shader::SetTexture(Texture t, const char* name)
{
	GLbyte u = TextureUnitManager::Add(t);
	return SetInt(u, name);
}
Melon::DynamicFloatArray Melon::Renderer::GenBuffer(DynamicVertexArray arr, VertexAttributesConfig bitmask, int* stride, DynamicUIntArray * offsets, DynamicUIntArray* sizes)
{
	int i = 0;
	if (bitmask & VertexAttributesConfig::Position3D)
	{
		offsets->push_back((GLuint)*stride);
		sizes->push_back(3);
		*stride += 3;
	}
	if (bitmask & VertexAttributesConfig::TextureCoords)
	{
		offsets->push_back((GLuint)*stride);
		sizes->push_back(2);
		*stride += 2;
	}
	if (bitmask & VertexAttributesConfig::Color)
	{
		offsets->push_back((GLuint)*stride);
		sizes->push_back(3);
		*stride += 3;
	}
	if (bitmask & VertexAttributesConfig::Normal)
	{
		offsets->push_back((GLuint)*stride);
		sizes->push_back(3);
		*stride += 3;
	}
	DynamicFloatArray res;
	for (int i = 0; i < arr.size(); i++)
	{
		if (bitmask & VertexAttributesConfig::Position3D)
		{
			res.push_back(arr[i].Position.x);
			res.push_back(arr[i].Position.y);
			res.push_back(arr[i].Position.z);
		}
		if (bitmask & VertexAttributesConfig::TextureCoords)
		{
			res.push_back(arr[i].TextureCoords.x);
			res.push_back(arr[i].TextureCoords.y);
		}
		if (bitmask & VertexAttributesConfig::Color)
		{
			res.push_back(arr[i].Color_.R);
			res.push_back(arr[i].Color_.G);
			res.push_back(arr[i].Color_.B);
		}
		if (bitmask & VertexAttributesConfig::Normal)
		{
			res.push_back(arr[i].Normal.x);
			res.push_back(arr[i].Normal.y);
			res.push_back(arr[i].Normal.z);
		}
	}
	return res;
}
Melon::Renderer::Renderer(Mesh* mesh, VertexAttributesConfig a) : indexed(mesh->is_indexed), indC(mesh->indecies.size()), vertC(mesh->verticies.size()), PrimitiveType(mesh->PrimitiveType)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	int stride = 0;
	DynamicUIntArray offsets;
	DynamicUIntArray sizes;
	DynamicFloatArray buffer = GenBuffer(mesh->verticies, a, &stride, &offsets, &sizes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);

	if (indexed)
	{
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indecies.size() * sizeof(unsigned int), mesh->indecies.data(), GL_STATIC_DRAW);
	}

	for (int i = 0; i < sizes.size(); i++)
	{
		glVertexAttribPointer(i, sizes[i], GL_FLOAT, 0, stride * sizeof(float), (void*)(offsets[i] * sizeof(float)));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Melon::Renderer::Draw()
{
	glBindVertexArray(VAO);
	if (indexed)
		glDrawElements(PrimitiveType, indC, GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(PrimitiveType, 0, vertC); 
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

void Melon::Mesh::ComputeNormals(Vector3 center) // probably works only with simple shapes
{
	for (int i = 0; i < verticies.size(); i++) // loop thru all vertieces
		verticies[i].Normal = (verticies[i].Position - center).Normalize(); // direction from center to the vertex
}
