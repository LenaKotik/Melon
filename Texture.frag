#version 330 core

out vec4 outColor;

in vec2 TexCoords;
in vec4 ColorMul;

uniform sampler2D Texture;

void main()
{
	outColor = texture(Texture, TexCoords) * ColorMul;
}