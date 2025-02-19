#pragma once

#include <functional>
#include "../Manager/Input/InputControllerTag.h"
#include "../CharactorBase.h"

class CharactorManager;
class ShotManager;
class Camera;
class InputController;
class PlayerInputComponent;
class Collision2D;
class PlayerShotTrigger;
class SoundController;
class CircleShadow;

/// <summary>
/// 自機基底クラス
/// </summary>
class PlayerBase :
    public CharactorBase
{

public:

    /// <summary>
    /// 状態
    /// </summary>
    enum class State
    {
        None,                   //初期値またはエラー値
        Idle,                   //静止
        Move,                   //移動
        Dash,                   //ダッシュ移動
        Shield,                 //シールド
        ShieldCounterAttack,    //シールドカウンター
        UltCharge,              //ウルトチャージ
        UltAttack,              //ウルト
        Destroy,                //破壊
        End,                    //終了
        MAX,                    //最大数カウント用
    };

    /// <summary>
    /// 攻撃の状態
    /// </summary>
    enum class AttackState
    {
        None,
        Shot1,
        Shot2,
        MAX,
    };

    PlayerBase(void);
    virtual ~PlayerBase(void);

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update(void) override final;

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name=""></param>
    virtual void Draw(void) override final;

    /// <summary>
    /// 回復処理
    /// </summary>
    /// <param name="amount">回復量</param>
    void Heal(const int amount);

    void Step(void);

    /// <summary>
    /// アクションの通知
    /// </summary>
    /// <param name="act">アクション</param>
    void NoticeAction(const ACTION act);

    /// <summary>
    /// 指定の状態に遷移  
    /// </summary>
    virtual void ChangeState(const State state);

    /// <summary>
    /// 指定の攻撃状態に遷移
    /// </summary>
    virtual void ChangeAttackState(const AttackState state);

    /// <summary>
    /// シールドが有効化されているか
    /// </summary>
    virtual const bool IsAbleShield(void) const;

    /// <summary>
    /// シールドカウンター中か
    /// </summary>
    virtual const bool IsShieldCounter(void) const;

    /// <summary>
    /// ウルト中か
    /// </summary>
    virtual const bool IsUltAttackAble(void) const;
    virtual const bool IsUltChargeAble(void) const;
    virtual const bool IsUltAble(void) const;

protected:

    CharactorManager& charaMng_;
    ShotManager& shotMng_;
    Camera& camera_;

    //状態
    State state_;

    AttackState attackState_;

    //平面の衝突判定用
    std::unique_ptr<Collision2D> col2d_;

    //プレイヤーの入力検知
    std::unique_ptr<PlayerInputComponent> playerInput_;

    //弾のトリガー
    std::unique_ptr<PlayerShotTrigger> shotTrigger_;

    //丸影
    std::unique_ptr<CircleShadow> circleShadow_;

    //デルタタイム
    float deltaTime_;

    float shieldCoolTime_;

    //ダッシュの加速率
    float dashPar_;

    //無敵時間
    float invFrameCnt_;

    //死亡時の回復時間
    float deadHealTime_;

    //死亡回数カウント
    int deadCnt_;

    Quaternion preQuaRot_;

    float mpHealTime_;

    float ucHealTime_;

    /// <summary>
    /// アップデート可能か判定
    /// </summary>
    virtual const bool IsErrorUpdate(void) const = 0;

    /// <summary>
    /// コライダー初期化
    /// </summary>
    virtual void InitCollider(void) override;

    virtual void InitParameter(void);

    /// <summary>
    /// 更新開始前の設定
    /// </summary>
    virtual void UpdateBefore(void);

    /// <summary>
    /// 状態ごとの更新
    /// </summary>
    virtual void UpdateState(void) = 0;

    virtual void UpdateCreateShot(void) = 0;

    virtual void UpdateMP(void);

    /// <summary>
    /// 回転処理
    /// </summary>
    virtual void UpdateRotate(void);

    /// <summary>
    /// 移動処理
    /// </summary>
    virtual void UpdateMove(void);

    /// <summary>
    /// Transformの更新
    /// </summary>
    virtual void UpdateTransform(void);

    /// <summary>
    /// 影描画
    /// </summary>
    virtual void DrawShadow(void);

    //状態遷移時に実行される関数
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //状態ごとの更新処理
    std::function<void(void)> stateUpdate_;

    //入力検知したアクションに応じて実行される関数
    std::unordered_map<ACTION, std::function<void(void)>> actionUpdate_;
    virtual void MoveDir_Forward(void);         //前移動
    virtual void MoveDir_Back(void);            //後移動
    virtual void MoveDir_Left(void);            //左移動
    virtual void MoveDir_Right(void);           //右移動
    virtual void MoveDir_Up(void);              //上移動
    virtual void Run(void);                     //通常移動
    virtual void Dash(void);                    //ダッシュ
    virtual void UltAttack(void);               //右クリック攻撃
    virtual void Shield(void);                  //シールド
    virtual void LockSwitch(void);              //ロックオンモード
    virtual void Idle(void);                    //Idle状態


    //「移動と共存出来る攻撃」の状態変化時に実行される関数
    std::unordered_map<AttackState, std::function<void(void)>> attackStateChange_;
   
    //「移動と共存出来る攻撃」の状態ごとの更新処理
    std::function<void(void)> attackStateUpdate_;
    
    virtual void Attack_Left(void);             //左クリック攻撃
    virtual void Attack_Right(void);            //右クリック攻撃

    /// <summary>
    /// 衝突したオブジェクトに応じて呼ばれる関数
    /// </summary>
    virtual void IsHit_Enemy(void);             //敵と衝突
    virtual void IsHit_EnemyShot1(void);        //敵の弾と衝突
    virtual void IsHit_EnemyMissile(void);      //敵の弾と衝突
    virtual void IsHit_EnemyBox(void);          //箱攻撃と衝突

    //状態ごとの状態遷移を追加
    virtual void AddStateChange(void) = 0;
    virtual void AddAttackStateChange(void) = 0;

    //state_がエラー値か
    virtual const bool IsStateError(void) const;
    virtual const bool IsStateError(const State state) const;
    
    virtual const bool IsAttackStateError(const AttackState state) const;
    virtual const bool IsNotNecessaryAttackStateUpdate(void) const;

    virtual const bool IsInvisible(void) const;

    void PlayerRotate(const Quaternion& qua);

    virtual void InitIsHitUpdate(void) = 0;

    virtual void InitActionUpdate(void) = 0;

    virtual void MoveDir(const VECTOR& deg, const VECTOR& dirAxis);

    /// <summary>
    /// 現在のMPと最大MPを取得
    /// </summary>
    /// <returns>
    /// x:現在のMP
    /// y:最大MP
    /// </returns>
    virtual Vector2 GetMpData(void) const;

    /// <summary>
    /// 現在のUCと最大UCを取得
    /// </summary>
    /// <returns>
    /// x:現在のUC
    /// y:最大UC
    /// </returns>
    virtual Vector2f GetUCData(void) const;

    /// <summary>
    /// 現在のHPと最大HPを取得
    /// </summary>
    /// <returns>
    /// x:現在のHP
    /// y:最大HP
    /// </returns>
    virtual Vector2 GetHPData(void) const;

    /// <summary>
    /// MP回復
    /// </summary>
    /// <param name="healVal">回復量</param>
    virtual void MpHeal(const int healVal);

    /// <summary>
    /// MP全回復
    /// </summary>
    virtual void MpHealMax(void);

    /// <summary>
    /// UCゲージ回復
    /// </summary>
    /// <param name="healVal">回復量</param>
    virtual void UCHeal(const float healVal);
    
    /// <summary>
    /// UCゲージ使用
    /// </summary>
    virtual void UCActive(void);

    /// <summary>
    /// HP回復
    /// </summary>
    virtual void HpHeal(const int healVal);

    /// <summary>
    /// 接地通知時の処理
    /// </summary>
    virtual void UpdateObserver_IsGroundPlayer(void);

    /// <summary>
    /// 非接地通知時の処理
    /// </summary>
    virtual void UpdateObserver_IsNotGroundPlayer(void);

private:
};

