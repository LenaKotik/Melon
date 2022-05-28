#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glfw3.h>
#include <stb_image.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define DEBUG_OUTPUT

namespace Melon
{
	// System
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using String =	std::string;
	using DynamicFloatArray = std::vector<float>;
	using DynamicIntArray = std::vector<int>;
	using DynamicUIntArray = std::vector<unsigned int>;

	struct Color
	{
	public:
		float R, G, B, A;
		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {};
	};
	// Windowing
	struct Window
	{
		friend class Windowing;
	public:
		GLFWwindow* handle;
		bool ShouldClose();
		bool IsKeyPressed(int key);
		void MakeActive();
		void Clear(Color);
		void Flip();
		void Close();
		void Destroy();
		~Window();
	};

	class Windowing
	{
	private:
		static bool initialized;
	public:
		static Window* Init(unsigned int Width, unsigned int Height, const char* Title); // initialize the current windowng system, that includes initializing glad
		static Window* CreateWindow(unsigned int Width, unsigned int Height, const char* Title);
		static void DestroyWindow(Window* win);
		static void Terminate();
		static void PollEvents();
	};
	// Rendering
	class Texture
	{
		friend class ResourceLoader;
	private:
		GLuint handle;
	public:
		void Bind();
		void Delete();
		~Texture();
	};
	class Shader
	{
		friend class ResourceLoader;
	private:
		GLuint handle;
		bool ready;
	public:
		void Use();
		void Delete();
		~Shader();
	};
	class Renderer
	{
	private:
		bool indexed;
		GLuint VAO, VBO, EBO;
		DynamicFloatArray verticies;
		DynamicUIntArray indexies;
	public:

	};
	// Resources
	class ResourceLoader 
	{
	public:
		static Texture* LoadTexture(const char* filename);
		static Shader*  LoadShader(const char* vertFile, const char* fragFile);
	};
}