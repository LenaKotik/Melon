#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform float TIME;

uniform struct Material
{
	vec4 albedo;
	bool albedoUseTex;
	sampler2D albedoTexture;
} material;

void main()
{
	FragColor = material.albedoUseTex? texture(material.albedoTexture, TexCoords) : material.albedo;
}