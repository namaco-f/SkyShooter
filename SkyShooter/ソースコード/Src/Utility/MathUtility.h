#pragma once

#include <DxLib.h>
#include <string>
#include <vector>
#include "../Common/Geometry2D.h"
#include "../Common/Quaternion.h"

/// <summary>
/// 数学関連処理をまとめたクラス
/// </summary>
class MathUtility
{

public:

	static constexpr float FLOAT_ZERO = 0.0f;
	static constexpr int INT_ZERO = 0;

	//ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	//回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

	//方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//四捨五入
	static int Round(float v);

	//文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);

	//ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static VECTOR Rad2Deg_VECTOR(const VECTOR& rad);
	static int Rad2DegI(int rad);

	//度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static VECTOR Deg2Rad_VECTOR(const VECTOR& deg);
	static int Deg2RadI(int deg);

	//0～360度の範囲に収める
	static double DegIn360(double deg);

	//0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	//回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	
	//回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	
	//線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	//角度の線形補間
	static double LerpDeg(double start, double end, double t);

	//色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	//ベジェ曲線
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);
	
	//Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	//ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const Vector2& v1, const Vector2& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	//球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	//球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, const float sphRadius, 
		const VECTOR& capPos1, const VECTOR& capPos2, const float capRadius);

	//球体とカプセルの衝突判定
	static bool IsHitCapsuleCapsule(
		const VECTOR& capAPos1, const VECTOR& capAPos2, const float capARadius,
		const VECTOR& capBPos1, const VECTOR& capBPos2, const float capBRadius
	);

	//比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	//正規化
	static VECTOR Normalize(const Vector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	//2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	//描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	//VECTORのXZをVector2fに変換
	static const Vector2f VECTORToVector2f_XZ(const VECTOR& pos);
	static const Vector2 VECTORToVector2_XZ(const VECTOR& pos);

	static const VECTOR ClampVECTOR(const VECTOR& val, const VECTOR& min, const VECTOR& max);
	static const float Clamp(const float val, const float min, const float max);

	static const VECTOR IntToVECTOR(const int x, const int y, const int z);

};

