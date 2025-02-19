#pragma once
#include "../Type/StandardShot.h"

/// <summary>
/// 自機標準弾
/// </summary>
class PlayerShot_Standard 
    : public StandardShot
{

public:

    PlayerShot_Standard(void) = default;
    virtual ~PlayerShot_Standard(void);

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="trans">弾発射時の座標</param>
    explicit PlayerShot_Standard(const Transform& trans);

    /// <summary>
    /// 描画処理
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// 弾発射の座標設定
    /// </summary>
    void InitShotStartPos(void) override;

    ///// <summary>
    ///// 弾の生存判定
    ///// </summary>
    ///// <returns>
    ///// true : 生存
    ///// false : 破壊
    ///// </returns>
    //virtual const bool IsAlive(void) const override;

private:
  
    //初期化時の読込
    void InitLoad(void) override;
    
    //モデル制御情報の初期化
    void InitTransform(void) override;

    //衝突判定の初期化
    void InitCollider(void) override;

    //全ての初期化終了後の処理
    void InitEnd(void) override;
 
    //敵に衝突している
    void HitEnemy(void) override;

    virtual void DrawDebugModel(void);

    virtual void AddChangeState(void) override;
    virtual void Change_None(void);
    virtual void Change_Blast(void);
    virtual void Change_End(void);

    virtual void Update_Blast(void) override;

    virtual void Draw_Blast(void) override;

};

