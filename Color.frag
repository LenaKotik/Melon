#version 330 core

out vec4 outColor;

in vec4 ColorMul;

void main()
{
	outColor = ColorMul;
}