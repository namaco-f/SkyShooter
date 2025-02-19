#pragma once

#include "../Type/UltBase.h"

/// <summary>
/// ���@�̃E���g
/// </summary>
class PlayerShot_Ult :
    public UltBase
{
public:

    PlayerShot_Ult(void) = default;
    virtual ~PlayerShot_Ult(void) = default;

    explicit PlayerShot_Ult(const Transform& trans);

    /// <summary>
    /// �`�揈��
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// �e���˂̍��W�ݒ�
    /// </summary>
    void InitShotStartPos(void) override;


protected:

    float step_;

    //���������̓Ǎ�
    void InitLoad(void) override;

    //���f��������̏�����
    void InitTransform(void) override;

    //�Փ˔���̏�����
    void InitCollider(void) override;

    //�S�Ă̏������I����̏���
    void InitEnd(void) override;

    virtual void InitEffect(void);

    //�G�ɏՓ˂��Ă���
    virtual void HitEnemy(void) override;

    //�f�o�b�O�p�`��
    virtual void DrawDebugModel(void);

    //��ԑJ�ڂ�ǉ�
    virtual void AddStateChange(void) override;

    virtual void Change_None(void);     //�ʏ�
    virtual void Change_Attack(void);   //�U����
    virtual void Change_Blast(void);    //����
    virtual void Change_End(void);      //�I��

    virtual void Update_None(void);     //�ʏ�
    virtual void Update_Attack(void);   //�U����
    virtual void Update_Blast(void);    //����
    virtual void Update_End(void);      //�I��   

private:

    //���ˈʒu
    virtual const VECTOR CalucuratePos(void);

    //�v���C���[�̍��W���擾
    virtual const Transform& GetPlayerTransform(void);

    //�G�̍��W���擾
    virtual const Transform& GetEnemyTransform(void);

    //���@����G�ւ̃x�N�g�����狁�߂���]�Ɣ��ˍ��W���擾
    virtual const Transform LookRotateTransform(void);

    //���f��������̍��W�ƃN�H�[�^�j�I�����X�V
    virtual void TransformUpdate_PosQua(const Transform& trans);
    
    //�v���C���[�Ƀ��b�Z�[�W
    virtual void SendPlayerMessage(const Common_Message message);
};

