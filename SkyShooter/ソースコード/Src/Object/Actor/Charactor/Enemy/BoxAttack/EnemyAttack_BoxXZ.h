#pragma once
#include "EnemyAttack_Box.h"

/// <summary>
/// XZ平面上に矩形が生成される箱攻撃
/// </summary>
class EnemyAttack_BoxXZ :
    public EnemyAttack_Box
{
public:

    EnemyAttack_BoxXZ(void) = default;
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="transform">モデルの制御情報</param>
	/// <param name="boxSize">縦横サイズ</param>
	/// <param name="boxAttackData">攻撃の情報</param>
	/// <param name="boxType">矩形の種別</param>
	EnemyAttack_BoxXZ(
		const Transform& transform,
		const Vector2f& boxSize,
		const BoxAttackData& boxAttackData
	);

    virtual ~EnemyAttack_BoxXZ(void);

protected:

	virtual void InitEffect(void) override;

	virtual void AttackEffectDraw(void) override;

	//2Dコライダの生成
	virtual void MakeBoxCollider(void) override;

	//モデルのスケールを取得
	virtual const VECTOR GetBoxScale(void) override;

	virtual const VECTOR GetBoxChangeScale(void) override;

};

