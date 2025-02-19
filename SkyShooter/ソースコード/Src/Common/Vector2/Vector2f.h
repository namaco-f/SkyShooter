#pragma once

/// <summary>
/// float型のVector2
/// </summary>
struct Vector2f
{
	float x;
	float y;

	Vector2f(void) 
		:
		x(0.0f),
		y(0.0f) 
	{};

	Vector2f(float vX, float vY) 
		:
		x(vX),
		y(vY)
	{};

	/// <summary>
	/// int型をfloat型に変換するコンストラクタ
	/// </summary>
	Vector2f(int vX, int vY) 
	{
		x = (float)vX;
		y = (float)vY;
	};

	/// <summary>
	/// ベクトルの大きさを取得
	/// </summary>
	float Magnitude(void)const;

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	void Normalize(void);

	/// <summary>
	/// 正規化したベクトルを取得
	/// </summary>
	Vector2f Normalized(void);

	void operator+=(const Vector2f& v);
	void operator-=(const Vector2f& v);
	void operator*=(const float scl);
	void operator*=(const Vector2f& v);

	Vector2f operator*(const float scl);
	Vector2f operator*(const Vector2f& scl);
	Vector2f operator/(const float scl);


	/// <summary>
	/// 半分にする
	/// </summary>
	Vector2f DivHalf(void);
};

Vector2f operator-(const Vector2f& va, const Vector2f vb);
Vector2f operator+(const Vector2f& va, const Vector2f vb);
Vector2f operator*(const Vector2f& va, const float scl);

/// <summary>
/// 内積
/// </summary>
inline float Dot(const Vector2f& va, const Vector2f& vb);

/// <summary>
/// 外積
/// </summary>
inline float Cross(const Vector2f& va, const Vector2f& vb);
