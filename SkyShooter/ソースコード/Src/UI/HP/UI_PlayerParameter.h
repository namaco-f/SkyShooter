#pragma once
#include "../UIParam.h"
#include "../UIBase.h"

/// <summary>
/// é©ã@HPÇÃUI
/// </summary>
class UI_PlayerParameter :
    public UIBase
{
public:

    UI_PlayerParameter(void);
    virtual ~UI_PlayerParameter(void);

    virtual void Update(void) override;

    /// <summary>
    /// ï`âÊèàóù
    /// </summary>
    virtual void Draw(void) override;


private:



    UIParam<int> hp_;
    UIParam<int> mp_;
    UIParam<float> uc_;

    int s20FontHandle_;
    int hpUITextImgHandle_;
    int mpUITextImgHandle_;

    /// <summary>
    /// HP
    /// </summary>
    virtual void DrawHP(void);
    
    virtual void DrawMP(void);

    virtual void DrawUC(void);

    virtual void DrawShieldTime(void);
};

