#include "Melon.hpp"

Melon::Material::Material() : Albedo(1,1,1,1)
{
}

void Melon::Material::SetAlbedo(Color c)
{
	Albedo = c;
	AlbedoUseTex = false;
}
void Melon::Material::SetAlbedo(Texture t)
{
	AlbedoTexture = t;
	AlbedoUseTex = true;
}