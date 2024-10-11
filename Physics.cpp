#include "Melon.hpp"

void Melon::PhysicsObject3D::SetCollisionShape(CollisionShape3D* s)
{
	s->Owner = this;
	s->T.Parent = &this->T;
	collider = s;
}

Melon::Rect Melon::AABBCollisionShape2D::GetGlobalRect()
{
	return Rect(T.GetGlobalPosition(), Vector2(Width, Height));
}

Melon::Vector3 Melon::AABBCollisionShape3D::ClosestToPoint(Vector3 point) const  // TODO: implement pls
{
	return Vector3::ZERO();
}

bool Melon::CollisionManager2D::GetCollision(Collision2D* res, AABBCollisionShape2D* A, AABBCollisionShape2D* B) // TODO: implement this, i am too lazy rn
{
	Vector2 Apos = A->T.GetGlobalPosition(), Bpos = B->T.GetGlobalPosition();

	
	return false;
}

bool Melon::CollisionManager3D::GetCollision(Collision3D* res, AABBCollisionShape3D* A, AABBCollisionShape3D* B)
{
	Vector3 Apos = A->T.GetGlobalPosition(), Bpos = B->T.GetGlobalPosition();

	float depth_x = -(abs(Apos.x - Bpos.x) - (A->Size.x + B->Size.x) * 0.5f);
	float depth_y = -(abs(Apos.y - Bpos.y) - (A->Size.y + B->Size.y) * 0.5f);
	float depth_z = -(abs(Apos.z - Bpos.z) - (A->Size.z + B->Size.z) * 0.5f);

	if (depth_x < 0 || depth_y < 0 || depth_z < 0)
		return false;

	res->ShapeA = A;
	res->ShapeB = B;
	res->ObjectA = A->Owner;
	res->ObjectB = B->Owner;

	float min_depth = min(min(depth_x, depth_y), depth_z);
	
	if (min_depth == depth_x)
	{
		res->Normal = Vector3::PX() * copysignf(1.0f, Apos.x - Bpos.x);
		res->Point = Apos + res->Normal * (A->Size.x * 0.5f-depth_x);
		res->Depth = depth_x;
	}
	else if (min_depth == depth_y)
	{
		res->Normal = Vector3::PY() * copysignf(1.0f, Apos.y - Bpos.y);
		res->Point = Apos + res->Normal * (A->Size.y * 0.5f - depth_y);
		res->Depth = depth_y;
	}
	else
	{
		res->Normal = Vector3::PZ() * copysignf(1.0f, Apos.z - Bpos.z);
		res->Point = Apos + res->Normal * (A->Size.z * 0.5f - depth_z);
		res->Depth = depth_z;
	}

	return true;
}