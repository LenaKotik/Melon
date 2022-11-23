#version 330 core

layout (location = 0) in vec3 Apos;
layout (location = 1) in vec3 Anormal;


out vec3 Normal;

uniform mat4 model;
uniform mat4 view;

void main()
{
	gl_Position = view * model * vec4(Apos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    Normal = normalize(vec3(vec4(normalMatrix * Anormal, 0.0)));
}