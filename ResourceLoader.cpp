#include "Melon.hpp"

static Melon::String readFile(const char* file)
{
	std::ifstream f(file);
	Melon::String s;
	Melon::String res;
	while (std::getline(f, s))
	{
		res += s + "\n";
	}
	return res;
}

Melon::Texture* Melon::ResourceLoader::LoadTexture(const char* filename)
{
	const GLenum color_spaces[] = { NULL, GL_RED, GL_RG, GL_RGB, GL_RGBA };

	GLint width, height, channels;

	GLubyte* img_mem = stbi_load(filename, &width, &height, &channels, 0);

	if (!img_mem)
	{
		fprintf(stderr, "failed to load texture: %s", filename);
		return nullptr;
	}

	GLenum color_space = color_spaces[channels];

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Texture* res = new Texture;

	glGenTextures(1, &res->handle);

	glBindTexture(GL_TEXTURE_2D, res->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, color_space, width, height, 0, color_space, GL_UNSIGNED_BYTE, img_mem);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(img_mem);

	return res;
}


Melon::Shader* Melon::ResourceLoader::LoadShader(const char* vertFile, const char* fragFile)
{
	Melon::String vertSrc = readFile(vertFile);
	Melon::String fragSrc = readFile(fragFile);
	const char* vertSrcC = vertSrc.c_str();
	const char* fragSrcC = fragSrc.c_str();

	Shader* shdr = new Shader;
	shdr->ready = false;

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex, 1, &vertSrcC, NULL);
	glShaderSource(fragment, 1, &fragSrcC, NULL);

	glCompileShader(vertex);
	glCompileShader(fragment);

	GLint success;
	char log[512];

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, log);
		fprintf(stderr, "VERTEX SHADER COMPILATION FAILED:\n %s", log);
		delete shdr;
		return nullptr;
	}
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, log);
		fprintf(stderr, "FRAGMENT SHADER COMPILATION FAILED:\n %s", log);
		delete shdr;
		return nullptr;
	}
	shdr->handle = glCreateProgram();
	glAttachShader(shdr->handle, vertex);
	glAttachShader(shdr->handle, fragment);
	glLinkProgram(shdr->handle);
	glGetProgramiv(shdr->handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(fragment, 512, NULL, log);
		fprintf(stderr, "SHADER PROGRAM LINKING FAILED:\n %s", log);
		delete shdr;
		return nullptr;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	shdr->ready = true;
	return shdr;
}
