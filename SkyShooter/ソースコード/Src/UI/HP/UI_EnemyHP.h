#pragma once
#include "../UIParam.h"
#include "../UIBase.h"

/// <summary>
/// �GHP��UI
/// </summary>
class UI_EnemyHP :
    public UIBase
{
public:

    UI_EnemyHP(void);
    virtual ~UI_EnemyHP(void);

    virtual void Update(void) override;

    /// <summary>
    /// �`�揈��
    /// </summary>
    virtual void Draw(void) override final;

private:

    int s20FontHandle_;

    UIParam<int> hp_;

    //HP�o�[�`��
    virtual void DrawHPBar(void);

    //�G�̖��O�`��
    virtual void DrawEnemyName(void);

    //�f�o�b�O�`��
    virtual void DrawDebug(void);
};

