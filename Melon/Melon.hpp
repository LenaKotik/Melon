#pragma once

#include <iostream>
#include <fstream>

#include <vector>

#include <stdexcept>

#include <glad/glad.h>

#include <glfw3.h>

namespace Melon
{
	/// <summary>
	/// Initializes the library and it's dependencies
	/// </summary>
	void InitMelon();
	
	/// <summary>
	/// Terminates the library, and all of it's dependencies, also frees all allocated resources
	/// </summary>
	void TerminateMelon();

	/// <summary>
	/// Represents a 4-componet color
	/// </summary>
	struct Color
	{
		Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		GLfloat R,G,B,A;
	};

	/// <summary>
	/// A basic window for rendering, has an OpenGL context attached
	/// </summary>
	class Window
	{
	public:
		/// <summary>
		/// Clear the framebuffer with a color, also clears the depth buffer
		/// </summary>
		/// <param name="c">Color to clear with</param>
		void Clear(Color c);

		/// <summary>
		/// Flips the framebuffer, displaying it's contents on screen
		/// </summary>
		void Flip();

		/// <returns>Close Flag of the window</returns>
		bool GotCloseRequest();

		/// <summary>
		/// Makes the window the current active OpenGL context on the caller thread.
		/// Note that you don't have to call this method right after creating a window,
		/// because it's already activated in the constructor
		/// </summary>
		void ActivateContext();

		/// <summary>
		/// Processes events and calls event-callback functions
		/// </summary>
		void PollEvents();

		/// <summary>
		/// Creates a new window and activates it's context
		/// </summary>
		/// <param name="Width">Width of the window</param>
		/// <param name="Height">Height of the window</param>
		/// <param name="Title">Window title</param>
		Window(GLuint Width, GLuint Height, const char* Title);
		~Window();

		/// <summary>
		/// GLFW window handler, and the context [internal]
		/// </summary>
		GLFWwindow *_handle;
	};
	enum class PrimitiveType
	{
		Triangles =			GL_TRIANGLES,
		TrainglesRad =		GL_TRIANGLE_FAN,
		Lines =				GL_LINES,
		LineStrip =			GL_LINE_STRIP,
		Points =			GL_POINTS,
	};
	/// <summary>
	/// incapsulates all properties of an object needed to define it's shape
	/// </summary>
	struct Mesh
	{
		/// <summary>
		/// Attribute data for each of the mesh'es verticies
		/// </summary>
		std::vector<GLfloat> verticies;
		/// <summary>
		/// Size of each per-vertex attribute
		/// </summary>
		std::vector<GLint> attribs;
		/// <summary>
		/// Order of verticies for indexed drawing
		/// </summary>
		std::vector<GLuint> indecies;
		/// <summary>
		/// Type of the primitive to draw
		/// </summary>
		PrimitiveType type = PrimitiveType::Triangles;
		/// <summary>
		/// Defines if drawing is indexed or not
		/// </summary>
		bool isIndexed;
	};
	/// <summary>
	/// Represents a texture, can be used for rendering objects
	/// with sampling shaders
	/// </summary>
	class Texture
	{
	friend class Shader;
	public:
		Texture();
		Texture(const char* file);
		~Texture();
		void LoadFromFile(const char *file);
		void LoadFromMemory(char* data);
		GLfloat Width, Height;
		/// <summary>
		/// amount of color channels the texture has.
		/// Ranges from 1 to 4
		/// </summary>
		GLuint Channels;
	private:
		/// <summary>
		/// the OpenGL texture unit, which this texture uses [internal]
		/// </summary>
		GLuint _unit;
	};
	/// <summary>
	/// incapsulates all properties of an object needed to define it's color
	/// </summary>
	struct Material
	{
		Material();
		/// <summary>
		/// The material's color
		/// </summary>
		Color Albedo;
		/// <summary>
		/// The material's color, individual on per-texel basis
		/// </summary>
		Texture AlbedoTexture;
		/// <summary>
		/// If set to true, shader will sample albedo color from the texture,
		/// otherwise - from the variable
		/// false by default
		/// </summary>
		bool AlbedoUseTex = false;
		/// <summary>
		/// Set object's albedo as a single color
		/// Note: this method also sets AlbedoUseTex
		/// </summary>
		/// <param name="color">color to use</param>
		void SetAlbedo(Color color);
		/// <summary>
		/// Set object's albedo as a texture
		/// Note: this method also sets AlbedoUseTex
		/// </summary>
		/// <param name="texture">texture to sample color from</param>
		void SetAlbedo(Texture texture);
	};
	enum class ShaderType
	{
		Fragment =		GL_FRAGMENT_SHADER,
		Vertex =		GL_VERTEX_SHADER,
		Geometry =		GL_GEOMETRY_SHADER,
	};
	class Shader
	{
		friend class Object;
	private:
		GLuint _ptr;
		std::vector<GLuint> _shdrs;
	public:
		Shader();
		~Shader();
		/// <summary>
		/// reads shader's source code from the file, compiles it,
		/// and attaches to the shader program
		/// </summary>
		/// <param name="file">path to the file with shader source</param>
		/// <param name="type">type of shader</param>
		/// <param name="err">error log buffer</param>
		/// <returns>true on success, otherwise - false</returns>
		bool Attach(const char* file, ShaderType type, char* err);

		/// <summary>
		/// compiles shader's source code,
		/// and attaches it to the shader program
		/// </summary>
		/// <param name="source">the source code of the shader</param>
		/// <param name="type">type of shader</param>
		/// <param name="err">error log buffer</param>
		/// <returns>true on success, otherwise - false</returns>
		bool AttachFromSource(const char* source, ShaderType type, char* err);

		/// <summary>
		/// Links together attached shaders
		/// </summary>
		/// <param name="err">error log buffer</param>
		/// <returns>true on succes, otherwise - false</returns>
		bool Link(char* err);

		// uniforms:

		void SetBool(bool v, const char* name);
		void SetInt(GLint v, const char* name);
		void SetFloat(GLfloat v, const char* name);
		void SetVec4(GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw, const char* name);
		void SetVec3(GLfloat vx, GLfloat vy, GLfloat vz, const char* name);
		void SetTexture(Texture &v, const char* name);
		void SetMaterial(Material v);
	};
	class Object
	{
	private:
		GLuint _vao, _vbo, _ebo;
		GLint stride;
	public:
		Object(Shader, Material, Mesh);
		~Object();
		/// <summary>
		/// Shader program used to render the object
		/// </summary>
		Shader shader;
		/// <summary>
		/// Material, that contains color and lighting properties for the object
		/// </summary>
		Material material;
		/// <summary>
		/// Mesh, that contains vertex data for the object
		/// </summary>
		Mesh mesh;

		/// <summary>
		/// Renders the object on the current active context
		/// </summary>
		void Draw();
	};
}
