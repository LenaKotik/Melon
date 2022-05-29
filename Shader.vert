#version 330 core

layout (location = 0) in vec3 Apos;
layout (location = 1) in vec4 Acolor;
layout (location = 2) in vec2 AtexCoords;

out vec2 TexCoords;
out vec4 ColorMul;

void main()
{
	TexCoords = AtexCoords;
	ColorMul = Acolor;
	gl_Position = vec4(Apos, 1.0);
}