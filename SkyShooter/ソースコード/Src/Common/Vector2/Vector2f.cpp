
#include <Dxlib.h>
#include <cmath>
#include "Vector2f.h"

const float DIV_HALF = 0.5f;

//ベクトルの大きさを返す
float Vector2f::Magnitude() const
{
	return hypot(x, y);
}

//ベクトルの正規化
void Vector2f::Normalize()
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}

//正規化ベクトルを返す
Vector2f Vector2f::Normalized()
{
	float mag = Magnitude();

	return Vector2f(x /= mag, y /= mag);
}

void Vector2f::operator+=(const Vector2f& v)
{
	x += v.x;
	y += v.y;
}

void Vector2f::operator-=(const Vector2f& v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2f::operator*=(const float scl)
{
	x *= scl;
	y *= scl;
}

void Vector2f::operator*=(const Vector2f& v)
{
	x *= v.x;
	y *= v.y;
}

Vector2f Vector2f::operator*(const float scl)
{
	return Vector2f(x * scl, y * scl);
}

Vector2f Vector2f::operator*(const Vector2f& scl)
{
	return Vector2f(x * scl.x, y * scl.y);
}

Vector2f Vector2f::operator/(const float scl)
{
	if (scl == 0.0000f)
	{
		//0除算エラー
		return Vector2f(x, y);
	}

	return Vector2f(x / scl, y / scl);
}

Vector2f Vector2f::DivHalf(void)
{
	return Vector2f(x * DIV_HALF, y * DIV_HALF);
}

Vector2f operator-(const Vector2f& va, const Vector2f vb)
{
	return Vector2f(va.x - vb.x, va.y - vb.y);
}

Vector2f operator+(const Vector2f& va, const Vector2f vb)
{
	return Vector2f(va.x + vb.x, va.y + vb.y);
}

Vector2f operator*(const Vector2f& va, const float scl)
{
	return Vector2f(va.x * scl, va.y * scl);
}

//内積
float Dot(const Vector2f& va, const Vector2f& vb)
{
	return va.x * vb.x + va.y * vb.y;
}

//外積
float Cross(const Vector2f& va, const Vector2f& vb)
{
	return va.x * vb.y - va.y * vb.x;
}
