#version 330 core
// mcL
layout (location = 0) in vec3 Apos;
layout (location = 1) in vec3 Anormal;

out vec4 ColorMul;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Normal = Anormal;
	FragPos = vec3(model * vec4(Apos, 1.0));
	gl_Position = projection * view * model * vec4(Apos, 1.0);
}