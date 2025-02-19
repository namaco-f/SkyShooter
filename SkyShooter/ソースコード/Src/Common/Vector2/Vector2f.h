#pragma once

/// <summary>
/// float�^��Vector2
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
	/// int�^��float�^�ɕϊ�����R���X�g���N�^
	/// </summary>
	Vector2f(int vX, int vY) 
	{
		x = (float)vX;
		y = (float)vY;
	};

	/// <summary>
	/// �x�N�g���̑傫�����擾
	/// </summary>
	float Magnitude(void)const;

	/// <summary>
	/// �x�N�g���̐��K��
	/// </summary>
	void Normalize(void);

	/// <summary>
	/// ���K�������x�N�g�����擾
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
	/// �����ɂ���
	/// </summary>
	Vector2f DivHalf(void);
};

Vector2f operator-(const Vector2f& va, const Vector2f vb);
Vector2f operator+(const Vector2f& va, const Vector2f vb);
Vector2f operator*(const Vector2f& va, const float scl);

/// <summary>
/// ����
/// </summary>
inline float Dot(const Vector2f& va, const Vector2f& vb);

/// <summary>
/// �O��
/// </summary>
inline float Cross(const Vector2f& va, const Vector2f& vb);
