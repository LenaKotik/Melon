#version 330 core

out vec4 outColor;

struct Material {
    vec4 albedoSolid;
	bool albedoIsSolid;
	sampler2D albedoMap;
/*
    vec4 diffuseSolid;
    bool diffuseIsSolid;
    sampler2D diffuseMap;
    
	vec4 specularSolid;
    bool specularIsSolid;
    sampler2D specularMap;
    
	float shininess;
*/
}; 
uniform vec4 LineColor;
uniform Material material;

void main()
{
	outColor = material.albedoSolid * LineColor;
}