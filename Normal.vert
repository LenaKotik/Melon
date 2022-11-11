#version 330 core

layout (location = 0) in vec3 Apos;
layout (location = 1) in vec4 Acolor;
layout (location = 2) in vec3 Anormal;
layout (location = 3) in vec2 AtexCoords;

out vec2 TexCoords;
out vec4 ColorMul;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;

void main()
{
	TexCoords = AtexCoords;
	ColorMul = Acolor;
	gl_Position = view * model * vec4(Apos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    Normal = normalize(vec3(vec4(normalMatrix * Anormal, 0.0)));
}