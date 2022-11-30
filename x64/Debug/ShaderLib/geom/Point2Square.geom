#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

uniform float radius;

void main()
{
    gl_Position = gl_in[0].gl_Position + vec4(-radius, -radius, 0.0, 0.0);    // 1:bottom-left   
    EmitVertex();   
    gl_Position = gl_in[0].gl_Position + vec4( radius, -radius, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-radius,  radius, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4( radius,  radius, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    EndPrimitive();
}