#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glfw3.h>
#include <stb_image.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

#define DEBUG_OUTPUT

namespace Melon
{
	// Prototypes
	class Camera;
	class Renderer;
	// Abstractions
	class IDeleted // class that needs to "clean up" memory after itself
	{
	public:
		virtual void Delete()=0;
	};
	class IShader // intersection between Shader and ComputeShader, felt important, might delete later
	{
	protected:
		GLuint handle;
	};
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
	
#define DynamicArray std::vector
	using DynamicFloatArray = DynamicArray<float>;
	using DynamicIntArray = DynamicArray<int>;
	using DynamicUIntArray = DynamicArray<unsigned int>;
	using DynamicVector3Array = DynamicArray<Vector3>;

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
		static Matrix4 Ortho(float aspect, float near, float far);
	};

	struct Color
	{
	public:
		float R, G, B, A;
		Color() : R(1), G(1), B(1), A(1) {};
		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}; 
		static Color FromBytes(GLubyte r, GLubyte g, GLubyte b, GLubyte a); // converts colors from [0-255] range to [0.0f-1.0f] range

	};
	struct Vertex
	{
	public:
		Vector3 Position;
		Color Color_;
		Vector2 TextureCoords;
		Vector3 Normal;
		Vertex(Vector3 p, Color c, Vector2 st) : Position(p), Color_(c), TextureCoords(st), Normal(0) {};
		Vertex(Vector3 p, Color c, Vector2 st, Vector3 n) : Position(p), Color_(c), TextureCoords(st), Normal(n) {};
	};
	using DynamicVertexArray = DynamicArray<Vertex>;

	// Algorithms
	class ComputeShader : IShader, IDeleted
	{
		// not implemented yet
	};

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
		Vector2 GetSize();
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
		static float lastLimitedFrame;
	public:
		static float MaxFrameRate;
		static bool FrameRateLimitSatisfied();
		static float GetTime(); // time since start (seconds)
		static float GetDelta(); // time since last call (seconds)
	};
	// Rendering1
	class Texture : IDeleted
	{
		friend class ResourceLoader;
		friend class TextureUnitManager;
	private:
		GLuint handle;
	public:
		static Texture Default();
		void Bind();
		void Delete() override;
	};
	class TextureUnitManager
	{
		friend class Windowing;
	private:
		static GLuint* units;
		static GLbyte cur;
		static GLint MaxUnits;
	public:
		static GLint GetMaxTextureUnits();
		static GLbyte Add(Texture t);
		static void Clear();
	};
	struct Brush;
	class Material;
	class Shader : IShader, IDeleted
	{
		friend class ResourceLoader;
	private:
		bool ready;
	public:
		void Use();
		void Delete() override;
		~Shader();
		void SetFloat(float v, const char* name);
		void SetInt(int v, const char* name);
		void SetBool(bool v, const char* name);
		void SetVector2(Vector2 v, const char* name);
		void SetVector3(Vector3 v, const char* name);
		void SetMatrix4(Matrix4 v, const char* name);
		void SetColor(Color v, const char* name);
		void SetTexture(Texture t, const char* name);
		void SetBrush(Melon::Brush b, const char* name);
		void SetMaterial(Melon::Material m, const char* name);
	};

	struct Mesh
	{
	public:
		Mesh(DynamicVertexArray vert, GLenum pt) : verticies(vert), PrimitiveType(pt), indecies({ 0 }), is_indexed(false) {};
		Mesh(DynamicVertexArray vert, GLenum pt, DynamicUIntArray ind, bool i) : verticies(vert), PrimitiveType(pt), indecies(ind), is_indexed(i) {};
		void SetColor(Color c);
		void ComputeNormals(Vector3 center);
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
			static Mesh Sphere(unsigned int Haccuracy, unsigned int Vaccuracy); // looks sus
			static Mesh Circle(unsigned int accuracy); // 2D
		};
	}
	struct Brush // class that represents a union of fragment shadering methods
	{
	public:
		bool isSolid;
		Color Solid;
		Texture Mapped;
		Brush() : Brush(Color()) {} // as white solid
		Brush(Color c) : Solid(c), isSolid(true), Mapped(Texture::Default()){}
		Brush(Texture t) : Solid(), isSolid(false), Mapped(t) {}
	};
	class Material : IDeleted
	{
	public:
		Brush Albedo,Diffuse,Specular;
		float Shininess;
		Material() {} // all set to white solid
		void Delete() override;
	};
	class Renderer : IDeleted
	{
		enum VertexAttributesConfig;
	private:
		bool indexed;
		GLenum PrimitiveType;
		GLuint VAO, VBO, EBO;
		int indC, vertC;
		DynamicFloatArray GenBuffer(DynamicVertexArray arr, VertexAttributesConfig bitmask, int* stride, DynamicUIntArray* offsets, DynamicUIntArray* sizes);
	public:
		enum VertexAttributesConfig
		{
			Position3D = 1,
			TextureCoords = 2,
			Color = 4,
			Normal = 8,
		};
		Renderer(Mesh* m, VertexAttributesConfig);
		void Draw();
		void Delete() override;
		~Renderer();
	};
	namespace Helpers
	{
		struct ShaderLoadOptions
		{
		public:
			GLuint Attributes; // can't be an enum for some reason
			bool UseQuaternionRotation;
			bool UseLighting;
			ShaderLoadOptions(Renderer::VertexAttributesConfig attr, bool useQuat, bool useLight) :
				Attributes((GLuint)attr), UseQuaternionRotation(useQuat), UseLighting(useLight) {}
		};
		class ShaderLib
		{
		public:
			static ComputeShader* LoadCompute(String shadername); // load a specific compute shader
			static Shader* LoadBasic(ShaderLoadOptions options); // load a basic pipeline by option
			static Shader* LoadBasic(String shadername); // load a specific basic pipeline
			static Shader* LoadGeom(String shadername); // load a specific geometry pipeline
		};
	}
	// Engine
	class Camera
	{
	public:
		virtual Matrix4 GetView() = 0;
	};
	class PhysicsBody
	{
	public:
		virtual void Update(float delta) = 0;
	};
	class Collider
	{

	};
	class CollisionSolver {};
	// Enigne 2D
	// Rendering
	class Camera2D : public Camera
	{
	public:
		Vector2 Position;
		float Rotation;
		float Zoom;
		Camera2D() : Position(0.0f), Rotation(0.0f) {};
		Matrix4 GetView();
	};
	class RenderedObject2D : IDeleted
	{
	protected:
		void BeginDraw(Window* win);
	public:
		Shader Shader_;
		Renderer Renderer_;
		Material Material_;
		Vector2 Position;
		Vector2 Scale;
		float Rotation;
		RenderedObject2D(Shader* sh, Mesh m, Renderer::VertexAttributesConfig a) : Shader_(*sh), Renderer_(&m, a), Position(0.0f), Rotation(0.0f), Scale(1.0f) {};
		virtual void Delete();
		virtual void Draw(Window* win);
	};
	namespace Helpers
	{
		class Objects2D
		{
		public:
			static RenderedObject2D* Shape(Mesh m);
			static RenderedObject2D* Sprite();
		};
	}
	// Physics
	class Collider2D : public Collider
	{
	};
	class CircleCollider2D : public Collider2D
	{
	public:
		float Radius;
		CircleCollider2D() : Radius(1.0f) {};
		CircleCollider2D(float r) : Radius(r) {};
	};
	class PhysicsBody2D : public PhysicsBody
	{
	public:
		Vector2 Position;
		Collider2D* Collider_;
		PhysicsBody2D() {};
		PhysicsBody2D(Collider2D& coll) : Collider_(&coll) {};
	};
	class KinematicBody2D : public PhysicsBody2D
	{
	public:
		Vector2 PositionLast;
		Vector2 Acceleration;
		KinematicBody2D() {};
		KinematicBody2D(Collider2D& coll) : PhysicsBody2D(coll) {};
		void Update(float delta);
	};
	class CollisionSolver2D : public CollisionSolver
	{
	public:
		void Solve(CircleCollider2D& A, CircleCollider2D& B, Vector2*Ap, Vector2* Bp);
	};
	// Engine 3D
	// Rendering
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
	class RenderedObject3D : IDeleted
	{
	protected:
		void BeginDraw(Window* win);
	public:
		Renderer Renderer_;
		Shader Shader_;
		Material Material_;
		Vector3 Position;
		Vector3 Scale;
		Vector3 Rotation;
		RenderedObject3D(Shader* sh, Mesh m, Renderer::VertexAttributesConfig a) : Shader_(*sh), Renderer_(&m, a), Position(0.0f), Rotation(0.0f), Scale(1.0f) {};
		void Delete() override;
		virtual void Draw(Window* win);
	};
	namespace Helpers
	{
		class Objects3D
		{
		public:
			static RenderedObject3D* Shape(Mesh m);
			static RenderedObject3D* TexturedShape(Mesh m);
		};
	}
	// Resources
	class ResourceLoader 
	{
	public:
		static Texture* LoadTexture(const char* filename);
		static Shader*  LoadShader(const char* vertFile, const char* fragFile);
		static Shader*  LoadShader(const char* vertFile, const char* fragFile, const char* geomFile);
	};
}
