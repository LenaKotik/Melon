#pragma once

#include <iostream>

#include <glad.h>
#include <glfw3.h>

namespace Melon
{
	bool InitMelon(char* error_buffer);

	class Window
	{
	public:
		static GLFWwindow Handle;
	};
}
