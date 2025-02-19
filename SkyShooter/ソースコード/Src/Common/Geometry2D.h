#pragma once
#include "Vector2/Vector2f.h"
#include "Vector2/Vector2.h"


/// <summary>
/// Vector2f��Vector2�ɕϊ�
/// </summary>
Vector2 ToVector2(const Vector2f& val);

/// <summary>
/// Vector2��Vector2f�ɕϊ�
/// </summary>
Vector2f ToVector2f(const Vector2& val);

/// <summary>
/// ����
/// </summary>
float Differential(float x, float h);

/// <summary>
/// �ݏ�
/// </summary>
/// <param name="val">��</param>
/// <param name="p">�w��</param>
float Pow(float val, float p);

/// <summary>
/// 2��
/// </summary>
/// <param name="val">��</param>
float Squared(float val);


