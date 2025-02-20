#pragma once
#include <unordered_Map>
#include <list>
#include "../ShotBase.h"


class UltBase :
    public ShotBase
{
public:

    /// <summary>
    /// 状態
    /// </summary>
    enum class State
    {
        None,       //初期化用
        Charge,     //攻撃前
        Attack,     //攻撃中
        Blast,      //爆発
        End,        //終了
        MAX,        //最大数カウント用
    };

    UltBase(void) = default;
    virtual ~UltBase(void) = default;

    explicit UltBase(const Transform& transform);

    virtual void Draw(void) override;

    /// <summary>
    /// 状態遷移
    /// </summary>
    /// <param name="state">状態</param>
    virtual void ChangeState(const State state);

protected:


    //状態
    State state_;

    //状態変化時の関数の登録
    virtual void AddStateChange(void) = 0;

    //弾発射の設定
    virtual void InitShotStartPos(void) = 0;

    //弾の発射方向を設定
    virtual void InitShotVec(void);

    //状態ごとの更新
    void UpdateState(void) override;

    //移動処理
    void Move(void) override;

    //状態変化時に行われる関数
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //状態ごとの更新処理
    std::function<void(void)> stateUpdate_;

    //弾の破壊処理
    void Destroy(void) override;

    //破壊されているか
    virtual const bool IsDestroy(void) const override;
};

