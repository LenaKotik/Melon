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
