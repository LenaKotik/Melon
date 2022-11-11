#include "Melon.hpp"

void Melon::KinematicBody2D::Update(float delta)
{
	Vector2 Velocity = Position - PositionLast;
	PositionLast = Position;
	Position += Velocity + Acceleration * delta;// * delta;
	Acceleration = Vector2(0);
}

void Melon::CollisionSolver2D::Solve(CircleCollider2D& A, CircleCollider2D& B, Vector2* Ap, Vector2* Bp)
{
	Vector2 axis = *Bp - *Ap;
	if (axis.MagnitudeSqr() > powf(A.Radius + B.Radius, 2.0f))
		return;
	float dist = axis.Magnitude();
	axis.x /= dist;
	axis.y /= dist;
	float depth = dist - (A.Radius + B.Radius);
	*Ap -= axis * depth * 0.5;
	*Bp += axis * depth * 0.5;
}
