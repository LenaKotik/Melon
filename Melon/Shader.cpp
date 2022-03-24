#include "Melon.hpp"

Melon::Shader::Shader()
{
	_ptr = glCreateProgram();
}
Melon::Shader::~Shader()
{
	for (GLuint s : _shdrs)
	{
		glDeleteShader(s);
	}
	glDeleteProgram(_ptr);
}
bool Melon::Shader::AttachFromSource(const char *src, ShaderType t, char *err)
{
	GLuint sh = glCreateShader((GLenum)t);
	GLint size_src = strlen(src);
	glShaderSource(sh, 1, &src, &size_src);
	glCompileShader(sh);
	GLint success;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(sh, 512, NULL, err);
		return false;
	}
	glAttachShader(_ptr, sh);
	_shdrs.push_back(sh);
	return true;
}
bool Melon::Shader::Attach(const char* file, ShaderType t, char* err)
{
	std::ifstream f;
	f.open(file);
	std::string src;
	char b[100];
	while (!f.eof())
	{
		f.getline(b, 100);
		src.append(b);
		src.append("\n");
	}
	f.close();
	return AttachFromSource(src.c_str(), t, err);
}
bool Melon::Shader::Link(char *err)
{
	glLinkProgram(_ptr);
	GLint success;
	glGetProgramiv(_ptr, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_ptr, 512, NULL, err);
		return false;
	}
	for (GLuint s : _shdrs)
	{
		glDeleteShader(s);
	}
}

void Melon::Shader::SetBool(bool v, const char* name)
{
	GLint loc = glGetUniformLocation(_ptr, name);
	glUniform1i(loc, v);
}

void Melon::Shader::SetInt(GLint v, const char* name)
{
	GLint loc = glGetUniformLocation(_ptr, name);
	glUniform1i(loc, v);
}

void Melon::Shader::SetFloat(GLfloat v, const char* name)
{
	GLint loc = glGetUniformLocation(_ptr, name);
	glUniform1f(loc, v);
}

void Melon::Shader::SetVec4(GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw, const char* name)
{
	GLint loc = glGetUniformLocation(_ptr, name);
	glUniform4f(loc, vx, vy, vz, vw);
}

void Melon::Shader::SetVec3(GLfloat vx, GLfloat vy, GLfloat vz, const char* name)
{
	GLint loc = glGetUniformLocation(_ptr, name);
	glUniform3f(loc, vx, vy, vz);
}

void Melon::Shader::SetTexture(Texture &v, const char* name)
{
	GLint loc = glGetUniformLocation(_ptr, name);
	glUniform1i(loc, v._unit);
}

void Melon::Shader::SetMaterial(Material v)
{
	SetBool(v.AlbedoUseTex, "material.albedoUseTex");
	SetVec4(v.Albedo.R, v.Albedo.G, v.Albedo.B, v.Albedo.A, "albedo");
	SetTexture(v.AlbedoTexture, "material.albedoTexture");
}
