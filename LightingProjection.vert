#version 330 core

layout (location = 0) in vec3 Apos;
layout (location = 1) in vec4 Acolor;
layout (location = 2) in vec3 Anormal;
layout (location = 3) in vec2 AtexCoords;

out vec2 TexCoords;
out vec4 ColorMul;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = AtexCoords;
	Normal = Anormal;
	FragPos = vec3(model * vec4(Apos, 1.0));
	ColorMul = Acolor;
	gl_Position = projection * view * model * vec4(Apos, 1.0);
}