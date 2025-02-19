#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "Collider2DTag.h"
#include "../../Transform.h"
#include "../Common/Vector2/Vector2f.h"

/// <summary>
/// 2Dコライダの制御情報
/// </summary>
struct ColliderData_2D
{
	VECTOR colCenterPos;		//中心座標
	Vector2f size;				//横縦サイズ
	float radius;				//半径
};

/// <summary>
/// 2Dコライダの基底クラス
/// </summary>
class Collider2D
{
public:

	Collider2D(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="colData">コライダ情報</param>
	explicit Collider2D(const ColliderData_2D& colData,const bool colliderDef = true);
	virtual ~Collider2D(void);

	/// <summary>
	/// コライダの描画
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// 制御情報の更新
	/// </summary>
	/// <param name="trans">制御情報</param>
	virtual void UpdateTransform(const Transform& trans);

	/// <summary>
	/// コライダの種別を取得
	/// </summary>
	/// <returns>colType_</returns>
	virtual inline const COLLISION2D_TYPE& GetColliderType(void) const
	{
		return colType_;
	}

	/// <summary>
	/// コライダの情報を取得
	/// </summary>
	/// <returns>colData_</returns>
	virtual inline const ColliderData_2D& GetColliderData(void) const
	{
		return colData_;
	}

	/// <summary>
	/// コライダの状態を反転
	/// </summary>
	virtual inline void SwitchAbleCollider(void)
	{
		isAbleCollider_ = !isAbleCollider_;
	}

	/// <summary>
	/// コライダ状態を取得
	/// </summary>
	/// <returns>isAbleCollider_</returns>
	virtual inline const bool IsAbleCollider(void) const
	{
		return isAbleCollider_;
	}

protected:

	//コライダの種別
	COLLISION2D_TYPE colType_;

	//コライダの情報
	ColliderData_2D colData_;

	//コライダが有効化されているか
	bool isAbleCollider_;

};

