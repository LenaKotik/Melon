#version 330 core
// ot
out vec4 outColor;

in vec2 TexCoords;

/*
struct Material {
    vec4 albedoSolid;
	bool albedoIsSolid;
	sampler2D albedoMap;
    vec4 diffuseSolid;
    bool diffuseIsSolid;
    sampler2D diffuseMap;
    
	vec4 specularSolid;
    bool specularIsSolid;
    sampler2D specularMap;
    
	float shininess;
}; 
uniform Material material;
*/

void main()
{
	if (material.albedoIsSolid) outColor = material.albedoSolid;
	else outColor = texture(material.albedoMap, TexCoords);
}