#version 330 core

layout (location = 0) in vec3 Apos;
layout (location = 1) in vec2 AtexCoords;

out vec2 TexCoords;

void main()
{
	TexCoords = AtexCoords;
	gl_Position = vec4(Apos.xyz, 1.0);
}