#pragma once
#include "../Type/Missile.h"

/// <summary>
/// �v���C���[�̃~�T�C���e
/// </summary>
class PlayerShot_Missile 
    :
    public Missile
{

public:

    PlayerShot_Missile(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="trans">���f��������</param>
    explicit PlayerShot_Missile(const Transform& trans);
    virtual ~PlayerShot_Missile(void);

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
    void HitEnemy(void) override;

    //��ԕω����̊֐��̓o�^
    virtual void AddStateChange(void) override;

private:
};

