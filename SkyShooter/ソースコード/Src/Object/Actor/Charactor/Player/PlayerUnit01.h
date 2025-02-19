#pragma once

#include "PlayerBase.h"
#include "../../Shot/Shot_CreateData.h"

/// <summary>
/// 自機01
/// </summary>
class PlayerUnit01 :
    public PlayerBase
{

public:

    PlayerUnit01(void);
    virtual ~PlayerUnit01(void);

protected:

    //初期化時の読込
    void InitLoad(void) override;

    //モデル制御情報の初期化
    void InitTransform(void) override;

    //コライダの初期化
    void InitCollider(void) override;

    void InitParameter(void) override;

    //レンダーの初期化
    void InitRender(void) override;

    //全ての初期化終了後の処理
    void InitEnd(void) override;

    //サウンド初期化
    void InitSound(void) override;

    //
    void Destroy(void) override;

    //アニメーションの初期化
    void InitAnimation(void) override;

    virtual void IsHit_EnemyShot1(void) override;        //敵の弾と衝突
    virtual void IsHit_EnemyMissile(void) override;      //敵の弾と衝突
    virtual void IsHit_EnemyBox(void) override;          //箱攻撃と衝突

    //重力無効中か
    virtual const bool InvalidGravity(void) const override;

private:


    //通常弾 : バースト
    ShotData standardShot_;

    //ミサイル
    ShotData missile_;

    //カウンター弾
    ShotData counterShot_;

    //シールド時間カウント用
    float shieldTimeCnt_;

    //シールド時間上限
    float shieldTimeLimit_;

    //シールド時間上限カウント用
    float shieldTimeLimitCnt_;

    //シールドクールタイム
    float shieldTimeCool_;

    //シールド反撃の数
    int counterCnt_;

    //状態ごとの更新
    void UpdateState(void) override;
    void UpdateCreateShot(void) override;

    virtual void Run(void) override;
    virtual void Dash(void) override;
    virtual void Idle(void) override;
    virtual void Attack_Left(void) override;
    virtual void Attack_Right(void) override;
    virtual void Shield(void) override;
    virtual void UltAttack(void) override;


    //通常弾の生成
    void CreateUltAttack(void);

    //状態遷移時に実行される関数
    void Change_Idle(void);
    void Change_Move(void);
    void Change_Dash(void);
    void Change_Shot1(void);
    void Change_Shot2(void);
    void Change_Destroy(void);
    void Change_Shield(void);
    void Change_ShieldCounterAttack(void);
    void Change_UltCharge(void);
    void Change_UltAttack(void);
    void Change_End(void);

    //状態ごとの更新処理
    void Update_Idle(void);
    void Update_Move(void);
    void Update_Dash(void);
    void Update_Destroy(void);
    void Update_Shield(void);
    void Update_ShieldCounterAttack(void);
    void Update_UltCharge(void);
    void Update_UltAttack(void);
    void Update_End(void);

    virtual void Message_UltAttack(void);
    virtual void Message_UltEnd(void);
    virtual void Message_Dash(void);
    virtual void Message_DashEnd(void);

    //状態ごとの状態遷移を追加
    virtual void AddStateChange(void) override;
    virtual void AddAttackStateChange(void) override;

    //開始前に確認できるエラーがあるか
    const bool IsErrorUpdate(void) const override;

    //シールドが使用可能
    const bool AbleUseSheild(void) const;

    //衝突時処理の登録
    virtual void InitIsHitUpdate(void) override;

    //メッセージ受信時処理の登録
    virtual void InitCommonMessageUpdate(void) override;

    //アクション通知時の処理の登録
    virtual void InitActionUpdate(void) override;
    
    //カウンターカウントを追加
    virtual void CounterCount(void);

    virtual void InitUpdateObserver(void) override;

};

