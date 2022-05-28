#include "Melon.hpp"

bool Melon::Windowing::initialized = false;

static void on_windowResized(GLFWwindow* win, int width, int height)
{
	glfwMakeContextCurrent(win);
	glViewport(0, 0, width, height);
}

#ifdef DEBUG_OUTPUT
static void APIENTRY debugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam)
{
	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;

}
#endif // DEBUG_OUTPUT

Melon::Window* Melon::Windowing::CreateWindow(unsigned int Width, unsigned int Height, const char* Title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG_OUTPUT
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	Melon::Window* res = new Window;
	res->handle = glfwCreateWindow(Width, Height, Title, NULL, NULL);
	if (!res->handle)
	{
		fprintf(stderr, "GLFW failed to create a window");
		return nullptr;
	}
	glfwMakeContextCurrent(res->handle);
	glfwSetFramebufferSizeCallback(res->handle, on_windowResized);

	if (initialized)
	{
		// some GL calls here
		glViewport(0, 0, Width, Height);
	}

	return res;
}

void Melon::Windowing::DestroyWindow(Window* win)
{
	delete win;
}


Melon::Window* Melon::Windowing::Init(unsigned int Width, unsigned int Height, const char* Title)
{
	glfwInit();

	Window* res = CreateWindow(Width, Height, Title);
	if (!res) return nullptr;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "GLAD failed to initialize");
		return nullptr;
	}
#ifdef DEBUG_OUTPUT
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif // DEBUG_OUTPUT

	glViewport(0, 0, Width, Height);

	initialized = true;

	return res;
}

bool Melon::Window::IsKeyPressed(int key)
{
	return (glfwGetKey(this->handle, key) == GLFW_PRESS);
}

void Melon::Window::MakeActive()
{
	glfwMakeContextCurrent(this->handle);
}

bool Melon::Window::ShouldClose()
{
	return glfwWindowShouldClose(this->handle);
}

void Melon::Window::Close()
{
	glfwSetWindowShouldClose(this->handle, true);
}

void Melon::Window::Destroy()
{
	this->Close();
	printf("Window go bye bye\n");
	glfwDestroyWindow(this->handle);
}

Melon::Window::~Window()
{
	this->Destroy();
}

void Melon::Window::Flip()
{
	glfwSwapBuffers(this->handle);
}

void Melon::Window::Clear(Color c)
{
	glClearColor(c.R, c.G, c.B, c.A);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Melon::Windowing::PollEvents()
{
	glfwPollEvents();
}

void Melon::Windowing::Terminate()
{
	glfwTerminate();
}
