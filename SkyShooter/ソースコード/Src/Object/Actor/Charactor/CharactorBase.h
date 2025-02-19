#pragma once
#include <memory>
#include <unordered_map>
#include "../ActorBase.h"

class Collider;
class Collision;
class AnimationController;
class ShotBase;
class EnemyAttack_Box;
class EnemyAttack_Boxes;

/// <summary>
/// キャラクター基底クラス
/// </summary>
class CharactorBase
	: public ActorBase
{

public:
	CharactorBase(void);
    virtual ~CharactorBase(void) override = default;

    /// <summary>
    /// ダメージ
    /// </summary>
    /// <param name="damage">ダメージ量</param>
    virtual void Damage(const int damage);

    /// <summary>
    /// 生存判定
    /// </summary>
    /// <returns>
    /// true : 生存
    /// false : 生存していない
    /// </returns>
    virtual const bool IsAlive(void) const override;

    /// <summary>
    /// 重力の更新
    /// </summary>
    void UpdateGravity(void);

    /// <summary>
    /// 敵の攻撃の取得
    /// </summary>
    const std::list<std::shared_ptr<EnemyAttack_Box>>& GetAttackBox(void) const;

protected:

    //アニメーションコントローラー
    std::unique_ptr<AnimationController> animationController_;

    std::unique_ptr<Collision> collision_;

    //hp
    int hp_;

    //hp最大値
    int hpMax_;   

    std::unique_ptr<EnemyAttack_Boxes> enemyAttackboxes_;

    //重力
    VECTOR gravityPow_;

    //hp下限以下
    virtual void Destroy(void) = 0;

    //アニメーションの設定
    virtual void InitAnimation(void) = 0;

    virtual void IsHitGround(void);

private:
};

