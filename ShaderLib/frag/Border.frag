#version 330 core

in vec2 TexCoords;

out vec4 outColor;

uniform vec4 Color;
uniform vec4 BorderColor;
uniform float BorderWidth;

void main()
{
   float max_ = 1.0 - BorderWidth;
   float min_ = BorderWidth;
   if (TexCoords.x < max_ && TexCoords.x > min_ && TexCoords.y < max_ && TexCoords.y > min_)
   {
		outColor = Color;
   }
	else
		outColor = BorderColor;
}