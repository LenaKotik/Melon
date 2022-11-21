#version 330 core
// qc
layout (location = 0) in vec3 Apos;
layout (location = 1) in vec4 Acolor;
layout (location = 2) in vec3 Anormal;

out vec4 ColorMul;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Normal = Anormal;
	ColorMul = Acolor;
	gl_Position = projection * view * model * vec4(Apos, 1.0);
}