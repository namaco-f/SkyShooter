#pragma once
#include "../UIParam.h"
#include "../UIBase.h"

/// <summary>
/// “GHP‚ÌUI
/// </summary>
class UI_EnemyHP :
    public UIBase
{
public:

    UI_EnemyHP(void);
    virtual ~UI_EnemyHP(void);

    virtual void Update(void) override;

    /// <summary>
    /// •`‰æˆ—
    /// </summary>
    virtual void Draw(void) override final;

private:

    int s20FontHandle_;

    UIParam<int> hp_;

    //HPƒo[•`‰æ
    virtual void DrawHPBar(void);

    //“G‚Ì–¼‘O•`‰æ
    virtual void DrawEnemyName(void);

    //ƒfƒoƒbƒO•`‰æ
    virtual void DrawDebug(void);
};

