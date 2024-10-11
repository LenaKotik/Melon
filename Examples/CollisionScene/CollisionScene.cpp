#include <Melon.hpp>

using namespace Melon;


int main()
{
	Window* win = Windowing::Init(800, 800, "Collision", true);
	if (!win) return -1;


	Color green = Color::FromBytes(9, 184, 27);
	Color red = Color::FromBytes(229, 25, 25);

	Mesh cube_mesh = Helpers::Meshes::Cube();
	RenderedObject3D* cubeA = Helpers::Objects3D::ColoredShape(cube_mesh);
	if (!cubeA) return -1;
	cubeA->Graphics->SetColor(green);

	RenderedObject3D* cubeB = Helpers::Objects3D::ColoredShape(cube_mesh);
	if (!cubeB) return -1;
	cubeB->Graphics->SetColor(green);


	PhysicsObject3D A, B;
	AABBCollisionShape3D Acoll, Bcoll;

	Acoll.Size = Vector3(1.0f);
	Bcoll.Size = Vector3(1.0f);

	A.SetCollisionShape(&Acoll);
	B.SetCollisionShape(&Bcoll);

	A.T.Position = Vector3(-0.0f, 0.0f, 3.0f);
	//B.T.Position = Vector3(2.0f, 0.0f, 3.0f);

	cubeA->T.Parent = &A.T;
	cubeB->T.Parent = &B.T;

	Animation<Vector3> anim_B_pos;
	InterpolationTrack<Vector3> track_B_pos;
	track_B_pos.Add({ Vector3(2.0f, 0.0f, 3.0f), 0.0 });
	track_B_pos.Add({ Vector3(2.0f, 0.0f, 3.0f), 1.0 });
	track_B_pos.Add({ Vector3(-2.0f, 0.0f, 3.0f), 3.0 });
	track_B_pos.Add({ Vector3(-2.0f, 0.0f, 3.0f), 4.0 });
	track_B_pos.Add({ Vector3(2.0f, 0.0f, 3.0f), 6.0 });
	anim_B_pos.Add(track_B_pos);
	anim_B_pos.loop = true;

	Camera3D cam;
	win->MainCamera = &cam;

	Collision3D col;

	anim_B_pos.Play();
	Time::GetDelta();
	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();

		B.T.Position = anim_B_pos[0];

		bool flag = CollisionManager3D::GetCollision(&col, &Acoll, &Bcoll);

		cubeA->Graphics->SetColor((flag)? red : green);

		win->Clear(Color::FromBytes(94, 94, 94));

		cubeA->Draw(win);
		cubeB->Draw(win);

		win->Flip();

		Windowing::PollEvents();
	}
	cubeA->Delete();
	cubeB->Delete();
	Windowing::DestroyWindow(win);
	Windowing::Terminate();
	return 0;
}