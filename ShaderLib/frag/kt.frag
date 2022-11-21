#version 330 core
// qc

out vec4 outColor;

in vec4 ColorMul;

uniform sampler2D Texture;

void main()
{
	outColor = texture(Texture, TexCoords) * ColorMul;
}