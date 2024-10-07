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
    vec4 Color = texture(material.albedoMap, TexCoords);
    float avg = 0.2126 * Color.r + 0.7152 * Color.g + 0.0722 * Color.b;
    outColor = vec4(avg, avg, avg, 1.0f);
}