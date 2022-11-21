#version 330 core
// ot
out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D Texture;

void main()
{
	outColor = texture(Texture, TexCoords);
}