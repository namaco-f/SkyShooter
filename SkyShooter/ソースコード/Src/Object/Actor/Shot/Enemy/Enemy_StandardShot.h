#pragma once
#include "../Type/StandardShot.h"

class Enemy_StandardShot :
    public StandardShot
{

public:
    
    Enemy_StandardShot(void) = default;
    explicit Enemy_StandardShot(const Transform& trans);
    virtual ~Enemy_StandardShot(void);

private:

    /// <summary>
    /// 描画処理
    /// </summary>
    virtual void Draw(void) override;

    //弾発射の設定
    void InitShotStartPos(void) override;

    /// <summary>
    /// 弾の生存判定
    /// </summary>
    /// <returns>
    /// true : 生存
    /// false : 破壊
    /// </returns>
    virtual const bool IsAlive(void) const override;

private:

    //リソースの読込
    void InitLoad(void) override;

    //モデル制御情報の更新
    void InitTransform(void) override;

    void InitCollider(void) override;
    void InitEnd(void) override;

    void HitPlayer(void) override;

    virtual void AddChangeState(void) override;
    virtual void Change_None(void);
    virtual void Change_Blast(void);
    virtual void Change_End(void);

};

