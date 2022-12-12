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

bool Melon::ResourceLoader::flipYTextures = true;

bool Melon::ResourceLoader::LoadTextureData(TextureData* result, const char* filename)
{
	stbi_set_flip_vertically_on_load(flipYTextures);

	result->data = stbi_load(filename, &result->width, &result->height, &result->channels, 0);

	if (!result->data)
	{
		fprintf(stderr, "failed to load texture: %s", filename);
		return false;
	}
	return true;
}


bool Melon::ResourceLoader::LoadShader(Melon::Shader* result, const char* vertFile, const char* fragFile)
{
	Melon::String vertSrc = readFile(vertFile);
	Melon::String fragSrc = readFile(fragFile);
	const char* vertSrcC = vertSrc.c_str();
	const char* fragSrcC = fragSrc.c_str();

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
		return false;
	}
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, log);
		fprintf(stderr, "FRAGMENT SHADER COMPILATION FAILED:\n %s", log);
		return false;
	}
	result->handle = glCreateProgram();
	glAttachShader(result->handle, vertex);
	glAttachShader(result->handle, fragment);
	glLinkProgram(result->handle);
	glGetProgramiv(result->handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(fragment, 512, NULL, log);
		fprintf(stderr, "SHADER PROGRAM LINKING FAILED:\n %s", log);
		return false;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return true;
}

bool Melon::ResourceLoader::LoadShader(Melon::Shader* result, const char* vertFile, const char* fragFile, const char* geomFile)
{
	Melon::String vertSrc = readFile(vertFile);
	Melon::String fragSrc = readFile(fragFile);
	Melon::String geomSrc = readFile(geomFile);

	const char* vertSrcC = vertSrc.c_str();
	const char* fragSrcC = fragSrc.c_str();
	const char* geomSrcC = geomSrc.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(vertex, 1, &vertSrcC, NULL);
	glShaderSource(fragment, 1, &fragSrcC, NULL);
	glShaderSource(geometry, 1, &geomSrcC, NULL);

	glCompileShader(vertex);
	glCompileShader(fragment);
	glCompileShader(geometry);

	GLint success;
	char log[512];

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, log);
		fprintf(stderr, "VERTEX SHADER COMPILATION FAILED:\n %s", log);
		return false;
	}
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, log);
		fprintf(stderr, "FRAGMENT SHADER COMPILATION FAILED:\n %s", log);
		return false;
	}
	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geometry, 512, NULL, log);
		fprintf(stderr, "GEOMETRY SHADER COMPILATION FAILED:\n %s", log);
		return false;
	}
	result->handle = glCreateProgram();
	glAttachShader(result->handle, vertex);
	glAttachShader(result->handle, fragment);
	glAttachShader(result->handle, geometry);
	glLinkProgram(result->handle);
	glGetProgramiv(result->handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(fragment, 512, NULL, log);
		fprintf(stderr, "SHADER PROGRAM LINKING FAILED:\n %s", log);
		return false;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
	return true;
}
bool Melon::ResourceLoader::LoadAudio(AudioBuffer* result, const char* filename) 
{
	AudioHeaderData meta;
	char* data = nullptr;
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) return false;

	LoadWav_(&file, &meta, data); // TODO: implement more file formats
	
	file.close();
	// the first bit is BPS (8=0;16=1) second bit is channel (1=0;2=1) 
	ALenum format = AL_FORMAT_MONO8 + (meta.BitsPerSample == 16) + 2 * (meta.Channels == 2);
	alGenBuffers(1, &result->handle);
	alBufferData(result->handle, format, (void*)data, meta.Size, meta.SampleRate);

	delete data;
	return true;
}
