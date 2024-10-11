#pragma once

#include "MelonConfig.h"

#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h> 
#include <AL/al.h>
#include <AL/alc.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#define MELON_ABSTRACTIONS
#define MELON_SYSTEM_AND_MATH
#define MELON_CONTROLLERS
#define MELON_WINDOWING
#define MELON_RENDERING
#define MELON_AUDIO
#define MELON_ENGINE
#define MELON_ENGINE_2D
#define MELON_ENGINE_3D
#define MELON_RESOURCES

typedef std::uint8_t Byte;
typedef unsigned long long Size_t;
namespace Melon
{
	// Prototypes
	class Camera;
	class Renderer;
	struct Window;
	struct Vector2;

#ifdef MELON_ABSTRACTIONS
	// Abstractions & Templates
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
	struct Color;
	class RenderTarget // An object you can render to
	{
	public:
		Camera* MainCamera;
		virtual void Bind() = 0;
		virtual Vector2 GetSize()=0;
		float GetAspect();
		virtual void Clear(Color);
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
		/// <param name="idx">index of the new element</param>
		void Insert(T element, int idx);
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
	
	template <typename K,typename V>
	using Map = std::map<K, V>;
	
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
#endif // MELON_ABSTRACTIONS

#ifdef MELON_SYSTEM_AND_MATH
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
		float Angle(const Vector2& oth) const;
		float Magnitude() const;
		float MagnitudeSqr() const;
		Vector2 Normalize() const;
		static Vector2 ZERO()
		{
			return Vector2(0.0f, 0.0f);
		}
		static Vector2 UP()
		{
			return Vector2(0.0f, 1.0f);
		}
		static Vector2 DOWN()
		{
			return Vector2(0.0f, -1.0f);
		}
		static Vector2 LEFT()
		{
			return Vector2(-1.0f, 0.0f);
		}
		static Vector2 RIGHT()
		{
			return Vector2(1.0f, 0.0f);
		}
	};
	using DynamicVector2Array = DynamicArray<Vector2>;

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
		Vector3 Cross(const Vector3& oth) const;
		float Dot(const Vector3& oth) const;
		float Angle(const Vector3& oth) const;
		float Magnitude() const;
		float MagnitudeSqr() const;
		Vector3 Normalize() const;
		static Vector3 PX()
		{
			return Vector3(1.0f, 0.0f, 0.0f);
		}
		static Vector3 PY()
		{
			return Vector3(0.0f, 1.0f, 0.0f);
		}
		static Vector3 PZ()
		{
			return Vector3(0.0f, 0.0f, 1.0f);
		}
		static Vector3 NX()
		{
			return Vector3(-1.0f, 0.0f, 0.0f);
		}
		static Vector3 NY()
		{
			return Vector3(0.0f, -1.0f, 0.0f);
		}
		static Vector3 NZ()
		{
			return Vector3(0.0f, 0.0f, -1.0f);
		}
		static Vector3 ZERO()
		{
			return Vector3(0.0f, 0.0f, 0.0f);
		}
	};
	using DynamicVector3Array = DynamicArray<Vector3>;

	struct Rect
	{
		Vector2 Position;
		Vector2 Size;
		Rect() : Position(0), Size(0) {}
		Rect(Vector2 pos, Vector2 size) : Position(pos),Size(size) {}
		Rect(float x, float y, float w, float h) : Position(x,y),Size(w,h) {}
	};

	struct Rotator
	{
	public:
		float Angle;
		Vector3 Axis;
		Rotator operator+=(const Rotator&);
		Rotator operator+(const Rotator&) const;
		Rotator() : Angle(0), Axis(0,0,1) {}
		Rotator(float angle, Vector3 axis) : Angle(angle), Axis(axis.Normalize()) {}
		static Rotator FromEulerVector(Vector3 euler_vector)
		{
			Rotator res;
			res.Angle = (euler_vector.Magnitude());
			if (res.Angle!=0)
				res.Axis = euler_vector * (1.0f / res.Angle);
			return res;
		}
		static Rotator FromDirection(Vector3 dir)
		{
			Rotator res;
			res.Axis = dir.Normalize().Cross(Vector3::PZ());
			res.Angle = dir.Angle(Vector3::PZ());
			return res;
		}
		Vector3 AsEulerVector() const { return Axis * Angle; }
	};
	template <typename T>
	T lerp(T a, T b, float t);

	template <>
	Rotator lerp(Rotator a, Rotator b, float t);
	
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
		Vector2 Transform(const Vector2 vec) const;
		Matrix4 Translate(const Vector3 pos) const;
		Matrix4 Rotate(const Rotator rot) const;
		Matrix4 Scale(const float scalar) const;
		Matrix4 Scale(const Vector3 scalar) const;
		Matrix4 Transpose() const;
		Matrix4 Inverse() const; // DO NOT USE, NOT IMPLEMENTED
		static Matrix4 FromBasis(Vector3 i, Vector3 j, Vector3 k);
		static Matrix4 Perspective(float FOV_deg, float aspect, float near, float far);
		static Matrix4 Ortho(float aspect, float near, float far);
	};

	struct Color
	{
	public:
		float R, G, B, A;
		Color() : R(1), G(1), B(1), A(1) {};
		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}; 
		static Color FromBytes(GLubyte r, GLubyte g, GLubyte b, GLubyte a=255); // converts colors from [0-255] range to [0.0f-1.0f] range
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

	const String SourceDir = SOURCE_DIR;
#endif // MELON_SYSTEM_AND_MATH

#ifdef MELON_CONTROLLERS
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
#endif // MELON_CONTROLLERS

#ifdef MELON_ALGORITHMS
	// Algorithms
	class ComputeShader : IShader, IDeleted
	{
		// not implemented yet
	};
#endif // MELON_ALGORITHMS

#ifdef MELON_WINDOWING
	// Windowing
	struct Window : IDeleted, RenderTarget
	{
	public:
		GLFWwindow* handle;
		//Camera* MainCamera;
		bool ShouldClose();
		bool IsKeyPressed(int key);
		void MakeActive();
		virtual void Bind() override;
		void Maximize();
		void Minimize();
		void SetCursor(bool);
		Vector2 GetMousePosition();
		virtual Vector2 GetSize() override;
		//virtual void Clear(Color, bool depth) override;
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
		friend class RenderTarget;
		friend class FrameBufferFactory;
	private:
		static bool initialized;
		static bool depth_;
	public:
		static Window* Init(unsigned int Width, unsigned int Height, const char* Title, bool depth); // initialize the current windowng system, that includes initializing glad
		static Window* CreateWindow(unsigned int Width, unsigned int Height, const char* Title);
		static AudioDevice* OpenAudioDevice();
		static AudioDevice* OpenAudioDevice(const char* Device_name);
		static FT_Library freetype_handle;
		static void SetDepth(bool value);
		static bool InitFreetype();
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
#endif // MELON_WINDOWING

#ifdef MELON_RENDERING
	// Rendering1
	class TextureData : IDeleted
	{
	public:
		Byte* data;
		GLint width, height;
		GLint channels;
		GLenum wraping_mode;
		GLenum min_filtering_mode;
		GLenum mag_filtering_mode;
		bool enable_mipmap;
		TextureData() {}
		TextureData(Byte* data_, GLint w, GLint h, GLint channels_=4,GLenum wraping_mode_= GL_MIRRORED_REPEAT, GLenum min_filtering_mode_= GL_LINEAR_MIPMAP_LINEAR, GLenum mag_filtering_mode_ = GL_LINEAR, bool enable_mipmap_=true)
			: data(data_), width(w), height(h), channels(channels_), 
				wraping_mode(wraping_mode_), min_filtering_mode(min_filtering_mode_), mag_filtering_mode(mag_filtering_mode_), enable_mipmap(enable_mipmap_) {}
		void Delete() override;
	};
	class Texture : IDeleted
	{
		friend class FrameBuffer;
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
		//friend class TextureUnitManager;
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
		static Byte GetCurrentUnit();
		static Byte Add(Texture*);
		//static Byte Add(CubeMap t);
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
		void SetFloatArray(const float* v, Size_t count, const char* name);
		void SetColor(Color v, const char* name);
		void SetTexture(Texture* t, const char* name);
		void SetCubeMap(CubeMap* t, const char* name);
		void SetBrush(Melon::Brush b, const char* name);
		void SetMaterial(Melon::Material m, const char* name);
	};

	struct Mesh
	{
	public:
		Mesh() {};
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
			static Mesh QuadFromRect(Rect);
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
		Texture* Mapped;
		Brush() : Brush(Color()) {} // as white solid
		Brush(Color c) : Solid(c), isSolid(true){}
		Brush(Texture* t) : Mapped(t), isSolid(false){}
		Brush(Texture* t, Color c) : Mapped(t), Solid(c), isSolid(false) {}
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
	class DepthBuffer : IDeleted
	{
		friend class FrameBuffer;
	private:
		GLuint handle;
		Vector2 size_;
	public:
		DepthBuffer(Vector2 size);
		void Bind();
		virtual void Delete() override;
	};
	class FrameBuffer : IDeleted, public RenderTarget
	{
		friend class FrameBufferFactory;
	private:
		Vector2 size_;
		GLuint handle;
		DepthBuffer* depthAttachment;
		DynamicArray<Texture*> colorAttachments;
	public:
		FrameBuffer(Vector2 size);
		virtual Vector2 GetSize() override
		{
			return size_;
		}
		void DepthAttachment(DepthBuffer*);
		void ColorAttachment(Texture*);
		Texture* GetColorAttachment(int idx);
		virtual void Bind() override;
		virtual void Delete() override;
	};
	class FrameBufferFactory
	{
	public:
		static FrameBuffer* GetBasic(Vector2 size);
	};
	class Renderer : IDeleted
	{
	public:
		enum VertexAttributesConfig;
	private:
		bool indexed;
		GLuint VAO, VBO, EBO;
		int indC, vertC;
	public:
		GLenum PrimitiveType;
		static DynamicFloatArray GenBuffer(DynamicVertexArray arr, VertexAttributesConfig bitmask, int* stride, DynamicUIntArray* offsets, DynamicUIntArray* sizes);
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
	struct Glyph
	{
	public:
		Texture texture;
		Vector2 Size;
		Vector2 Bearing;
		unsigned int Advance;
		Rect GetBoundingBox();
	};
	class Font : IDeleted
	{
		friend class ResourceLoader;
	private:
		FT_Face handle;
		Map<char, Glyph> glyphs;
	public:
		static const String ASCII;
		Glyph GetGlyph(char);
		bool AutoPreload = false;
		bool PreloadGlyph(char);
		bool PreloadGlyphs(String);
		void Delete() override;
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
#ifdef MELON_ALGORITHMS
			static ComputeShader* LoadCompute(String shadername); // load a specific compute shader
#endif // MELON_ALGORITHMS
			static Shader* LoadBasic(ShaderLoadOptions options); // load a basic pipeline by option
			static Shader* LoadBasic(String shadername); // load a specific basic pipeline
			static Shader* LoadGeom(String shadername); // load a specific geometry pipeline
		};
	}
#endif // MELON_RENDERING

#ifdef MELON_AUDIO
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
		AudioHeaderData headerData;
	public:
		AudioHeaderData GetHeaderData() const;
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
		AudioBuffer* Buffer;
		ALfloat Pitch;
		ALfloat Gain;
		Vector3 Position;
		Vector3 Velocity;
		bool Loop = false;
		AudioSource();
		void Delete() override;
		void Play();
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
#endif // MELON_AUDIO

#ifdef MELON_ENGINE
	// Engine
	class CoordinateSystem
	{
	public:
		virtual Matrix4 TransformationTo() const=0;
		virtual Matrix4 TransformationFrom() const=0;
		virtual Matrix4 LocalTransformationTo() const=0;
		virtual Matrix4 LocalTransformationFrom() const=0;
	};
	class Camera
	{
	public:
		virtual Matrix4 GetView() = 0;
	};
	class CollisionSolver {};
	class ShaderGraphics
	{
	public:
		virtual void SetGraphics(Shader*)=0;
		virtual bool SetTexture(Texture*, int id=0) { return 0; }
		virtual bool SetColor(Color, int id=0) { return 0; }
		virtual bool SetBrush(Brush, int id=0) { return 0; }
		virtual bool SetMaterial(Material, int id=0) { return 0; }
	};
	class ColorGraphics : ShaderGraphics
	{
	public:
		Color Color_;
		virtual void SetGraphics(Shader*);
		virtual bool SetColor(Color, int id=0) override;
	};
	class TextureGraphics : ShaderGraphics
	{
	public:
		Texture* Texture_;
		virtual void SetGraphics(Shader*) override;
		virtual bool SetTexture(Texture*, int id=0) override;
	};
	class BrushGraphics : ShaderGraphics
	{
	public:
		Brush Brush_;
		virtual void SetGraphics(Shader*) override;
		virtual bool SetColor(Color, int id = 0) override;
		virtual bool SetTexture(Texture*, int id = 0) override;
		virtual bool SetBrush(Brush, int id = 0) override;
	};
	class MaterialGraphics : ShaderGraphics
	{
	public:
		Material Material_;
		virtual void SetGraphics(Shader*) override;
		virtual bool SetColor(Color, int id = 0) override;
		virtual bool SetTexture(Texture*, int id = 0) override;
		virtual bool SetBrush(Brush, int id = 0) override;
		virtual bool SetMaterial(Material, int id = 0) override;
	};
	class BorderGraphics : ShaderGraphics
	{
	public:
		Color Color_;
		Color BorderColor;
		float BorderWidth;
		virtual void SetGraphics(Shader*) override;
		/// <param name="id">0 = Interior| 1 = Border</param>
		virtual bool SetColor(Color, int id) override;
		bool SetWidth(float);
	};
	class CubeMapGraphics : ShaderGraphics
	{
	public:
		CubeMap* CubeMap_;
		virtual void SetGraphics(Shader*) override;
		bool SetCubeMap(CubeMap*, int id=0);
	};
	

	template <typename T>
	struct Keyframe
	{
	public:
		T value;
		float time;
	};
	template <typename T>
	class InterpolationTrack
	{
	public:
		DynamicArray<Keyframe<T>> keyframes;
		void Add(Keyframe<T> keyframe);
		float Length();
		T Get(float t);
	};
	template <typename T>
	class Animation
	{
		float start_time;
	public:
		float length;
		DynamicArray<InterpolationTrack<T>> tracks;
		void Add(InterpolationTrack<T> track);
		void ComputeLength();
		T operator[](int idx);
		bool backward = false;
		bool loop = false;
		void Play();
		void Stop();
		bool IsPlaying();
	};

	// Enigne 2D
#ifdef MELON_ENGINE_2D
	// Rendering
	class CoordinateSystem2D : CoordinateSystem
	{
	public:
		CoordinateSystem* Parent = nullptr;
		Vector2 Position;
		float Rotation;
		Vector2 Scale;
		Vector2 GetGlobalPosition();
		//float GetGlobalRotation();
		Vector2 GetGlobalScale();
		CoordinateSystem2D() : Position(0.0f),Rotation(0.0f),Scale(1.0f) {}
		Matrix4 LocalTransformationTo() const;
		Matrix4 LocalTransformationFrom() const;
		Matrix4 TransformationTo() const override;
		Matrix4 TransformationFrom() const override;
	};
	class ShaderTransform2D
	{
	public:
		virtual void SetTransform(Shader*, const CoordinateSystem2D&) = 0;
	};
	class DefaultTransform2D : ShaderTransform2D
	{
	public:
		virtual void SetTransform(Shader*, const CoordinateSystem2D&);
	};
	class Camera2D : public Camera
	{
	public:
		CoordinateSystem2D T;
		Camera2D() : T(){};
		Matrix4 GetView();
	};
	class RenderedObject2D : IDeleted
	{
	public:
		Shader Shader_;
		Renderer Renderer_;
		ShaderGraphics* Graphics;
		ShaderTransform2D* Transform;
		CoordinateSystem2D T;
		RenderedObject2D(Shader* sh, Mesh *m, Renderer::VertexAttributesConfig a) : Shader_(*sh), Renderer_(m, a) {};
		virtual void Delete();
		virtual void Draw(RenderTarget* target);
	};
	class RenderedObject2DBuilder
	{
		char state;
		Mesh* m;
		Renderer::VertexAttributesConfig vac;
		Shader* sh;
		ShaderGraphics* gr;
		ShaderTransform2D* tr;
	public:
		bool SetShader(Shader*);
		bool SetGraphics(ShaderGraphics*);
		bool SetTransform2D(ShaderTransform2D*);
		bool SetRenderer(Mesh*, Renderer::VertexAttributesConfig);
		bool Done();
		RenderedObject2D* Get();
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
	class RenderedText : IDeleted
	{
	private:
		unsigned int VAO, VBO;
		Mesh mesh;
	public:
		RenderedText(Shader*,Font*);
		Shader Shader_;
		CoordinateSystem2D T;
		Color Color_;
		Font* Font_;
		String Text;
		void Draw(RenderTarget*);
		virtual void Delete() override;
	};
	namespace Helpers
	{
		class Text
		{
		public:
			static RenderedText* Default(Font*);
		};
	}
	// Physics
	class PhysicsObject2D;
	class CollisionShape2D
	{
	public:
		PhysicsObject2D* Owner;
		CoordinateSystem2D T;
		virtual float DistanceToPoint(Vector2 point) const=0;
	};
	class AABBCollisionShape2D : public CollisionShape2D
	{
	public:
		float Width, Height;
		Rect GetGlobalRect();
		AABBCollisionShape2D() : Width(0.0f), Height(0.0f) {};
		virtual float DistanceToPoint(Vector2 point) const;
	};
	struct Collision2D
	{
	public:
		PhysicsObject2D* ObjectA;
		CollisionShape2D* ShapeA;
		PhysicsObject2D* ObjectB;
		CollisionShape2D* ShapeB;
		Vector2 Normal;
		Vector2 Point;
	};
	class CollisionManager2D
	{
		static bool GetCollision(Collision2D* res, AABBCollisionShape2D* A, AABBCollisionShape2D* B);
	};
	class PhysicsObject2D
	{
		CollisionShape2D* collider;
	public:
		CoordinateSystem2D T;
		CollisionShape2D* GetCollisionShape();
		CollisionShape2D* SetCollisionShape();
	};
	
#endif // MELON_ENGINE_2D

#ifdef MELON_ENGINE_3D
	// Engine 3D
	// Rendering
	class CoordinateSystem3D : CoordinateSystem
	{
	public:

		CoordinateSystem3D* Parent = nullptr;
		Vector3 Position;
		Rotator Rotation;
		Vector3 Scale;
		Vector3 GetGlobalPosition();
		CoordinateSystem3D() : Position(0.0f), Rotation(), Scale(1.0f) {}
		virtual Matrix4 LocalTransformationTo() const;
		virtual Matrix4 LocalTransformationFrom() const;
		Matrix4 TransformationTo() const override;
		Matrix4 TransformationFrom() const override;
	};
	class Camera3DCoordinateSystem : public CoordinateSystem3D
	{
	public:
		Vector3 Direction;
		Camera3DCoordinateSystem() : Direction(0.0f, 0.0f, 1.0f) {};
		//Vector3 Right;
		//Vector3 Up;
		Vector3 GetRightDirection();
		Vector3 GetUpDirection();

		virtual Matrix4 LocalTransformationTo() const override;
		virtual Matrix4 LocalTransformationFrom() const override;
	};
	class ShaderTransform3D
	{
	public:
		virtual void SetTransform(Shader*, const CoordinateSystem3D&) = 0;
	};
	class DefaultTransform3D : ShaderTransform3D
	{
	public:
		virtual void SetTransform(Shader*, const CoordinateSystem3D&);
	};
	class Camera3D : public Camera
	{
	public:
		float FOV; // DEGREES
		Camera3DCoordinateSystem T;
		Camera3D() : T(), FOV(90.0f) {};
		Matrix4 GetView();
	};
	class RenderedObject3D : IDeleted
	{
	public:
		Renderer Renderer_;
		Shader Shader_;
		ShaderGraphics* Graphics;
		ShaderTransform3D* Transform;
		CoordinateSystem3D T;
		RenderedObject3D(Shader* sh, Mesh* m, Renderer::VertexAttributesConfig a) : Shader_(*sh), Renderer_(m, a) {};
		void Delete() override;
		virtual void Draw(RenderTarget* target);
	};
	class RenderedObject3DBuilder
	{
		char state;
		Mesh* m;
		Renderer::VertexAttributesConfig vac;
		Shader* sh;
		ShaderGraphics* gr;
		ShaderTransform3D* tr;
	public:
		RenderedObject3DBuilder() : state(0) {}
		bool SetRenderer(Mesh*, Renderer::VertexAttributesConfig);
		bool SetShader(Shader*);
		bool SetGraphics(ShaderGraphics*);
		bool SetTransform3D(ShaderTransform3D*);
		bool Done();
		RenderedObject3D* Get();
	};
	class Skybox : RenderedObject3D // TODO: add deletion
	{
	friend class SkyboxFactory;
	Skybox(Shader*s,Mesh*m) : RenderedObject3D(s,m,Renderer::Position3D) {}
	public:
		CubeMap CubeMap_;
		virtual void Draw(RenderTarget* target);
	};
	class SkyboxFactory
	{
	public:
		static Skybox* Create(CubeMap);
	};
	namespace Helpers
	{
		class Objects3D
		{
		public:
			static RenderedObject3D* ColoredShape(Mesh m);
			static RenderedObject3D* TexturedShape(Mesh m);
			static RenderedObject3D* Shape(Mesh m);
			static RenderedObject3D* MappedCube(Mesh m);
		};
	}
	// Physics
	class PhysicsObject3D;
	class CollisionShape3D
	{
	public:
		PhysicsObject3D* Owner;
		CoordinateSystem3D T;
		virtual Vector3 ClosestToPoint(Vector3 point) const = 0;
	};
	class AABBCollisionShape3D : public CollisionShape3D
	{
	public:
		Vector3 Size;
		AABBCollisionShape3D() :Size(0.0f) {};
		virtual Vector3 ClosestToPoint(Vector3 point) const;
	};
	struct Collision3D
	{
	public:
		PhysicsObject3D* ObjectA;
		CollisionShape3D* ShapeA;
		PhysicsObject3D* ObjectB;
		CollisionShape3D* ShapeB;
		float Depth;
		Vector3 Normal;
		Vector3 Point;
	};
	class CollisionManager3D
	{
	public:
		static bool GetCollision(Collision3D* res, AABBCollisionShape3D* A, AABBCollisionShape3D* B);
	};
	class PhysicsObject3D
	{
		CollisionShape3D* collider;
	public:
		CoordinateSystem3D T;
		CollisionShape3D* GetCollisionShape();
		void SetCollisionShape(CollisionShape3D*);
	};
#endif // MELON_ENGINE_3D

#endif // MELON_ENGINE

#ifdef MELON_RESOURCES
	// Resources
	bool LoadWav_(std::ifstream* file, Melon::AudioHeaderData* header, char* data);
	class ResourceLoader 
	{
	public:
		static bool flipYTextures;
		static bool LoadTextureData(TextureData* result,const char* filename);
		static bool LoadShader(Shader* result, const char* vertFile, const char* fragFile);
		static bool LoadShader(Shader* result, const char* vertFile, const char* fragFile, const char* geomFile);
		static bool LoadAudio(AudioBuffer* result, const char* filename);
		static bool LoadFont(Font* result, const char* filename,long face_index=0);
	};
#endif // MELON_RESOURCES
}

#include "Collections.inl"
#include "Animation.inl"