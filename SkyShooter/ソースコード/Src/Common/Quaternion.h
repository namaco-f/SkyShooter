#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>

/// <summary>
/// クォータニオン
/// </summary>
class Quaternion
{
public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	Quaternion(void);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="rad">ラジアン角</param>
	explicit Quaternion(const VECTOR& rad);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="wxyz">クォータニオン</param>
	Quaternion(double w, double x, double y, double z);

	virtual ~Quaternion(void);

	/// <summary>
	/// オイラー角からクォータニオンへ変換
	/// </summary>
	static Quaternion Euler(const VECTOR& rad);
	
	/// <summary>
	/// オイラー角からクォータニオンへ変換
	/// </summary>
	static Quaternion Euler(double radX, double radY, double radZ);

	/// <summary>
	/// クォータニオンの合成
	/// </summary>
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);

	/// <summary>
	/// クォータニオンの合成
	/// </summary>
	Quaternion Mult(const Quaternion& q) const;

	/// <summary>
	/// 指定軸を指定角分、回転させる
	/// </summary>
	static Quaternion AngleAxis(double rad, VECTOR axis);

	/// <summary>
	/// 座標を回転させる
	/// </summary>
	static VECTOR PosAxis(const Quaternion& q, VECTOR pos);

	/// <summary>
	/// 座標を回転させる
	/// </summary>
	VECTOR PosAxis(VECTOR pos) const;

	/// <summary>
	/// クォータニオンからオイラー角へ変換
	/// </summary>
	static VECTOR ToEuler(const Quaternion& q);

	/// <summary>
	/// クォータニオンからオイラー角へ変換
	/// </summary>
	VECTOR ToEuler(void) const;

	/// <summary>
	/// クォータニオンから行列へ変換
	/// </summary>
	static MATRIX ToMatrix(const Quaternion& q);

	/// <summary>
	/// クォータニオンから行列へ変換
	/// </summary>
	MATRIX ToMatrix(void) const;

	/// <summary>
	/// ベクトルからクォータニオンに変換
	/// </summary>
	static Quaternion LookRotation(VECTOR dir);

	/// <summary>
	/// ベクトルからクォータニオンに変換
	/// </summary>
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	/// <summary>
	/// 行列からクォータニオンに変換
	/// </summary>
	static Quaternion GetRotation(MATRIX mat);

	/// <summary>
	/// 前方ベクトルを取得
	/// </summary>
	VECTOR GetForward(void) const;

	/// <summary>
	/// 後方ベクトルを取得
	/// </summary>
	VECTOR GetBack(void) const;

	/// <summary>
	/// 右方ベクトルを取得
	/// </summary>
	VECTOR GetRight(void) const;

	/// <summary>
	/// 左方ベクトルを取得
	/// </summary>
	VECTOR GetLeft(void) const;

	/// <summary>
	/// 上方ベクトルを取得
	/// </summary>
	VECTOR GetUp(void) const;

	/// <summary>
	/// 下方ベクトルを取得
	/// </summary>
	VECTOR GetDown(void) const;

	/// <summary>
	/// 内積
	/// </summary>
	static double Dot(const Quaternion& q1, const Quaternion& q2);

	/// <summary>
	/// 内積
	/// </summary>
	double Dot(const Quaternion& b) const;

	/// <summary>
	/// 正規化
	/// </summary>
	static Quaternion Normalize(const Quaternion& q);

	/// <summary>
	/// 正規化
	/// </summary>
	Quaternion Normalized(void) const;

	/// <summary>
	/// 正規化
	/// </summary>
	void Normalize(void);

	/// <summary>
	/// 逆クォータニオン
	/// </summary>
	Quaternion Inverse(void) const;

	/// <summary>
	/// 球面補間
	/// </summary>
	static Quaternion Slerp(const Quaternion from, const Quaternion to, double t);

	/// <summary>
	/// 球面補間
	/// </summary>
	static Quaternion Slerp(const Quaternion from, const Quaternion to, float t);

	/// <summary>
	/// ２つのベクトル間の回転量を取得する
	/// </summary>
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity(void);

	double Length(void) const;
	double LengthSquared(void) const;
	VECTOR xyz(void) const;

	/// <summary>
	/// 対象方向の回転
	/// </summary>
	void ToAngleAxis(float* angle, VECTOR* axis);

private:

	//基本ベクトルを取得
	VECTOR GetDir(VECTOR dir) const;

	const Quaternion operator*(const float& rhs);
	const Quaternion operator+(const Quaternion& rhs);
};
