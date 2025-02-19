#pragma once
#include "../Type/Missile.h"

/// <summary>
/// �G�̃~�T�C��
/// </summary>
class Enemy_Missile :
    public Missile
{

public:

    Enemy_Missile(void) = default;
    explicit Enemy_Missile(const Transform& trans);
    virtual ~Enemy_Missile(void);

protected:

    //���\�[�X�̓Ǎ�
    virtual void InitLoad(void) override;

    //���f��������̏�����
    virtual void InitTransform(void) override;

    //�Փ˔���̏�����
    virtual void InitCollider(void) override;

    //�����_���[�̏�����
    virtual void InitRender(void) override;

    //�S�Ă̏������I����̏���
    virtual void InitEnd(void) override;

    virtual void Change_None(void);     //�ʏ�
    virtual void Change_Blast(void);    //����
    virtual void Change_End(void);      //�I��

    virtual void Update_None(void);     //�ʏ�
    virtual void Update_Blast(void);    //����
    virtual void Update_End(void);      //�I��

    //�G�ƏՓ�
    void HitPlayer(void) override;

    //��ԕω����̊֐��̓o�^
    virtual void AddStateChange(void) override;

private:
};

