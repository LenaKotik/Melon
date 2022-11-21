#version 330 core
// qc

out vec4 outColor;

in vec2 TexCoords;

uniform vec4 Color;

void main()
{
	outColor = Color;
}