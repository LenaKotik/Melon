#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <glad/glad.h>
#include <glfw3.h>
#include <stb_image.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h> 
#include <AL/al.h>
#include <AL/alc.h>
#include <ft2build.h>
#include FT_FREETYPE_H // sus

#define DEBUG_OUTPUT
typedef std::uint8_t Byte;
typedef unsigned long long Size_t;
namespace Melon
{
	// Prototypes
	class Camera;
	class Renderer;
	struct Window;
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
	class Stream
	{
	public:
		enum SeekPosition
		{
			Beginning = std::ios::_Seekbeg,
			Current = std::ios::_Seekcur,
			End = std::ios::_Seekend,
		};
		virtual bool Seek(Size_t offset, SeekPosition from)=0;
		virtual Size_t Tell()=0;
		virtual Size_t Size()=0;
	};
	class InputStream : Stream
	{
	public:
		virtual bool Read(Byte* destination, Size_t bytes_to_read) = 0;
	};
	class OutputStream : Stream
	{
	public:
		virtual bool Write(Byte* source, Size_t bytes_to_write) = 0;
	};
	
	template <typename T,Size_t N>
	class FixedArray
	{
	public:
		T Data[N];
		constexpr Size_t Size() const { return N; }
		constexpr Size_t ByteSize() const { return N * sizeof(T); }
		T& operator [](Size_t index) { return Data[index]; }
		const T& operator [](Size_t index) const { return Data[index]; }
		T* begin() const { return Data; }
		T* end() const { return Data + N; }
	};
	
	template<Size_t N>
	using FixedUintArray = FixedArray<unsigned int, N>;
	template<Size_t N>
	using FixedIntArray = FixedArray<int, N>;
	template<Size_t N>
	using FixedFloatArray = FixedArray<float, N>;
	
	template <typename T>
	class DynamicArray
	{
	private:
		Size_t actual_size;
		Size_t size;
	public:
		T* Data;
		/// <returns>amount of actual elements</returns>
		Size_t Size() const;
		/// <returns>ocupied size in bytes</returns>
		Size_t ByteSize() const;
		bool Resize(Size_t end_size);
		void PushBack(T element);
		T PeekBack();
		T PopBack();
		T& operator [](Size_t index);
		const T& operator [](Size_t index) const;
		T* begin() const; // foreach loop support
		T* end() const;
		DynamicArray(std::initializer_list<T>);
		DynamicArray();
	};
	using DynamicFloatArray = DynamicArray<float>;
	using DynamicIntArray = DynamicArray<int>;
	using DynamicUIntArray = DynamicArray<unsigned int>;

	template <Size_t IN, Size_t OUT>
	class Controller
	{
	public:
		virtual FixedFloatArray<OUT> Value(FixedFloatArray<IN>)=0;
	};

	template <Size_t IN>
	class Controller<IN, 0>
	{
	public:
		virtual void Value(FixedFloatArray<IN>) = 0;
	};
	template <Size_t OUT>
	class Controller<0, OUT>
	{
	public:
		virtual FixedFloatArray<OUT> Value() = 0;
	};
	// System & Math
	using String = std::string;
	using DynamicStringArray = DynamicArray<String>;
	const double Pi = 3.14159265358979323846;

	float deg2rad(float deg);
	float rad2deg(float rad);
	
	template <typename T>
	T min(T a, T b) { return (a < b) ? a : b; }
	
	template <typename T>
	T max(T a, T b) { return (a > b) ? a : b; }

	template <typename T>
	T clamp(T value, T minValue, T maxValue) { return max<T>(minValue, min<T>(maxValue, value)); }
	
	template <typename T>
	T lerp(T a, T b, float t) { return a * (1.0f - t) + b * t; }

	struct Vector2 // for now only floats, but this should be enough
	{
	public:
		float x, y;
		Vector2() : x(0), y(0) {};
		Vector2(float X, float Y) : x(X), y(Y) {};
		Vector2(float XandY) : Vector2(XandY, XandY){};
		Vector2(FixedFloatArray<2> arr) : Vector2(arr[0], arr[1]) {}
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
		Vector3(FixedFloatArray<3> arr) : Vector3(arr[0], arr[1], arr[2]) {}
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
	using DynamicVector3Array = DynamicArray<Vector3>;

	struct Rotator
	{
	public:
		float Angle;
		Vector3 Axis;
		Rotator() : Angle(0), Axis(0) {}
		Rotator(float angle, Vector3 axis) : Angle(angle), Axis(axis) {}
	};
	
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
		Vector3 Transform(const Vector3 vec) const;
		Matrix4 Translate(const Vector3 pos) const;
		Matrix4 Rotate(const Rotator rot) const;
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
		Color operator+(Color);
		Color operator*(float);
	};

	struct Vertex
	{
	public:
		Vector3 Position;
		Color Color_;
		Vector2 TextureCoords;
		Vector3 Normal;
		Vertex() {}
		Vertex(Vector3 p, Color c, Vector2 st) : Position(p), Color_(c), TextureCoords(st), Normal(0) {};
		Vertex(Vector3 p, Color c, Vector2 st, Vector3 n) : Position(p), Color_(c), TextureCoords(st), Normal(n) {};
	};
	using DynamicVertexArray = DynamicArray<Vertex>;
	// Controllers
	class KeyPressVector2Controller : Controller<0, 2>
	{
	public:
		Window* win;
		GLuint 
			KeyUp = GLFW_KEY_W, KeyDown = GLFW_KEY_S,
			KeyLeft = GLFW_KEY_A, KeyRight = GLFW_KEY_D;
		explicit KeyPressVector2Controller(Window* w) : win(w) {}
		FixedFloatArray<2> Value() override;
	};
	class KeyPressVector3Controller : Controller<0, 3>
	{
	public:
		Window* win;
		GLuint 
			KeyForward = GLFW_KEY_W, KeyBackward = GLFW_KEY_S,
			KeyLeft = GLFW_KEY_A, KeyRight = GLFW_KEY_D,
			KeyUp = GLFW_KEY_SPACE, KeyDown = GLFW_KEY_LEFT_SHIFT;
		explicit KeyPressVector3Controller(Window* w) : win(w) {}
		FixedFloatArray<3> Value() override;
	};
	class MouseOffsetController : Controller<0, 2>
	{
	private:
		Vector2 lastPos;
		bool first = true;
	public:
		Window* win;
		explicit MouseOffsetController(Window* w) : win(w) {}
		FixedFloatArray<2> Value() override;
	};
	class PitchYaw2DirectionController : Controller<2, 3>
	{
	public:
		FixedFloatArray<3> Value(FixedFloatArray<2> pitchYaw) override;
	};
	// Algorithms
	class ComputeShader : IShader, IDeleted
	{
		// not implemented yet
	};

	// Windowing
	struct Window : IDeleted
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
		void Delete() override;
		~Window();
	};

	struct AudioDevice : IDeleted
	{
	public:
		static DynamicStringArray GetDeviceNames();
		String GetName();
		ALenum GetDeviceError();
		ALenum GetContextError();
		ALCdevice* handle;
		ALCcontext* context;
		void Delete() override;
		~AudioDevice();
	};

	class Windowing
	{
	private:
		static bool initialized;
	public:
		static Window* Init(unsigned int Width, unsigned int Height, const char* Title, bool depth); // initialize the current windowng system, that includes initializing glad
		static Window* CreateWindow(unsigned int Width, unsigned int Height, const char* Title);
		static AudioDevice* OpenAudioDevice();
		static AudioDevice* OpenAudioDevice(const char* Device_name);
		static void DestroyWindow(Window* win);
		static void CloseAudioDevice(AudioDevice* device);
		static void Terminate();
		static void PollEvents();
	};
	class Event;
	struct EventArgs
	{
	public:
		Event* Sender;
	};
	class EventListener
	{
	public:
		virtual void Callback(EventArgs*) = 0;
	};
	class Event
	{
		DynamicArray<EventListener*> subcribers;
	public:
		void Invoke(EventArgs*);
		void operator +=(EventListener*);
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
	class TimeoutEvent : public Event {};
	class Timer
	{
		friend class Windowing;
	private:
		static DynamicArray<Timer*> all;
		static void update_all();
		void update();
		float start_time;
		float wait_time;
		bool running;
	public:
		bool Loop = false;
		Timer();
		TimeoutEvent Timeout;
		void Start(float time);
		float GetTime();
		float GetTimeLeft();
		void Wait();
		void Stop();
		bool isRunning();
	};
	// Rendering1
	class TextureData : IDeleted
	{
	public:
		Byte* data;
		GLint width, height;
		GLint channels;
		TextureData() {}
		TextureData(Byte* data_, GLint w, GLint h, GLint channels_)
			: data(data_), width(w), height(h), channels(channels_) {}
		void Delete() override;
	};
	class Texture : IDeleted
	{
		friend class TextureUnitManager;
	private:
		GLuint handle;
	public:
		Texture() : handle(-1) {}
		Texture(TextureData);
		void Bind();
		void Delete() override;
	};
	class CubeMap : IDeleted
	{
		friend class TextureUnitManager;
	private:
		GLuint handle;
	public:
		CubeMap() : handle(-1) {}
		CubeMap(FixedArray<TextureData, 6>);
		void Bind();
		void Delete() override;
	};
	class TextureUnitManager
	{
		friend class Windowing;
	private:
		static GLuint* units;
		static Byte cur;
		static GLint MaxUnits;
	public:
		static GLint GetMaxTextureUnits();
		static Byte Add(Texture t);
		static Byte Add(CubeMap t);
		static void Clear();
	};
	struct Brush;
	class Material;
	class Shader : IShader, IDeleted
	{
		friend class ResourceLoader;
	public:
		void Use();
		void Delete() override;
		void SetFloat(float v, const char* name);
		void SetInt(int v, const char* name);
		void SetBool(bool v, const char* name);
		void SetVector2(Vector2 v, const char* name);
		void SetVector3(Vector3 v, const char* name);
		void SetMatrix4(Matrix4 v, const char* name);
		void SetColor(Color v, const char* name);
		void SetTexture(Texture t, const char* name);
		void SetCubeMap(CubeMap t, const char* name);
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
		Brush(Color c) : Solid(c), isSolid(true){}
		Brush(Texture t) : Mapped(t), isSolid(false){}
	};
	class Material : IDeleted
	{
	public:
		Brush Albedo,Diffuse,Specular;
		float Shininess, Ambient;
		Material() : Shininess(1), Ambient(1){}
		void Delete() override;
	};
	namespace Helpers
	{
		class Materials
		{
		public:
			static Material Gold();
		};
	}
	class Renderer : IDeleted
	{
	public:
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
			GLuint Attributes; 
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
	// Audio
	struct AudioHeaderData
	{
	public:
		ALint SampleRate;
		ALint BitsPerSample;
		GLuint Channels;
		Size_t Size; // size of the data, header excluded
		ALdouble GetDuration();
	};
	class AudioBuffer : IDeleted
	{
		friend class ResourceLoader;
		friend class AudioSource;
	private:
		ALuint handle;
	public:
		void Delete() override;
	};
	class AudioListener
	{

	};
	class AudioSource : IDeleted
	{
	private:
		ALuint handle;
		DynamicArray<AudioBuffer> streamingBuffers;
	public:
		ALuint StreamingBufferCount; // the amount of buffers used to stream audio, don't change this mid-streaming
		Size_t StreamingBufferSize; // the size of buffer used to stream audio, don't change this mid-streaming
		ALfloat Pitch ;
		ALfloat Gain;
		Vector3 Position;
		Vector3 Velocity;
		bool Loop = false;
		AudioSource();
		void Delete() override;
		void Play(AudioBuffer* buffer);
		void Play(InputStream* stream, AudioHeaderData* header);
		void Resume(); // if stopped, replays the source 
		void Stop();
		void Pause();
		void Rewind();
		bool IsPlaying();
		/// <summary>
		/// Must be called consistently after calling Play(InputStream), and until playing is finished/stopped.
		/// Perfect call frequency depends on StreamingBufferCount and StreamingBufferSize,
		/// but easiest way is to call per frame
		/// </summary>
		/// <param name="same_stream">the stream passed earlier to the Play function</param>
		void UpdateStreaming(InputStream* same_stream);
	};
	// Engine
	class CoordinateSystem
	{
	public:
		virtual Matrix4 TransformationTo()=0;
		virtual Matrix4 TransformationFrom()=0;
	};
	class Camera
	{
	public:
		virtual Matrix4 GetView() = 0;
	};
	class CollisionSolver {};
	// Enigne 2D
	// Rendering
	class CoordinateSystem2D : CoordinateSystem
	{
	public:
		Vector2 Position;
		float Rotation;
		Vector2 Scale;
		CoordinateSystem2D() : Position(0.0f),Rotation(0.0f),Scale(1.0f) {}
		Matrix4 TransformationTo() override;
		Matrix4 TransformationFrom() override;
	};

	class Camera2D : public Camera
	{
	public:
		Vector2 Position;
		float Rotation;
		float Zoom;
		Camera2D() : Position(0.0f), Rotation(0.0f) {};
		CoordinateSystem2D GetCoordinateSystem();
		Matrix4 GetView();
	};
	class RenderedObject2D : IDeleted
	{
	protected:
		virtual void SetGraphics(Window* win);
		virtual void SetGeometry(Window* win);
	public:
		Shader Shader_;
		Renderer Renderer_;
		Material Material_;
		CoordinateSystem2D T;
		RenderedObject2D(Shader* sh, Mesh m, Renderer::VertexAttributesConfig a) : Shader_(*sh), Renderer_(&m, a) {};
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
	
	// Engine 3D
	// Rendering
	class CoordinateSystem3D : CoordinateSystem
	{
	public:
		Vector3 Position;
		Rotator Rotation;
		Vector3 Scale;
		CoordinateSystem3D() : Position(0.0f), Rotation(), Scale(1.0f) {}
		Matrix4 TransformationTo() override;
		Matrix4 TransformationFrom() override;
	};
	class Camera3D : public Camera
	{
	public:
		float FOV;
		Vector3 Position;
		Vector3 Direction;
		Vector3 Up;
		Vector3 Right;
		void SetDirection(Vector3);
		Camera3D() : Position(0.0f), Direction(0.0f, 0.0f, -1.0f),
			Up(0.0f, 1.0f, 0.0f), FOV(45.0f) {};
		CoordinateSystem3D GetCoordinateSystem();
		Matrix4 GetView();
	};
	class RenderedObject3D : IDeleted
	{
	protected:
		virtual void SetGraphics(Window* win);
		virtual void SetGeometry(Window* win);
	public:
		Renderer Renderer_;
		Shader Shader_;
		Material Material_;
		CoordinateSystem3D T;
		RenderedObject3D(Shader* sh, Mesh m, Renderer::VertexAttributesConfig a) : Shader_(*sh), Renderer_(&m, a) {};
		void Delete() override;
		virtual void Draw(Window* win);
	};
	class MappedCube : RenderedObject3D
	{
	protected:
		virtual void SetGraphics(Window* win);
	public:
		CubeMap CubeMap_;
		MappedCube() : RenderedObject3D(
			Helpers::ShaderLib::LoadBasic("CubeMap"), Helpers::Meshes::Cube(), Renderer::Position3D) {}
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
	bool LoadWav_(std::ifstream* file, Melon::AudioHeaderData* header, char* data);
	class ResourceLoader 
	{
	public:
		static bool LoadTextureData(TextureData* result,const char* filename);
		static bool LoadShader(Shader* result, const char* vertFile, const char* fragFile);
		static bool LoadShader(Shader* result, const char* vertFile, const char* fragFile, const char* geomFile);
		static bool LoadAudio(AudioBuffer* result, const char* filename);
	};
}

#include "Collections.inl"