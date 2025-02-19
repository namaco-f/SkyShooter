#pragma once
#include "Collider2D_Box.h"

/// <summary>
/// ZY平面上の矩形の2Dコライダ
/// </summary>
class Collider2D_BoxZY :
    public Collider2D_Box
{

public:

	Collider2D_BoxZY(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="colData">2Dコライダ情報</param>
	/// <param name="colliderDef">コライダを有効化するか</param>
	explicit Collider2D_BoxZY(const ColliderData_2D& colData, const bool colliderDef = true);
	virtual ~Collider2D_BoxZY(void);

protected:

	//Vector2fをVECTORに変換
	virtual const VECTOR Vector2fToVECTOR(const Vector2f& pos) override;

	//矩形の座標を計算
	virtual void CalculateBoxPos(void) override;

	//コライダ描画用
	virtual const VECTOR GetDirection(void) const override;

};

