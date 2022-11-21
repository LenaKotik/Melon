#version 330 core
// qc

out vec4 outColor;

in vec4 ColorMul;

uniform vec4 Color;

void main()
{
	outColor = Color * ColorMul;
}