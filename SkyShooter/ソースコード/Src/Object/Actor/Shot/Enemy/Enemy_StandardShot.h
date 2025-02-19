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
    /// •`‰æˆ—
    /// </summary>
    virtual void Draw(void) override;

    //’e”­Ë‚Ìİ’è
    void InitShotStartPos(void) override;

    /// <summary>
    /// ’e‚Ì¶‘¶”»’è
    /// </summary>
    /// <returns>
    /// true : ¶‘¶
    /// false : ”j‰ó
    /// </returns>
    virtual const bool IsAlive(void) const override;

private:

    //ƒŠƒ\[ƒX‚Ì“Ç
    void InitLoad(void) override;

    //ƒ‚ƒfƒ‹§Œäî•ñ‚ÌXV
    void InitTransform(void) override;

    void InitCollider(void) override;
    void InitEnd(void) override;

    void HitPlayer(void) override;

    virtual void AddChangeState(void) override;
    virtual void Change_None(void);
    virtual void Change_Blast(void);
    virtual void Change_End(void);

};

