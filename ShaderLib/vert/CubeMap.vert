#version 330 core

layout (location = 0) in vec3 Apos;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = Apos*2;
    gl_Position = projection * view * vec4(Apos, 1.0);
}