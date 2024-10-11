#version 430 core
// qc

out vec4 outColor;

in vec2 TexCoords;

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
  
uniform Material material;

void main()
{
    vec4 Color = vec4(0.0);
	if (material.albedoIsSolid) 
        Color = material.albedoSolid;
	else 
        Color = texture(material.albedoMap, TexCoords);
    outColor = Color;
}