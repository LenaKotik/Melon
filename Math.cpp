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
	return Vector3(y*oth.z - z*oth.y, -(x*oth.z - z*oth.x), x*oth.y - y*oth.x);
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

Melon::Matrix4 Melon::Matrix4::operator=(const float v[4][4])
{
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		Value[x][y] = v[x][y];
	return *this;
}

Melon::Matrix4 Melon::Matrix4::operator=(const Matrix4& v)
{
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		this->Value[x][y] = v.Value[x][y];
	return *this;
}

Melon::Matrix4 Melon::Matrix4::operator+(const Matrix4& oth) const
{
	Matrix4 res;
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		res.Value[x][y] = Value[x][y] + oth.Value[x][y];
	return res;
}

Melon::Matrix4 Melon::Matrix4::operator-(const Matrix4& oth) const
{
	Matrix4 res;
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		res.Value[x][y] = Value[x][y] - oth.Value[x][y];
	return res;
}

/*
*   [1, 2]	     [5, 6]		[1 * 5 + 2 * 7,		1 * 6 + 2 * 8]		[c00 = a00 * b00 + a01 * b10,		c01 = a00 * b01 + a01 * b11]
*	[3, 4]   *	 [7, 8]	 =  [3 * 5 + 4 * 7,		3 * 6 + 2 * 8]		[c10 = a10 * b00 + a11 * b10,		c11 = a10 * b01 + a11 * b11]
*	
*	for x, y in range 2x2:
*		cxy = ax0 * b0y + ax1 * b1y;
*/

/*
* a larger example:
* [4, 2, 0]       [4, 2, 1]       [4 * 4 + 2 * 2 + 0 * 9, 4 * 2 + 2 * 0 + 0 * 4, 4 * 1 + 2 * 4 + 0 * 2]		[c00 = a00 * b00 + a01 * b10 + a02 * b20
* [0, 8, 1]	  *   [2, 0, 4]   =   [0 * 4 + 8 * 2 + 1 * 9, 0 * 2 + 8 * 0 + 1 * 4, 0 * 1 + 8 * 4 + 1 * 2]		[c10 = a10 * b00 + a11 * b10 + a12 * b20
* [0, 1, 0]       [9, 4, 2]       [0 * 4 + 1 * 2 + 0 * 9, 0 * 2 + 1 * 0 + 0 * 4, 0 * 1 + 1 * 4 + 0 * 2]		[c20 = a20 * b00 + a11
* 
*	for x,y in range 3x3:
*		cxy = ax0 * b0y + ax1 * b1y + ax2 * b2y;
* 
*	for x,y in range 4x4:
*		cxy = ax0 * b0y + ax1 * b1y + ax2 * b2y + ax3 * b2y;
*	????
*	for x,y in range 4x4:
*		for i in range 4:
*			cxy += axi * biy
* 
* mat x vector
* 
* [4, 2, 0]       [3]          [3 * 4 + 4 * 2 + 5 * 0]  [c0 = v0 * m00 + v1 * m01 + v2 * m02]
* [0, 7, 2]   *   [4]     =    [3 * 0 + 4 * 7 + 5 * 2]  [c1 = v0 * m10 + v1 * m11 + v2 * m12]
* [5, 4, 3]       [5]          [3 * 5 + 4 * 4 + 5 * 3]  [c2 = v0 * m20 + v1 * m21 + v2 * m22]
* 
*  cx = v0 * mx0 + v1 * mx1 + v2 * mx2
* 
*  cx = v0 * mx0 + v1 * mx1 + v2 * mx2 + v3 * mx3
* 
* for x,y in range 4x4:
*    cx += vy * mxy
* 
*/


Melon::Matrix4 Melon::Matrix4::operator*(const Matrix4& oth) const
{
	Matrix4 res;
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		for (int i = 0; i < 4; i++)
			res.Value[x][y] += this->Value[x][i] * oth.Value[i][y];
	return res;
}

Melon::Matrix4 Melon::Matrix4::operator*(const float& scalar) const
{
	Matrix4 res;
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		res.Value[x][y] = Value[x][y] * scalar;
	return res;
}

Melon::Vector3 Melon::Matrix4::Transform(const Vector3 vec) const
{
	Vector3 res(0);
	float w;
	res.x = vec.x * Value[0][0] + vec.y * Value[0][1] + vec.z * Value[0][2] + Value[0][3];
	res.y = vec.x * Value[1][0] + vec.y * Value[1][1] + vec.z * Value[1][2] + Value[1][3];
	res.z = vec.x * Value[2][0] + vec.y * Value[2][1] + vec.z * Value[2][2] + Value[2][3];
	w     = vec.x * Value[3][0] + vec.y * Value[3][1] + vec.z * Value[3][2] + Value[3][3];
	res = res * (1.0f / w);

	return res;
}

Melon::Matrix4 Melon::Matrix4::Translate(const Vector3 pos) const
{
	Matrix4 res(1.0f);
	res.Value[0][3] = pos.x;
	res.Value[1][3] = pos.y;
	res.Value[2][3] = pos.z;
	return res ** this;
}

Melon::Matrix4 Melon::Matrix4::Rotate(Rotator rotator) const
{
	rotator.Axis = rotator.Axis.Normalize();
	Matrix4 res;
	float CosTheta = cosf(rotator.Angle);
	float SinTheta = sinf(rotator.Angle);
	auto f1 = [=](float a, float b, float c)
	{
		return a * b * (1 - CosTheta) + c * SinTheta;
	};
	auto f2 = [=](float a)
	{
		return CosTheta + a * a * (1 - CosTheta);
	};
	res.Value[0][0] = f2(rotator.Axis.x);
	res.Value[0][1] = f1(rotator.Axis.x, rotator.Axis.y, rotator.Axis.z);
	res.Value[0][2] = f1(rotator.Axis.x, rotator.Axis.z,-rotator.Axis.y);

	res.Value[1][0] = f1(rotator.Axis.y, rotator.Axis.x,-rotator.Axis.z);
	res.Value[1][1] = f2(rotator.Axis.y);
	res.Value[1][2] = f1(rotator.Axis.y, rotator.Axis.z, rotator.Axis.x);

	res.Value[2][0] = f1(rotator.Axis.z, rotator.Axis.x, rotator.Axis.y);
	res.Value[2][1] = f1(rotator.Axis.z, rotator.Axis.y,-rotator.Axis.x);
	res.Value[2][2] = f2(rotator.Axis.z);

	res.Value[3][3] = 1.0f;
	
	return res **this;
}

Melon::Matrix4 Melon::Matrix4::Scale(float scalar) const
{
	Matrix4 res(1.0f);
	for (int xy = 0; xy < 3; xy++)
	{
		res.Value[xy][xy] = scalar;
	}
	return res **this;
}

Melon::Matrix4 Melon::Matrix4::Scale(const Vector3 scalar) const
{
	Matrix4 res(1.0f);
	res.Value[0][0] = scalar.x;
	res.Value[1][1] = scalar.y;
	res.Value[2][2] = scalar.z;
	return res * *this;
}

Melon::Matrix4 Melon::Matrix4::Transpose() const
{
	Matrix4 res;
	for (int x = 0; x < 4; x++) for (int y = 0; y < 4; y++)
		res.Value[x][y] = this->Value[y][x];
	return res;
}

Melon::Matrix4 Melon::Matrix4::Perspective(float FOV, float aspect, float near, float far)
{
	Matrix4 res;
	float oa = tanf(FOV / 2);

	res.Value[0][0] = 1 / (aspect * oa);
	res.Value[1][1] = 1 / oa;
	res.Value[2][2] = -(far + near) / (far - near);
	res.Value[2][3] = -(2 * far * near) / (far - near);
	res.Value[3][2] = -1;

	return res;
}

Melon::Matrix4 Melon::Matrix4::Ortho(float aspect, float near, float far)
{
	Matrix4 res(1.0f);

	res.Value[0][0] =  1 / aspect;
	res.Value[2][2] = -2 / (far - near);

	res.Value[3][0] = 0;
	res.Value[3][1] = 0;
	res.Value[3][2] = -(far + near) / (far - near);

	return res;
}

Melon::Matrix4 Melon::Camera3D::GetView()
{
	Vector3 right = Vector3(0.0f, 1.0f, 0.0f).Cross(Direction).Normalize();
	Up = Direction.Cross(right);

	Matrix4 translation(1.0f);
	translation.Value[0][3] = -Position.x;
	translation.Value[1][3] = -Position.y;
	translation.Value[2][3] = -Position.z;
	Matrix4 rotation(1.0f);
	rotation.Value[0][0] = -right.x;
	rotation.Value[0][1] = -right.y;
	rotation.Value[0][2] = -right.z;
	rotation.Value[1][0] = Up.x;
	rotation.Value[1][1] = Up.y;
	rotation.Value[1][2] = Up.z;
	rotation.Value[2][0] = -Direction.x;
	rotation.Value[2][1] = -Direction.y;
	rotation.Value[2][2] = -Direction.z;

	return rotation * translation;
}
void Melon::Camera3D::SetDirection(Vector3 dir)
{
	Direction = dir;
	Right = Direction.Cross(Up).Normalize();
	Up = Right.Cross(Direction).Normalize();
}
Melon::Matrix4 Melon::Camera2D::GetView()
{
	Matrix4 res(1.0f);
	res = res.Translate(Vector3(-Position.x,-Position.y, 0));
	res = res.Rotate(Rotator(Rotation, Vector3(0.0f, 0.0f, 1.0f)));
	return res;
}
Melon::Matrix4 Melon::CoordinateSystem2D::TransformationTo() const
{
	Matrix4 model(1.0f);
	model = model.Translate(-Vector3(Position.x, Position.y, 0.0f));
	model = model.Rotate(Rotator(-Rotation, Vector3(0.0f, 0.0f, 1.0f)));
	model = model.Scale(Vector3(1.0f / Scale.x, 1.0f / Scale.y, 1.0f));
	return model;
}
Melon::Matrix4 Melon::CoordinateSystem2D::TransformationFrom() const
{
	Matrix4 model(1.0f);
	model = model.Scale(Vector3(Scale.x, Scale.y, 1.0f));
	model = model.Rotate(Rotator(Rotation, Vector3(0.0f, 0.0f, 1.0f)));
	model = model.Translate(Vector3(Position.x, Position.y, 0.0f));
	return model;
}
Melon::Matrix4 Melon::CoordinateSystem3D::TransformationTo() const
{
	Matrix4 model(1.0f);
	model = model.Translate(-Position);
	model = model.Rotate(Rotation);
	model = model.Scale(Vector3(1.0f / Scale.x, 1.0f / Scale.y, 1.0f/Scale.z));
	return model;
}
Melon::Matrix4 Melon::CoordinateSystem3D::TransformationFrom() const
{
	Matrix4 model(1.0f);
	model = model.Scale(Scale);
	model = model.Rotate(Rotation);
	model = model.Translate(Position);
	return model;
}