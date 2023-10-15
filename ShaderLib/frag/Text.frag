#version 330 core

out vec4 outColor;

in vec2 TexCoords;

uniform vec4 TextColor;
uniform sampler2D glyph;

void main()
{
    vec4 monochrome = vec4(1.0,1.0,1.0,texture(glyph,TexCoords).r);
    outColor = TextColor * monochrome;
}