
#include <Dxlib.h>
#include <cmath>
#include "Vector2f.h"

const float DIV_HALF = 0.5f;

//�x�N�g���̑傫����Ԃ�
float Vector2f::Magnitude() const
{
	return hypot(x, y);
}

//�x�N�g���̐��K��
void Vector2f::Normalize()
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}

//���K���x�N�g����Ԃ�
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
		//0���Z�G���[
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

//����
float Dot(const Vector2f& va, const Vector2f& vb)
{
	return va.x * vb.x + va.y * vb.y;
}

//�O��
float Cross(const Vector2f& va, const Vector2f& vb)
{
	return va.x * vb.y - va.y * vb.x;
}
