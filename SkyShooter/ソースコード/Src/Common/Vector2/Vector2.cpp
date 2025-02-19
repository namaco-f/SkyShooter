#include <Dxlib.h>
#include <cmath>
#include "Vector2.h"

void Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
}

void Vector2::operator+=(const int v)
{
	x += v;
	y += v;
}

void Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;

}

void Vector2::operator*=(const int scl)
{
	x *= scl;
	y *= scl;
}

Vector2 Vector2::operator*(const int scl)
{
	return Vector2(x * scl, y * scl);
}

Vector2 Vector2::operator/(const int div)
{
	if (div == 0)
	{
		//0èúéZ
		return Vector2(x, y);
	}

	return Vector2(x / div, y / div);
}

Vector2 operator-(const Vector2& va, const Vector2 vb)
{
	return Vector2(va.x - vb.x, va.y - vb.y);
}

Vector2 operator+(const Vector2& va, const Vector2 vb)
{
	return Vector2(va.x + vb.x, va.y + vb.y);
}