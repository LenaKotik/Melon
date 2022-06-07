#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glfw3.h>
#include <stb_image.h>

#define DEBUG_OUTPUT

namespace Melon
{
	// System & Math
	using String = std::string;
	const double Pi = 3.14159265358979323846;

	float deg2rad(float deg);
	float rad2deg(float rad);
	
	template <typename T>
	T min(T a, T b) { return (a < b) ? a : b; }
	
	template <typename T>
	T max(T a, T b) { return (a > b) ? a : b; }

	template <typename T>
	T clamp(T value, T minValue, T maxValue) { return max<T>(minValue, min<T>(maxValue, value)); }

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
		Matrix4(const float scal);
		Matrix4 operator=(const float v[4][4]);
		Matrix4 operator=(const Matrix4& v);
		Matrix4 operator+(const Matrix4& oth) const;
		Matrix4 operator-(const Matrix4& oth) const;
		Matrix4 operator*(const Matrix4& oth) const;
		Matrix4 operator*(const float& scalar) const;
		Matrix4 Translate(const Vector3 pos) const;
		Matrix4 Rotate(const float angle, const Vector3 axis) const;
		Matrix4 Scale(const float scalar) const;
		Matrix4 Scale(const Vector3 scalar) const;
		Matrix4 Transpose() const;
		Matrix4 Inverse() const; // DO NOT USE, NOT IMPLEMENTED
		static Matrix4 Perspective(float FOV, float aspect, float near, float far);
		static Matrix4 Ortho(float width, float height, float near, float far);
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

	class Camera;
    class Windowing;

	// Windowing
	struct Window
	{
	public:
		GLFWwindow* handle;
		Camera* MainCamera;
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
		void SetColor(Color v, const char* name);
		void SetMatrix4(Matrix4 v, const char* name);
	};
	struct Mesh
	{
	public:
		Mesh(DynamicVertexArray vert, GLenum pt, DynamicUIntArray ind, bool i) : verticies(vert), PrimitiveType(pt), indecies(ind), is_indexed(i) {};
		void SetColor(Color c);
		DynamicVertexArray verticies;
		GLenum PrimitiveType;
		DynamicUIntArray indecies;
		bool is_indexed;
	};
	namespace Helpers
	{
		class Meshes
		{
		public:
			static Mesh Quad(); // 2D
			static Mesh Triangle(); // 2D
			static Mesh Cube(); // 3D
			static Mesh Sphere(unsigned int Haccuracy, unsigned int Vaccuracy); // do not use this, bad
			static Mesh Circle(unsigned int accuracy); // 2D
		};
	}
	class Renderer
	{
	private:
		bool indexed;
		GLenum PrimitiveType;
		GLuint VAO, VBO, EBO;
		int indC, vertC;
		//DynamicFloatArray verticies;
		//DynamicUIntArray indexies;
	public:
		Renderer(Mesh* m);
		void Draw();
		void Delete();
		~Renderer();
	};
	// Engine
	class Camera
	{
	public:
		virtual Matrix4 GetView() = 0;
	};
	// Enigne 2D
	class Camera2D : public Camera
	{
	public:
		Vector2 Position;
		float Rotation;
		Camera2D() : Position(0.0f), Rotation(0.0f) {};
		Matrix4 GetView();
	};
	class RenderedObject2D
	{
	protected:
		void BeginDraw(Window* win);
	public:
		Shader Shader_;
		Renderer Renderer_;
		Vector2 Position;
		Vector2 Scale;
		float Rotation;
		RenderedObject2D(Shader* sh, Mesh m) : Shader_(*sh), Renderer_(&m), Position(0.0f), Rotation(0.0f), Scale(1.0f) {};
		void Delete();
		virtual void Draw(Window* win) = 0;
	};
	class Shape2D : public RenderedObject2D
	{
	public:
		Color Color_;
		Shape2D(Mesh m);
		void Draw(Window* win);
		~Shape2D();
	};
	class Sprite2D : public RenderedObject2D
	{
	public:
		Texture Texture_;
		Sprite2D(Texture* tex);
		void Draw(Window* win);
		~Sprite2D();
	};
	// Engine 3D
	class Camera3D : public Camera
	{
	public:
		float FOV;
		Vector3 Position;
		Vector3 Direction;
		Vector3 Up;
		Camera3D() : Position(0.0f), Direction(0.0f, 0.0f, -1.0f),
			Up(0.0f, 1.0f, 0.0f), FOV(45.0f) {};
		Matrix4 GetView();
	};
	class RenderedObject3D
	{
	protected:
		void BeginDraw(Window* win);
	public:
		Renderer Renderer_;
		Shader Shader_;
		Vector3 Position;
		Vector3 Scale;
		Vector3 Rotation;
		RenderedObject3D(Shader* sh, Mesh m) : Shader_(*sh), Renderer_(&m), Position(0.0f), Rotation(0.0f), Scale(1.0f) {};
		void Delete();
		virtual void Draw(Window* win) = 0;
	};
	class Shape3D : public RenderedObject3D
	{
	public:
		Color Color_;
		Shape3D(Mesh m);
		void Draw(Window* win);
		~Shape3D();
	};
	class TexturedMesh : public RenderedObject3D
	{
	public:
		Texture Texture_;
		TexturedMesh(Texture* tex, Mesh m);
		void Draw(Window* win);
		~TexturedMesh();
	};
	// Resources
	class ResourceLoader 
	{
	public:
		static Texture* LoadTexture(const char* filename);
		static Shader*  LoadShader(const char* vertFile, const char* fragFile);
	};
}
