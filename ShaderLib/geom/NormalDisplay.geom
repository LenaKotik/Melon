#version 330 core

layout (points) in;
layout (line_strip, max_vertices=2) out;

in vec3 Normal[];

uniform mat4 projection;

uniform float length_;

void main()
{
    gl_Position = projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(Normal[0], 0.0) * length_);
    EmitVertex();
    EndPrimitive();
}