#pragma once

#include <functional>
#include "../CharactorBase.h"

class ShotManager;
class CircleShadow;

/// <summary>
/// 敵基底クラス
/// </summary>
class EnemyBase :
    public CharactorBase
{

public:

    /// <summary>
    /// 敵の状態
    /// </summary>
    enum class State
    {
        None,       //初期値またはエラー値
        Destroy,
        End,
        MAX,        //状態の数
    };

    /// <summary>
    /// 移動パターン
    /// </summary>
    enum class MOVE_PATTERN
    {
        None,           //移動しない
        Start,          //開始時
        Standard,       //直進
        Octagon,        //八角形
        PlayerDir,      //プレイヤーに向けて移動
        Warp,           //ワープ
        MAX,
    };

    /// <summary>
    /// 攻撃パターン
    /// </summary>
    enum class AttackState
    {
        None,           //攻撃しない
        CoolTime,       //クールタイム中
        XZBox,          //XZ平面箱攻撃
        ZYBox,          //ZY平面箱攻撃
        XYBox,          //XY平面箱攻撃         
        Missile,        //ミサイル弾
        MAX,
    };

    EnemyBase(void);
    virtual ~EnemyBase(void);

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name=""></param>
    virtual void Update(void) override final;

    //void IsHitNotice(const OBJECT_TYPE obj) override;

    /// <summary>
    /// 状態遷移
    /// </summary>
    virtual void ChangeState(const State state);

    /// <summary>
    /// 移動パターンの遷移
    /// </summary>
    /// <param name="movePattern"></param>
    virtual void ChangeMovePattern(const MOVE_PATTERN movePattern);

    /// <summary>
    /// 攻撃パターンの遷移
    /// </summary>
    /// <param name="attackState"></param>
    virtual void ChangeAttackState(const AttackState attackState);

protected:

    //弾
    ShotManager& shotMng_;

    //敵の状態
    State state_;

    //移動状態
    MOVE_PATTERN movePattern_;

    //攻撃状態
    AttackState attackState_;

    //影
    std::unique_ptr<CircleShadow> circleShadow_;

    //移動中の時間カウント用
    float moveTimeCnt_;

    //回転の時間カウント用
    float moveTurnCnt_;

    //攻撃終了後のクールタイム
    float attackCoolTime_;

    //クールタイム終了後攻撃開始までのディレイ
    float attackCoolTimeDelay_;

    //デルタタイム
    float deltaTime_;

    //回転時間
    float turnTime_;

    //移動状態変化時の回転を格納
    Quaternion preQuaRot;

    //ワープ中透明状態時間
    float invCnt_;

    /// <summary>
    /// 更新処理可能か確認
    /// </summary>
    /// <returns>
    /// true : 更新処理が不可能
    /// false : 更新処理可能
    /// </returns>
    virtual const bool IsErrorUpdate(void) const = 0;

    /// <summary>
    /// 更新開始前の設定
    /// </summary>
    virtual void UpdateBefore(void);

    /// <summary>
    /// 攻撃の更新
    /// </summary>
    virtual void UpdateAttack(void);

    /// <summary>
    /// 状態ごとの更新
    /// </summary>
    virtual void UpdateState(void) = 0;

    /// <summary>
    /// 回転処理
    /// </summary>
    virtual void UpdateRotate(void);

    /// <summary>
    /// 移動処理
    /// </summary>
    virtual void UpdatePos(void);

    /// <summary>
    /// Transformの更新
    /// </summary>
    virtual void UpdateTransform(void);

    /// <summary>
    /// デバッグ描画
    /// </summary>
    virtual void DrawDebug(void);
    
    //hp下限以下
    virtual void Destroy(void) override;

    //プレイヤーの攻撃1と衝突
    void IsHit_PlayerShot1(void);       
    void IsHit_PlayerShot2(void);       

    //プレイヤーの攻撃1と衝突した際の処理
    virtual void HitPlayerShot1(void) = 0;
    virtual void HitPlayerShot2(void) = 0;
    virtual void HitPlayerShotUlt(void) = 0;

    //状態遷移時に実行される関数
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //移動パターンの変更
    std::unordered_map<MOVE_PATTERN, std::function<void(void)>> movePatternChange_;
    virtual void Change_Start(void);        //スタート時
    virtual void Change_Standard(void);     //通常
    virtual void Change_Circle(void);       //円形
    virtual void Change_Octagon(void);      //八角
    virtual void Change_PlayerDir(void);    //プレイヤー方向へ前進
    virtual void Change_Warp(void);         //ワープ

    //パターンごとの移動
    std::function<void(void)> patternMove_;
    virtual void Move_Standard(void);       //通常
    virtual void Move_Circle(void);         //円形
    virtual void Move_Octagon(void);        //八角
    virtual void Move_PlayerDir(void);      //八角
    virtual void Move_Start(void);          //ゲームスタート時
    virtual void Move_Warp(void);          //ゲームスタート時

    //移動パターンの登録
    virtual void AddMovePattern(void) = 0;

    //攻撃の状態変化時に実行される関数
    std::unordered_map<AttackState, std::function<void(void)>> attackStateChange_;

    //攻撃の状態ごとに実行される関数
    std::function<void(void)> attackStateUpdate_;

    //攻撃状態の登録
    virtual void AddAttackStateChange(void) = 0;

    //state_がエラー値か
    virtual const bool IsStateError(void) const;

    //stateがエラー値か
    virtual const bool IsStateError(const State state) const;

    //攻撃状態がエラー
    virtual const bool IsAttackStateError(void);

    //攻撃状態がエラー
    virtual const bool IsAttackStateError(const AttackState attackState);

    void EnemyRotate(Quaternion qua);

    //HPが半分以下
    virtual const bool IsChangeHardMode(void);

    //isHitUpdate_の初期化
    virtual void InitIsHitUpdate(void) = 0;

    /// <summary>
    /// 影の初期化
    /// </summary>
    virtual void InitShadow(void) = 0;

private:

};

