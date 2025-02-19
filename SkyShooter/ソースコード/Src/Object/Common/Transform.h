#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：VECTOR基準
/// 回転　：Quaternion基準
/// 位置　：VECTOR基準
/// </summary>


/// <summary>
/// モデル制御情報
/// </summary>
class Transform
{

public:

	/// <summary>
	/// モデルのハンドルID
	/// </summary>
	int modelId_;

	/// <summary>
	/// 大きさ
	/// </summary>
	VECTOR scl;
	
	/// <summary>
	/// 回転(デグリー)
	/// </summary>
	VECTOR rot;
	
	/// <summary>
	/// 位置
	/// </summary>
	VECTOR pos;
	VECTOR localPos;

	/// <summary>
	/// 拡大行列
	/// </summary>
	MATRIX matScl;

	/// <summary>
	/// 回転行列
	/// </summary>
	MATRIX matRot;

	/// <summary>
	/// 座標行列
	/// </summary>
	MATRIX matPos;

	/// <summary>
	/// 回転(クォータニオン)
	/// </summary>
	Quaternion quaRot;

	/// <summary>
	/// ローカル回転(クォータニオン)
	/// </summary>
	Quaternion quaRotLocal;
	

	Transform(void);
	explicit Transform(const int model);
	~Transform(void);

	/// <summary>
	/// モデル制御の基本情報更新
	/// </summary>
	void Update(void);

	////モデルの向き描画
	//void DrawDirection(float len = DEBUG_DIR_LEN);

	/// <summary>
	/// モデルを設定
	/// </summary>
	/// <param name="modelHId">対象のモデルのハンドルID</param>
	void SetModel(const int modelHId);

	/// <summary>
	/// 前方方向を取得
	/// </summary>
	VECTOR GetForward(void) const;

	/// <summary>
	/// 後方方向を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	VECTOR GetBack(void) const;

	/// <summary>
	/// 右方向を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	VECTOR GetRight(void) const;

	/// <summary>
	/// 左方向を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	VECTOR GetLeft(void) const;

	/// <summary>
	/// 上方向を取得
	/// </summary>
	VECTOR GetUp(void) const;

	/// <summary>
	/// 下方向を取得
	/// </summary>
	VECTOR GetDown(void) const;

	/// <summary>
	/// 指定した方向を取得
	/// </summary>
	/// <param name="vec">指定する方向</param>s
	VECTOR GetDir(const VECTOR& vec) const;

	/// <summary>
	/// 指定したフレームの座標を取得
	/// </summary>
	/// <param name="frameNo">指定するフレーム</param>
	/// <returns>フレームの座標</returns>
	const VECTOR GetPosFrame(const int& frameNo) const;

private:

};
