#include "Melon.hpp"

Melon::Shader* Melon::Helpers::ShaderLib::LoadBasic(ShaderLoadOptions options)
{
	String filename = "";
	char attr_key = ((char)(options.Attributes)) + 100;
	filename = attr_key;
	if (options.UseLighting) filename += "L";
	return LoadBasic(filename);
}

Melon::Shader* Melon::Helpers::ShaderLib::LoadBasic(String shadername)
{
	Shader* res = new Shader();
	if (!ResourceLoader::LoadShader(res,
		("ShaderLib/vert/" + shadername + ".vert").c_str(), 
		("ShaderLib/frag/" + shadername + ".frag").c_str())) return nullptr;
	return res;
}

Melon::Shader* Melon::Helpers::ShaderLib::LoadGeom(String shadername)
{
	Shader* res = new Shader();
	if (!ResourceLoader::LoadShader(res,
		("ShaderLib/vert/" + shadername + ".vert").c_str(),
		("ShaderLib/frag/" + shadername + ".frag").c_str(),
		("ShaderLib/geom/" + shadername + ".geom").c_str())) return nullptr;
	return res;
}

#define MATERIAL(am,aR,aG,aB,dR,dG,dB,sR,sG,sB,sh) \
{	Material res;res.Ambient = am;	res.Albedo = Color(aR/res.Ambient,aG/res.Ambient,aB/res.Ambient,1); \
	res.Diffuse = Color(dR,dG,dB,1);	res.Specular =	Color(sR,sG,sB, 1); \
	res.Shininess = sh;	return res; }

Melon::Material Melon::Helpers::Materials::Gold()
MATERIAL(0.5,0.24725,0.1995,0.0745,0.75164,0.60648,0.22648,0.628281,0.555802,0.366065,4)