#version 330 core
// qc

out vec4 outColor;

in vec4 ColorMul;

uniform vec4 Color;
uniform sampler2D Texture;

void main()
{
	outColor = texture(Texture, TexCoords) * Color * ColorMul;
}