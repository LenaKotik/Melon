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
	// System & Math
	using String = std::string;
	const double Pi = 3.14159265358979323846;

	float deg2rad(float deg);
	float rad2deg(float rad);

	struct Vector2 // for now only floats, but this should be enough
	{
	public:
		float x, y;
		Vector2() : x(0), y(0) {};
		Vector2(float X, float Y) : x(X), y(Y) {};
		Vector2(float XandY) : x(XandY), y(XandY) {};
		Vector2 operator=(const Vector2& oth);
		Vector2 operator+=(const Vector2& oth);
		Vector2 operator-=(const Vector2& oth);
		Vector2 operator*=(const float& scalar);
		Vector2 operator+(const Vector2& oth) const;
		Vector2 operator-(const Vector2& oth) const;
		Vector2 operator*(const float& scalar) const;
		Vector2 operator-() const;
		float Dot(const Vector2& oth) const;
		float Magnitude() const;
		float MagnitudeSqr() const;
		Vector2 Normalize() const;
	};
	struct Vector3 // same thing
	{
	public:
		float x, y, z;
		Vector3() : x(0), y(0), z(0) {};
		Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {};
		Vector3(float xyz) : Vector3(xyz, xyz, xyz) {};
		Vector3 operator=(const Vector3& oth);
		Vector3 operator+=(const Vector3& oth);
		Vector3 operator-=(const Vector3& oth);
		Vector3 operator*=(const float& scalar);
		Vector3 operator+(const Vector3& oth) const;
		Vector3 operator-(const Vector3& oth) const;
		Vector3 operator*(const float& scalar) const;
		Vector3 operator-() const;
		Vector3 Cross(const Vector3& oth);
		float Dot(const Vector3& oth) const;
		float Magnitude() const;
		float MagnitudeSqr() const;
		Vector3 Normalize() const;
	};
	//using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	//using Matrix4 = glm::mat4;
	
	using DynamicFloatArray = std::vector<float>;
	using DynamicIntArray = std::vector<int>;
	using DynamicUIntArray = std::vector<unsigned int>;
	using DynamicVector3Array = std::vector<Vector3>;

	struct Matrix4
	{
	public:
		float Value[4][4];
		Matrix4();
		Matrix4(float val[4][4]);
		Matrix4(float scal);
		Matrix4 operator+(Matrix4& oth) const;
		Matrix4 operator-(Matrix4& oth) const;
		Matrix4 operator*(Matrix4& oth) const;
		Matrix4 operator*(float& scalar) const;
		Matrix4 Translate(Vector3 pos) const;
		Matrix4 Rotate(float angleScal, Vector3 vec) const;
		Matrix4 Scale(float scalar) const;
		Matrix4 Transpose() const; // DO NOT USE, NOT IMPLEMENTED
		Matrix4 Inverse() const; // DO NOT USE, NOT IMPLEMENTED
		static Matrix4 Perspective(float FOV, float aspect, float near, float far);
	};

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
		static Matrix4 GetView();
	};
	// Resources
	class ResourceLoader 
	{
	public:
		static Texture* LoadTexture(const char* filename);
		static Shader*  LoadShader(const char* vertFile, const char* fragFile);
	};
}
