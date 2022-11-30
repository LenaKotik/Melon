#version 330 core
// qc
layout (location = 0) in vec3 Apos;

out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(Apos, 1.0);
}