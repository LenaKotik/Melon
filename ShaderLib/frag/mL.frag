#version 330 core

out vec4 outColor;

in vec3 Normal;
in vec3 FragPos;

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
	float ambient;
}; 
  
uniform Material material;

uniform vec4 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;

void main()
{
	vec3 ambient = LightColor.xyz * material.ambient * material.albedoSolid.xyz;

    vec3 dir2Light = normalize(LightPosition - FragPos);
	vec3 dir2Cam = normalize(CameraPosition - FragPos);
	vec3 reflectLight = reflect(-dir2Light, normalize(Normal));
	
	float diffuseK = max(0.0, dot(normalize(Normal), dir2Light));
	vec3 diffuseC = material.diffuseSolid.xyz;
	vec3 diffuse = LightColor * diffuseC * diffuseK;
	
	float specularK = pow(max(0.0, dot(reflectLight, dir2Cam)), material.shininess);
	vec3 specularC = material.specularSolid.xyz;
	vec3 specular = LightColor * specularC * specularK;
	
	outColor = vec4((ambient + diffuse + specular).xyz, 1); 
}