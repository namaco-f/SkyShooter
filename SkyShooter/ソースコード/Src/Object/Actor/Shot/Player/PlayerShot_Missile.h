#pragma once
#include "../Type/Missile.h"

/// <summary>
/// プレイヤーのミサイル弾
/// </summary>
class PlayerShot_Missile 
    :
    public Missile
{

public:

    PlayerShot_Missile(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="trans">モデル制御情報</param>
    explicit PlayerShot_Missile(const Transform& trans);
    virtual ~PlayerShot_Missile(void);

protected:

    //リソースの読込
    virtual void InitLoad(void) override;

    //モデル制御情報の初期化
    virtual void InitTransform(void) override;

    //衝突判定の初期化
    virtual void InitCollider(void) override;

    //レンダラーの初期化
    virtual void InitRender(void) override;

    //全ての初期化終了後の処理
    virtual void InitEnd(void) override;

    virtual void Change_None(void);     //通常
    virtual void Change_Blast(void);    //爆発
    virtual void Change_End(void);      //終了

    virtual void Update_None(void);     //通常
    virtual void Update_Blast(void);    //爆発
    virtual void Update_End(void);      //終了

    //敵と衝突
    void HitEnemy(void) override;

    //状態変化時の関数の登録
    virtual void AddStateChange(void) override;

private:
};

