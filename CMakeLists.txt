# CMakeList.txt : CMake project for CMakeMelon, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project ("Melon")

# find packages

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(Freetype REQUIRED)


configure_file(MelonConfig.h.in MelonConfig.h)

add_library(Melon
			STATIC
			Melon.hpp 
			Animation.inl 
			Audio.cpp 
			AudioLoading.cpp 
			Collections.inl 
			DynamicArray.inl 
			Math.cpp 
			MeshLib.cpp 
			Misc.cpp 
			Rendering.cpp 
			Rendering2.cpp 
			ResourceLoader.cpp 
			ROLib.cpp 
			ShaderLib.cpp 
			stb_image.h 
			stb_image.cpp 
			System.cpp
			Windowing.cpp
	)

target_link_libraries(Melon PUBLIC glfw glad::glad OpenAL::OpenAL Freetype::Freetype)

target_include_directories(Melon PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

add_subdirectory(Examples)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Melon PROPERTY CXX_STANDARD 20)
endif()