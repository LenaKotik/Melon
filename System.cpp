#include "Melon.hpp"

Melon::Color Melon::Color::FromBytes(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	return Color((float)(r) / 255.0f, (float)(g) / 255.0f, (float)(b) / 255.0f, (float)(a) / 255.0f);
}