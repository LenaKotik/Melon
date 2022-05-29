#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glfw3.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define DEBUG_OUTPUT

namespace Melon
{
	// System
	using String = std::string;
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Matrix4 = glm::mat4;

	template <typename T>
	class DynamicArray
	{
	private:
		T* data;
		size_t size;
		size_t capacity;
	public:
		DynamicArray(size_t initial_size);
		size_t step;
		size_t Size();
		T* Data();
		void Shrink();
		void PushBack(T&&);
		T PopBack();
		~DynamicArray();
	};
	using DynamicFloatArray = std::vector<float>;
	using DynamicIntArray = std::vector<int>;
	using DynamicUIntArray = std::vector<unsigned int>;
	using DynamicVector3Array = std::vector<Vector3>;

	struct Color
	{
	public:
		float R, G, B, A;
		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}; 
		static Color FromBytes(GLubyte r, GLubyte g, GLubyte b, GLubyte a); // converts colors from [0-255] range to [0.0f-1.0f] range

	};
	struct Vertex
	{
	public:
		Vector3 Position;
		Color Color_;
		Vector2 TextureCoords;
		Vertex(Vector3 p, Color c, Vector2 st) : Position(p), Color_(c), TextureCoords(st) {};
	};
	using DynamicVertexArray = std::vector<Vertex>;
	// Math
	const double Pi = 3.14159265358979323846;

	float deg2rad(float deg);
	float rad2deg(float rad);


	// Windowing
	struct Window
	{
		friend class Windowing;
	public:
		GLFWwindow* handle;
		bool ShouldClose();
		bool IsKeyPressed(int key);
		void MakeActive();
		void SetCursor(bool);
		Vector2 GetMousePosition();
		float GetAspect();
		void Clear(Color, bool depth);
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
		static Window* Init(unsigned int Width, unsigned int Height, const char* Title, bool depth); // initialize the current windowng system, that includes initializing glad
		static Window* CreateWindow(unsigned int Width, unsigned int Height, const char* Title);
		static void DestroyWindow(Window* win);
		static void Terminate();
		static void PollEvents();
	};
	class Time
	{
	private:
		static float lastDeltaCall;
	public:
		static float GetTime(); // time since start (seconds)
		static float GetDelta(); // time since last call (seconds)
	};
	// Rendering1
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
		void SetFloat(float v, const char* name);
		void SetInt(int v, const char* name);
		void SetBool(bool v, const char* name);
		void SetVector2(Vector2 v, const char* name);
		void SetVector3(Vector3 v, const char* name);
		void SetMatrix4(Matrix4 v, const char* name);
	};
	struct Mesh
	{
	public:
		Mesh(DynamicVertexArray vert, DynamicUIntArray ind, bool i) : verticies(vert), indecies(ind), is_indexed(i) {};
		void SetColor(Color c);
		DynamicVertexArray verticies;
		DynamicUIntArray indecies;
		bool is_indexed;
	};
	namespace Helpers
	{
		class Meshes
		{
		public:
			static Mesh Quad();
			static Mesh Triangle();
			static Mesh Cube();
		};
	}
	class Renderer
	{
	private:
		bool indexed;
		GLuint VAO, VBO, EBO;
		int indC, vertC;
		//DynamicFloatArray verticies;
		//DynamicUIntArray indexies;
	public:
		Renderer(Mesh* m);
		void Draw(GLenum mode);
		void Delete();
		~Renderer();
	};
	// Rendering2
	class Camera
	{
	public:
		static Vector3 Position;
		static Vector3 Direction;
		static Vector3 Up;
	};
	// Resources
	class ResourceLoader 
	{
	public:
		static Texture* LoadTexture(const char* filename);
		static Shader*  LoadShader(const char* vertFile, const char* fragFile);
	};
}