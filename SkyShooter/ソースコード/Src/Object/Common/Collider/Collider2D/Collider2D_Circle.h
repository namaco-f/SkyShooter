#pragma once
#include "Collider2D.h"


/// <summary>
/// 円の2Dコライダ
/// </summary>
class Collider2D_Circle :
    public Collider2D
{

public:

	Collider2D_Circle(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="colData">コライダ情報</param>
	/// <param name="colliderDef">コライダを有効化するか</param>
	explicit Collider2D_Circle(const ColliderData_2D& colData, const bool colliderDef = true);
	virtual ~Collider2D_Circle(void);

	/// <summary>
	/// コライダ描画処理
	/// </summary>
	virtual void Draw(void)override;

private:
};

