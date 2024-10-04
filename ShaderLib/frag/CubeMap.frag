#version 330 core

out vec4 outColor;
in vec3 TexCoords;

uniform samplerCube CubeMap;

void main()
{
    outColor = //vec4(0.1647, 0.9137, 0.3529, 0.5);
    texture(CubeMap, TexCoords);
}