#pragma once

#include "PlayerBase.h"
#include "../../Shot/Shot_CreateData.h"

/// <summary>
/// ���@01
/// </summary>
class PlayerUnit01 :
    public PlayerBase
{

public:

    PlayerUnit01(void);
    virtual ~PlayerUnit01(void);

protected:

    //���������̓Ǎ�
    void InitLoad(void) override;

    //���f��������̏�����
    void InitTransform(void) override;

    //�R���C�_�̏�����
    void InitCollider(void) override;

    void InitParameter(void) override;

    //�����_�[�̏�����
    void InitRender(void) override;

    //�S�Ă̏������I����̏���
    void InitEnd(void) override;

    //�T�E���h������
    void InitSound(void) override;

    //
    void Destroy(void) override;

    //�A�j���[�V�����̏�����
    void InitAnimation(void) override;

    virtual void IsHit_EnemyShot1(void) override;        //�G�̒e�ƏՓ�
    virtual void IsHit_EnemyMissile(void) override;      //�G�̒e�ƏՓ�
    virtual void IsHit_EnemyBox(void) override;          //���U���ƏՓ�

    //�d�͖�������
    virtual const bool InvalidGravity(void) const override;

private:


    //�ʏ�e : �o�[�X�g
    ShotData standardShot_;

    //�~�T�C��
    ShotData missile_;

    //�J�E���^�[�e
    ShotData counterShot_;

    //�V�[���h���ԃJ�E���g�p
    float shieldTimeCnt_;

    //�V�[���h���ԏ��
    float shieldTimeLimit_;

    //�V�[���h���ԏ���J�E���g�p
    float shieldTimeLimitCnt_;

    //�V�[���h�N�[���^�C��
    float shieldTimeCool_;

    //�V�[���h�����̐�
    int counterCnt_;

    //��Ԃ��Ƃ̍X�V
    void UpdateState(void) override;
    void UpdateCreateShot(void) override;

    virtual void Run(void) override;
    virtual void Dash(void) override;
    virtual void Idle(void) override;
    virtual void Attack_Left(void) override;
    virtual void Attack_Right(void) override;
    virtual void Shield(void) override;
    virtual void UltAttack(void) override;


    //�ʏ�e�̐���
    void CreateUltAttack(void);

    //��ԑJ�ڎ��Ɏ��s�����֐�
    void Change_Idle(void);
    void Change_Move(void);
    void Change_Dash(void);
    void Change_Shot1(void);
    void Change_Shot2(void);
    void Change_Destroy(void);
    void Change_Shield(void);
    void Change_ShieldCounterAttack(void);
    void Change_UltCharge(void);
    void Change_UltAttack(void);
    void Change_End(void);

    //��Ԃ��Ƃ̍X�V����
    void Update_Idle(void);
    void Update_Move(void);
    void Update_Dash(void);
    void Update_Destroy(void);
    void Update_Shield(void);
    void Update_ShieldCounterAttack(void);
    void Update_UltCharge(void);
    void Update_UltAttack(void);
    void Update_End(void);

    virtual void Message_UltAttack(void);
    virtual void Message_UltEnd(void);
    virtual void Message_Dash(void);
    virtual void Message_DashEnd(void);

    //��Ԃ��Ƃ̏�ԑJ�ڂ�ǉ�
    virtual void AddStateChange(void) override;
    virtual void AddAttackStateChange(void) override;

    //�J�n�O�Ɋm�F�ł���G���[�����邩
    const bool IsErrorUpdate(void) const override;

    //�V�[���h���g�p�\
    const bool AbleUseSheild(void) const;

    //�Փˎ������̓o�^
    virtual void InitIsHitUpdate(void) override;

    //���b�Z�[�W��M�������̓o�^
    virtual void InitCommonMessageUpdate(void) override;

    //�A�N�V�����ʒm���̏����̓o�^
    virtual void InitActionUpdate(void) override;
    
    //�J�E���^�[�J�E���g��ǉ�
    virtual void CounterCount(void);

    virtual void InitUpdateObserver(void) override;

};

