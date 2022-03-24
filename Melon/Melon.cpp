#include "Melon.hpp"


void Melon::InitMelon()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Melon::TerminateMelon()
{
	glfwTerminate();
}

Melon::Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) : R(r), G(g), B(b), A(a) {}