#pragma once
#include "Vector2/Vector2f.h"
#include "Vector2/Vector2.h"


/// <summary>
/// Vector2f‚ğVector2‚É•ÏŠ·
/// </summary>
Vector2 ToVector2(const Vector2f& val);

/// <summary>
/// Vector2‚ğVector2f‚É•ÏŠ·
/// </summary>
Vector2f ToVector2f(const Vector2& val);

/// <summary>
/// ”÷•ª
/// </summary>
float Differential(float x, float h);

/// <summary>
/// —İæ
/// </summary>
/// <param name="val">’ê</param>
/// <param name="p">w”</param>
float Pow(float val, float p);

/// <summary>
/// 2æ
/// </summary>
/// <param name="val">’ê</param>
float Squared(float val);


