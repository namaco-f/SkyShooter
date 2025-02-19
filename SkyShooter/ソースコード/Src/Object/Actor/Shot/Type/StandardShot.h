#pragma once
#include <unordered_Map>
#include "../ShotBase.h"

/// <summary>
/// 通常弾の基底クラス
/// </summary>
class StandardShot
	: 
	public ShotBase
{

public:

    /// <summary>
    /// 状態
    /// </summary>
    enum class State
    {
        None,       //通常
        Blast,      //爆発
        End,        //終了
        MAX,        //最大数カウント用
    };

	StandardShot(void) = default;
    virtual ~StandardShot(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="trans">モデル制御情報</param>
    explicit StandardShot(const Transform& trans);

    /// <summary>
    /// 描画処理
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// 状態遷移
    /// </summary>
    /// <param name="state">状態</param>
    virtual void ChangeState(const State state);

    virtual const bool IsAlive(void) const override;

protected:

    //状態
    State state_;

    //状態変化時の関数の登録
    void AddStateChange(void);

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
    void Change_None(void);     //通常
    void Change_Blast(void);    //爆発
    void Change_End(void);      //終了

    //状態ごとの更新処理
    std::function<void(void)> stateUpdate_;
    void Update_None(void);
    virtual void Update_Blast(void);
    void Update_End(void);

    //状態ごとの更新処理
    std::function<void(void)> stateDraw_;
    virtual void Draw_None(void);
    virtual void Draw_Blast(void);
    virtual void Draw_End(void);

    //弾の破壊処理
    void Destroy(void) override;

    virtual void AddChangeState(void) = 0;

    //破壊されているか
    virtual const bool IsDestroy(void) const override;

private:
};

