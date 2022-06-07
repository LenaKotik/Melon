#version 330 core

out vec4 outColor;

in vec4 ColorMul;

uniform vec4 Color;
uniform vec4 LightColor;

const float ambientStrength = 0.1;

void main()
{
	outColor = Color * ColorMul * vec4((LightColor * ambientStrength).xyz, 1.0);
}