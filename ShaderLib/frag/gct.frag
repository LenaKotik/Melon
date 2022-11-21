#version 330 core
// qc

out vec4 outColor;

in vec2 TexCoords;

uniform vec4 Color;
uniform sampler2D Texture;

void main()
{
	outColor = Color * texture(Texture, TexCoords);
}