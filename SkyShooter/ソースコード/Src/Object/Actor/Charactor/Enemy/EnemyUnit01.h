#pragma once
#include "EnemyBase.h"

/// <summary>
/// 敵01
/// </summary>
class EnemyUnit01 :
    public EnemyBase
{

public:

    /// <summary>
    /// 1トリガーごとの弾のデータ
    /// </summary>
    struct ShotData
    {
        float shotDelay_;       //弾1発ごとのディレイ
        float shotDelayStep_;       //ディレイのカウント用
        int shotNumMax_;        //1トリガーで発射出来る弾数
        int shotNum_;       //弾のカウント用

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="delayTime">弾1発ごとのディレイ</param>
        /// <param name="shotNumMax">1トリガーで発射出来る弾数</param>
        ShotData(const float delayTime, const int shotNumMax)
            :
            shotDelay_(delayTime),
            shotDelayStep_(delayTime),
            shotNumMax_(shotNumMax),
            shotNum_(0) {};

        /// <summary>
        /// カウント初期化
        /// </summary>
        void Init(void)
        {
            shotDelayStep_ = 0.0f;
            shotNum_ = 0;
        }

        /// <summary>
        /// 弾のディレイが終わっているか
        /// </summary>
        /// <returns>
        /// true : 終わっている
        /// false : 終わっていない
        /// </returns>
        const bool IsEndShotDelay(void) const
        {
            return shotDelayStep_ > shotDelay_;
        }

        /// <summary>
        /// 弾生成可能か
        /// </summary>
        /// <returns>
        /// true : 可能
        /// false : 弾数が上限
        /// </returns>
        const bool IsAbleShot(void) const
        {
            return shotNumMax_ > shotNum_;
        }
    };

    EnemyUnit01(void);
    virtual ~EnemyUnit01(void);

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw(void) override;

protected:
    //ミサイル弾
    ShotData missile_;

    //読込処理
    virtual void InitLoad(void) override;

    //モデル制御情報の初期化
    virtual void InitTransform(void) override;

    //コライダ情報の初期化
    virtual void InitCollider(void) override;

    //レンダーの初期化
    virtual void InitRender(void) override;
    
    //全ての処理終了時に行う初期化
    virtual void InitEnd(void) override;

    //アニメーションの初期化
    virtual void InitAnimation(void) override;

    //音初期化
    virtual void InitSound(void) override;

    //エフェクト初期化
    virtual void InitEffect(void) override;

    //破壊時の処理
    virtual void Destroy(void) override;

    //プレイヤーの左クリック弾に被弾
    virtual void HitPlayerShot1(void) override;

    //プレイヤーの右クリック弾に被弾
    virtual void HitPlayerShot2(void) override;

    virtual void HitPlayerShotUlt(void) override;

    virtual void InitShadow(void) override;

private:

    //ワープ時間カウント用
    float warpCnt_;

    //ワープ地点の座標
    VECTOR warpPos_;

    //ワープ準備が完了したか
    bool warpPosReady_;

    /// <summary>
    /// 事前に更新処理可能か確認
    /// </summary>
    /// <returns>
    /// true : 状態ごとの更新処理か状態がエラー
    /// </returns>
    const bool IsErrorUpdate(void) const override;

    void DrawAttackBox(void);

    /// <summary>
    /// 状態ごとの更新
    /// </summary>
    void UpdateState(void) override;

    void Change_None(void);
    void Change_Destroy(void);
    void Change_End(void);

    //状態ごとの更新処理
    std::function<void(void)> stateUpdate_;
    void Update_None(void);             //通常
    void Update_Destroy(void);          //破壊時
    void Update_End(void);              //終了

    //状態ごとの状態遷移を追加
    void AddStateChange(void);

    //攻撃の状態変化時に行われる関数
    virtual void AttackStateChange_None(void);          //攻撃していない
    virtual void AttackStateChange_CoolTime(void);      //クールタイム中
    virtual void AttackStateChange_Missile(void);       //ミサイル弾
    virtual void AttackStateChange_XYBox(void);         //XY箱攻撃
    virtual void AttackStateChange_XZBox(void);         //XZ箱攻撃
    virtual void AttackStateChange_ZYBox(void);         //ZY箱攻撃

    //攻撃の状態ごとに行われる関数
    virtual void AttackStateUpdate_None(void);          //攻撃していない
    virtual void AttackStateUpdate_CoolTime(void);      //クールタイム中
    virtual void AttackStateUpdate_Missile(void);       //ミサイル弾
    virtual void AttackStateUpdate_Box(void);           //箱攻撃

    //攻撃の状態遷移を登録
    virtual void AddAttackStateChange(void) override;

    //移動パターンの登録
    virtual void AddMovePattern(void) override;

    virtual void Change_Warp(void) override;

    virtual void Move_Warp(void) override;

    //ミサイル弾の生成
    virtual void CreateMissle(void);

    virtual void WarpLerp(void);

    virtual VECTOR GetAttackPosFromTable(const std::string& dataName,const int tableNo);

    virtual void InitIsHitUpdate(void) override;

};

