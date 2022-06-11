#version 330 core

out vec4 outColor;

in vec4 ColorGMul;

uniform vec4 Color;

void main()
{
	outColor = Color * ColorGMul;
}