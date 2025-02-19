#pragma once

/// <summary>
/// intŒ^‚ÌVector2
/// </summary>
struct Vector2
{
	int x;
	int y;

	Vector2() 
		:
		x(0),
		y(0) 
	{};

	Vector2(int vX, int vY) 
		:
		x(vX),
		y(vY)
	{};

	/// <summary>
	/// floatŒ^‚ðintŒ^‚É•ÏŠ·‚·‚éƒRƒ“ƒXƒgƒ‰ƒNƒ^
	/// </summary>
	Vector2(float vX, float vY) 
	{
		x = static_cast<int>(vX);
		y = static_cast<int>(vY);
	};

	void operator+=(const Vector2& v);
	void operator+=(const int v);
	void operator-=(const Vector2& v);
	void operator*=(const int scl);

	Vector2 operator* (const int scl);
	Vector2 operator/ (const int div);
};

Vector2 operator-(const Vector2& va, const Vector2 vb);

Vector2 operator+(const Vector2& va, const Vector2 vb);
