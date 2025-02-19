#pragma once

#include <list>
#include "../../Common/Transform.h"


/// <summary>
/// トレイル
/// </summary>
class Trail
{

public:

	Trail(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="trans">追従対象の制御情報</param>
	Trail(const Transform& trans,const VECTOR& color);
	~Trail(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="trans">追従対象の制御情報</param>
	void Update(const Transform& trans);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	//void CalculateBillboard(void);

private:

	ShaderManager& shaderMng_;

	//追従している座標
	VECTOR transformPos_;

	//トレイルの座標配列
	std::list<VECTOR> trailPosList_;

	//色
	COLOR_U8 diffuseColor_;

	//トレイルのテクスチャ
	int tex_;

	//トレイル配列の最大数
	int trailNumMax_;

	//トレイルの長さ
	float trailLength_;

	//トレイル関連の計算
	void CalculateTrail(void);

	//トレイル用座標の追加
	void AddTrailPosList(const VECTOR& addTrailPos);

	void GetColorVECTOR(const VECTOR& color);

};

