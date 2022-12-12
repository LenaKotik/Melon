#include "Melon.hpp"


Melon::Texture::Texture(TextureData data)
{
	const GLenum color_spaces[] = { NULL, GL_RED, GL_RG, GL_RGB, GL_RGBA };

	GLenum color_space = color_spaces[data.channels];

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, color_space, data.width, data.height, 0, color_space, GL_UNSIGNED_BYTE, data.data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Melon::Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Melon::Texture::Delete()
{
	glDeleteTextures(1, &this->handle);
}
GLuint* Melon::TextureUnitManager::units = nullptr;
Byte Melon::TextureUnitManager::cur = 0;
GLint Melon::TextureUnitManager::MaxUnits = 16;
GLint Melon::TextureUnitManager::GetMaxTextureUnits()
{
	return MaxUnits;
}

Melon::CubeMap::CubeMap(FixedArray<TextureData, 6> data)
{
	glGenTextures(1, &handle);
	Bind();
	const GLint formats[]{GL_RED, GL_RG, GL_RGB, GL_RGBA};
	for (int i = 0; i < 6; i++)
	{
		GLint format = formats[data[i].channels-1];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, data[i].width, data[i].height,
			0, format, GL_UNSIGNED_BYTE, data[i].data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
}

void Melon::CubeMap::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
}

void Melon::CubeMap::Delete()
{
	glDeleteTextures(1, &handle);
}

void Melon::TextureData::Delete()
{
	stbi_image_free(data);
}

Byte Melon::TextureUnitManager::Add(Texture t) 
{
	// if texture overflow happens, we just wrap around, later should make this behavior configurable
	if (cur >= MaxUnits) cur = 0; 
	glActiveTexture(GL_TEXTURE0 + cur);
	t.Bind();
	units[cur] = t.handle;
	return cur++;
}

Byte Melon::TextureUnitManager::Add(CubeMap t)
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
	Byte u = TextureUnitManager::Add(t);
	return SetInt(u, name);
}

void Melon::Shader::SetCubeMap(CubeMap t, const char* name)
{
	Byte u = TextureUnitManager::Add(t);
	return SetInt(u, name);
}

void Melon::Shader::SetBrush(Melon::Brush b, const char* c_name)
{
	String name(c_name);
	SetBool(b.isSolid, (name+"IsSolid").c_str());
	if (b.isSolid) SetColor(b.Solid, (name + "Solid").c_str());
	else SetTexture(b.Mapped, (name + "Map").c_str());
}

void Melon::Shader::SetMaterial(Melon::Material m, const char* c_name)
{
	String name(c_name);
	SetBrush(m.Albedo, (name + ".albedo").c_str());
	SetBrush(m.Diffuse, (name + ".diffuse").c_str());
	SetBrush(m.Specular, (name + ".specular").c_str());
	SetFloat(m.Shininess, (name + ".shininess").c_str());
	SetFloat(m.Ambient, (name + ".ambient").c_str());
}

Melon::DynamicFloatArray Melon::Renderer::GenBuffer(DynamicVertexArray arr, VertexAttributesConfig bitmask, int* stride, DynamicUIntArray * offsets, DynamicUIntArray* sizes)
{
	if (bitmask & VertexAttributesConfig::Position3D)
	{
		offsets->PushBack((GLuint)*stride);
		sizes->PushBack(3);
		*stride += 3;
	}
	if (bitmask & VertexAttributesConfig::TextureCoords)
	{
		offsets->PushBack((GLuint)*stride);
		sizes->PushBack(2);
		*stride += 2;
	}
	if (bitmask & VertexAttributesConfig::Color)
	{
		offsets->PushBack((GLuint)*stride);
		sizes->PushBack(3);
		*stride += 3;
	}
	if (bitmask & VertexAttributesConfig::Normal)
	{
		offsets->PushBack((GLuint)*stride);
		sizes->PushBack(3);
		*stride += 3;
	}
	DynamicFloatArray res;
	for (int i = 0; i < arr.Size(); i++)
	{
		if (bitmask & VertexAttributesConfig::Position3D)
		{
			res.PushBack(arr[i].Position.x);
			res.PushBack(arr[i].Position.y);
			res.PushBack(arr[i].Position.z);
		}
		if (bitmask & VertexAttributesConfig::TextureCoords)
		{
			res.PushBack(arr[i].TextureCoords.x);
			res.PushBack(arr[i].TextureCoords.y);
		}
		if (bitmask & VertexAttributesConfig::Color)
		{
			res.PushBack(arr[i].Color_.R);
			res.PushBack(arr[i].Color_.G);
			res.PushBack(arr[i].Color_.B);
		}
		if (bitmask & VertexAttributesConfig::Normal)
		{
			res.PushBack(arr[i].Normal.x);
			res.PushBack(arr[i].Normal.y);
			res.PushBack(arr[i].Normal.z);
		}
	}
	return res;
}
Melon::Renderer::Renderer(Mesh* mesh, VertexAttributesConfig a) : indexed(mesh->is_indexed), indC(mesh->indecies.Size()), vertC(mesh->verticies.Size()), PrimitiveType(mesh->PrimitiveType)
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
	glBufferData(GL_ARRAY_BUFFER, buffer.Size() * sizeof(float), buffer.Data, GL_STATIC_DRAW);

	if (indexed)
	{
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indecies.Size() * sizeof(unsigned int), mesh->indecies.Data, GL_STATIC_DRAW);
	}

	for (int i = 0; i < sizes.Size(); i++)
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
	for (int i = 0; i < verticies.Size(); i++)
	{
		verticies[i].Color_ = c;
	}
}

void Melon::Mesh::ComputeNormals(Vector3 center) // probably works only with simple shapes
{
	for (int i = 0; i < verticies.Size(); i++) // loop thru all vertieces
		verticies[i].Normal = (verticies[i].Position - center).Normalize(); // direction from center to the vertex
}

void Melon::Material::Delete()
{
	if (!Albedo.isSolid) Albedo.Mapped.Delete();
	if (!Diffuse.isSolid) Diffuse.Mapped.Delete();
	if (!Specular.isSolid) Specular.Mapped.Delete();
}