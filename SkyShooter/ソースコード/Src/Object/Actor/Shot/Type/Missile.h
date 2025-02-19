#pragma once

#include <unordered_Map>
#include <list>
#include "../ShotBase.h"

class Trail;

/// <summary>
/// ミサイル基底クラス
/// </summary>
class Missile :
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
        MAX,        //最大数カウント用 : 初期化用
    };

    Missile(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="trans">モデル制御情報</param>
    Missile(const Transform& trans);
    virtual ~Missile(void);

    /// <summary>
    /// 描画処理
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// 状態遷移
    /// </summary>
    virtual void ChangeState(const State state);

    virtual const bool IsAlive(void) const override;

protected:

    //状態
    State state_;

    //再誘導のカウント用
    float redirectionStep_;

    //トレイル
    std::unique_ptr<Trail> trail_;

    //状態変化時の関数の登録
    virtual void AddStateChange(void) = 0;

    //弾発射の設定
    void InitShotStartPos(void);

    //状態ごとの更新処理
    void UpdateState(void);

    //全ての更新処理終了後の処理
    void UpdateEnd(void) override;

    //移動処理
    void Move(void) override;

    //弾の破壊処理
    void Destroy(void) override;

    //状態変化時に行われる関数
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //状態ごとの更新処理
    std::function<void(void)> stateUpdate_;

    //モデル関連のデバッグ描画
    void DrawDebugModel(void);

    //破壊されているか
    virtual const bool IsDestroy(void) const override;

private:

};

