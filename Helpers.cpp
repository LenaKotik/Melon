#include "Melon.hpp"
// Rendering1

Melon::Mesh Melon::Helpers::Meshes::Quad()
{
	DynamicVertexArray verticies = {
		Vertex(Vector3(-0.5f, 0.5f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),	// top left  
		Vertex(Vector3(0.5f, 0.5f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),	// top right 
		Vertex(Vector3(-0.5f,-0.5f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),	// bottom left
		Vertex(Vector3(0.5f,-0.5f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),	// bottom right
	};
	DynamicUIntArray indecies = {
		0, 2, 1,
		2, 3, 1,
	};
	return { verticies, indecies, true };
}

Melon::Mesh Melon::Helpers::Meshes::Triangle()
{
    DynamicVertexArray verticies = {
        Vertex(Vector3(0.0f, 0.5f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f,-0.5f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3(-0.5f,-0.5f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),
    };
    return { verticies, {0}, false };
}

Melon::Mesh Melon::Helpers::Meshes::Cube()
{
    DynamicVertexArray verticies =
    {
        Vertex(Vector3(-0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),
        Vertex(Vector3( 0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3( 0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3( 0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),

        Vertex(Vector3(-0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),
        Vertex(Vector3( 0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3( 0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3( 0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),

        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3(-0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),
        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),

        Vertex(Vector3( 0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3( 0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3( 0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3( 0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3( 0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),
        Vertex(Vector3( 0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),

        Vertex(Vector3(-0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3( 0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3( 0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3( 0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3(-0.5f,-0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),
        Vertex(Vector3(-0.5f,-0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),

        Vertex(Vector3(-0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
        Vertex(Vector3( 0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        Vertex(Vector3( 0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3( 0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),
        Vertex(Vector3(-0.5f, 0.5f,-0.5f), Color(1.0f, 1.0, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),
    };
    return { verticies, {0}, false };
}
