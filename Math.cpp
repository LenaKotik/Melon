#include "Melon.hpp"

float Melon::deg2rad(float deg)
{
	return deg / (180.0 / Pi);
}


float Melon::rad2deg(float rad)
{
	return rad * (180.0 / Pi);
}

Melon::Vector2 Melon::Vector2::operator=(const Vector2& oth)
{
	this->x = oth.x; this->y = oth.y;
	return *this;
}

Melon::Vector2 Melon::Vector2::operator+=(const Vector2& oth)
{
	*this = *this + oth;
	return *this;
}

Melon::Vector2 Melon::Vector2::operator-=(const Vector2& oth)
{
	*this = *this - oth;
	return *this;
}

Melon::Vector2 Melon::Vector2::operator*=(const float& scalar)
{
	*this = *this * scalar;
	return *this;
}

Melon::Vector2 Melon::Vector2::operator+(const Vector2& oth) const
{
	return Vector2(x + oth.x, y + oth.y);
}

Melon::Vector2 Melon::Vector2::operator-(const Vector2& oth) const
{
	return Vector2(x - oth.x, y - oth.y);
}

Melon::Vector2 Melon::Vector2::operator*(const float& scalar) const
{
	return Vector2(x*scalar, y*scalar);
}

Melon::Vector2 Melon::Vector2::operator-() const
{
	return Vector2(-x, -y);
}

float Melon::Vector2::Dot(const Vector2& oth) const
{
	return x*oth.x + y*oth.y;
}

float Melon::Vector2::Magnitude() const
{
	return sqrt(x*x + y*y);
}

float Melon::Vector2::MagnitudeSqr() const
{
	return x*x + y*y;
}

Melon::Vector2 Melon::Vector2::Normalize() const
{
	float m = this->Magnitude();
	return Vector2(x/m, y/m);
}

Melon::Vector3 Melon::Vector3::operator=(const Vector3& oth)
{
	this->x = oth.x; this->y = oth.y; this->z = oth.z;
	return *this;
}

Melon::Vector3 Melon::Vector3::operator+=(const Vector3& oth)
{
	*this = *this + oth;
	return *this;
}

Melon::Vector3 Melon::Vector3::operator-=(const Vector3& oth)
{
	*this = *this - oth;
	return *this;
}

Melon::Vector3 Melon::Vector3::operator*=(const float& scalar)
{
	*this = *this * scalar;
	return *this;
}

Melon::Vector3 Melon::Vector3::operator+(const Vector3& oth) const
{
	return Vector3(this->x + oth.x, this->y + oth.y, this->z + oth.z);
}

Melon::Vector3 Melon::Vector3::operator-(const Vector3& oth) const
{
	return Vector3(this->x - oth.x, this->y - oth.y, this->z - oth.z);
}

Melon::Vector3 Melon::Vector3::operator*(const float& scalar) const
{
	return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Melon::Vector3 Melon::Vector3::operator-() const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Melon::Vector3 Melon::Vector3::Cross(const Vector3& oth)
{
	return Vector3(y*oth.z - z*oth.y, x*oth.z - z*oth.x, x*oth.y - y*oth.x);
}

float Melon::Vector3::Dot(const Vector3& oth) const
{
	return this->x * oth.x + this->y * oth.y, this->z * oth.z;
}

float Melon::Vector3::Magnitude() const
{
	return sqrt(x*x + y*y + z*z);
}

float Melon::Vector3::MagnitudeSqr() const
{
	return x * x + y * y + z * z;
}

Melon::Vector3 Melon::Vector3::Normalize() const
{
	float m = this->Magnitude();
	return Vector3(x/m, y/m, z/m);
}

Melon::Matrix4::Matrix4() : Value()
{
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		Value[x][y] = 0;
}

Melon::Matrix4::Matrix4(float val[4][4])
{
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		Value[x][y] = val[x][y];
}

Melon::Matrix4::Matrix4(float scal) : Matrix4()
{
	for (int xy = 0; xy < 4; xy++)
		Value[xy][xy] = scal;
}

Melon::Matrix4 Melon::Matrix4::operator+(Matrix4& oth) const
{
	Matrix4 res;
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		res.Value[x][y] = Value[x][y] + oth.Value[x][y];
	return res;
}

Melon::Matrix4 Melon::Matrix4::operator-(Matrix4& oth) const
{
	Matrix4 res;
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		res.Value[x][y] = Value[x][y] - oth.Value[x][y];
	return res;
}

Melon::Matrix4 Melon::Matrix4::operator*(Matrix4& oth) const
{
	return Matrix4();
}

Melon::Matrix4 Melon::Matrix4::operator*(float& scalar) const
{
	return Matrix4();
}

Melon::Matrix4 Melon::Matrix4::Translate(Vector3 pos) const
{
	return Matrix4();
}

Melon::Matrix4 Melon::Matrix4::Rotate(float angleScal, Vector3 vec) const
{
	return Matrix4();
}

Melon::Matrix4 Melon::Matrix4::Scale(float scalar) const
{
	return Matrix4();
}
