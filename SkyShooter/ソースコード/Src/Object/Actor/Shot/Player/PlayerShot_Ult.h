#pragma once

#include "../Type/UltBase.h"

/// <summary>
/// 自機のウルト
/// </summary>
class PlayerShot_Ult :
    public UltBase
{
public:

    PlayerShot_Ult(void) = default;
    virtual ~PlayerShot_Ult(void) = default;

    explicit PlayerShot_Ult(const Transform& trans);

    /// <summary>
    /// 描画処理
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// 弾発射の座標設定
    /// </summary>
    void InitShotStartPos(void) override;


protected:

    float step_;

    //初期化時の読込
    void InitLoad(void) override;

    //モデル制御情報の初期化
    void InitTransform(void) override;

    //衝突判定の初期化
    void InitCollider(void) override;

    //全ての初期化終了後の処理
    void InitEnd(void) override;

    virtual void InitEffect(void);

    //敵に衝突している
    virtual void HitEnemy(void) override;

    //デバッグ用描画
    virtual void DrawDebugModel(void);

    //状態遷移を追加
    virtual void AddStateChange(void) override;

    virtual void Change_None(void);     //通常
    virtual void Change_Attack(void);   //攻撃中
    virtual void Change_Blast(void);    //爆発
    virtual void Change_End(void);      //終了

    virtual void Update_None(void);     //通常
    virtual void Update_Attack(void);   //攻撃中
    virtual void Update_Blast(void);    //爆発
    virtual void Update_End(void);      //終了   

private:

    //発射位置
    virtual const VECTOR CalucuratePos(void);

    //プレイヤーの座標を取得
    virtual const Transform& GetPlayerTransform(void);

    //敵の座標を取得
    virtual const Transform& GetEnemyTransform(void);

    //自機から敵へのベクトルから求めた回転と発射座標を取得
    virtual const Transform LookRotateTransform(void);

    //モデル制御情報の座標とクォータニオンを更新
    virtual void TransformUpdate_PosQua(const Transform& trans);
    
    //プレイヤーにメッセージ
    virtual void SendPlayerMessage(const Common_Message message);
};

