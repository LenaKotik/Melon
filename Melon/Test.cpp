#include "Melon.hpp"

int main()
{
	Melon::InitMelon();

	Melon::Window win(800U, 600U, "Tsibulya");

	Melon::Mesh m =
	{
		std::vector<GLfloat> { // verticies
		 0.0f,  0.5f, 0.0f,  
		 0.5f, -0.5f, 0.0f,  
		-0.5f, -0.5f, 0.0f,   
		},
		std::vector<GLint> { // attribs
			 3
		},
		std::vector<GLuint>{ 0, 1, 2 }, // indecies
		Melon::PrimitiveType::Triangles,
		false,
	};
	Melon::Material mat;
	mat.SetAlbedo({ 1.0f, 0.5f, 0.2f, 1.0f });
	
	Melon::Shader sh;

	char err[512];
	if (!sh.Attach("shader.vert", Melon::ShaderType::Vertex, err))
		std::cout << "ERROR COMPILING VERTEX SHADER:" << err;
	if (!sh.Attach("shader.frag", Melon::ShaderType::Fragment, err))
		std::cout << "ERROR COMPILING FRAGMENT SHADER:" << err;

	Melon::Object triangle(sh, mat, m);

	while (!win.GotCloseRequest()) // render loop
	{
		win.PollEvents();
		win.Clear(Melon::Color(0.2f, 0.3f, 0.3f, 1.0f));

		triangle.Draw();

		win.Flip();
	}

	Melon::TerminateMelon();
	return 0;
}