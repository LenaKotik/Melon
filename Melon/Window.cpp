#include "Melon.hpp"

static void window_resize(GLFWwindow* win, GLint w, GLint h)
{
	glfwMakeContextCurrent(win);
	glViewport(0, 0, w, h);
}

Melon::Window::Window(GLuint Width, GLuint Height, const char* Title)
{
	_handle = glfwCreateWindow(Width, Height, Title, NULL, NULL/* other window to share resources with, maybe implement later*/);
	glfwSetWindowSizeCallback(_handle, window_resize);
	if (_handle == NULL)
		throw std::exception("Could not create a window");
	glfwMakeContextCurrent(_handle);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::exception("Couldn't initialize GLAD");
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, Width, Height);
}
Melon::Window::~Window()
{
	glfwDestroyWindow(_handle);
}
void Melon::Window::Clear(Color c)
{
	ActivateContext();
	glClearColor(c.R, c.G, c.B, c.A);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Melon::Window::Flip()
{
	glfwSwapBuffers(_handle);
}
bool Melon::Window::GotCloseRequest()
{
	return glfwWindowShouldClose(_handle);
}

void Melon::Window::ActivateContext()
{
	glfwMakeContextCurrent(_handle);
}

void Melon::Window::PollEvents()
{
	glfwPollEvents();
}
