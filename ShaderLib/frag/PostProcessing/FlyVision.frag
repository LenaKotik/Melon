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
    float N = 1.5f;
    vec2 TC = vec2(TexCoords.x*N, TexCoords.y*N);
    vec4 Color = texture(material.albedoMap, TC);
    outColor = vec4(Color.rgb*N*N,1.0f); // do stuff here
}