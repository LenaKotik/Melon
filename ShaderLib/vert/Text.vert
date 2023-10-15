#version 330 core

layout (location=0) in vec3 Apos;
layout (location=1) in vec2 AtexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = AtexCoords;
    gl_Position = projection * model * vec4(Apos,1.0);
}