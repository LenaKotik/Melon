#include "Melon.hpp"

Melon::Shader* Melon::Helpers::ShaderLib::LoadBasic(ShaderLoadOptions options)
{
	String filename = "";
	char attr_key = ((char)(options.Attributes)) + 100;
	filename = attr_key;
	if (options.UseLighting) filename += "L";
	if (options.UseQuaternionRotation) filename += 'Q';
	return LoadBasic(filename);
}

Melon::Shader* Melon::Helpers::ShaderLib::LoadBasic(String shadername)
{
	return ResourceLoader::LoadShader(
		("ShaderLib/vert/" + shadername + ".vert").c_str(), 
		("ShaderLib/frag/" + shadername + ".frag").c_str());
}

Melon::Shader* Melon::Helpers::ShaderLib::LoadGeom(String shadername)
{
	return ResourceLoader::LoadShader(
		("ShaderLib/vert/" + shadername + ".vert").c_str(), 
		("ShaderLib/frag/" + shadername + ".frag").c_str(),
		("ShaderLib/geom/" + shadername + ".geom").c_str());
}
