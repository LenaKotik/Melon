#version 330 core

out vec4 outColor;

in vec3 Normal;
in vec3 FragPos;

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

uniform vec4 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform int Brightness;

const float ambientStrength = 0.1;
const float diffuseStrength = 0.5;
const float specularStrength = 0.5;


void main()
{
    vec3 dir2Light = normalize(LightPosition - FragPos);
	vec3 dir2Cam = normalize(CameraPosition - FragPos);
	vec3 reflectLight = reflect(-dir2Light, Normal);
	float diffuseK = max(0.0, dot(normalize(Normal), dir2Light)) * diffuseStrength;
	float specularK = pow(max(0.0, dot(reflectLight, dir2Cam)), pow(2, Brightness));
	vec4 Color = material.albedoSolid;
	outColor = Color * vec4((LightColor * (ambientStrength + diffuseK + specularK)).xyz, 1.0);
}