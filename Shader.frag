#version 330 core

out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D Texture;

void main()
{
	outColor = texture(Texture, TexCoords);
	//if (outColor.a == 0) discard;
}