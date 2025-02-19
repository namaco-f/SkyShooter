#pragma once
#include "EnemyBase.h"

/// <summary>
/// �G01
/// </summary>
class EnemyUnit01 :
    public EnemyBase
{

public:

    /// <summary>
    /// 1�g���K�[���Ƃ̒e�̃f�[�^
    /// </summary>
    struct ShotData
    {
        float shotDelay_;       //�e1�����Ƃ̃f�B���C
        float shotDelayStep_;       //�f�B���C�̃J�E���g�p
        int shotNumMax_;        //1�g���K�[�Ŕ��ˏo����e��
        int shotNum_;       //�e�̃J�E���g�p

        /// <summary>
        /// �R���X�g���N�^
        /// </summary>
        /// <param name="delayTime">�e1�����Ƃ̃f�B���C</param>
        /// <param name="shotNumMax">1�g���K�[�Ŕ��ˏo����e��</param>
        ShotData(const float delayTime, const int shotNumMax)
            :
            shotDelay_(delayTime),
            shotDelayStep_(delayTime),
            shotNumMax_(shotNumMax),
            shotNum_(0) {};

        /// <summary>
        /// �J�E���g������
        /// </summary>
        void Init(void)
        {
            shotDelayStep_ = 0.0f;
            shotNum_ = 0;
        }

        /// <summary>
        /// �e�̃f�B���C���I����Ă��邩
        /// </summary>
        /// <returns>
        /// true : �I����Ă���
        /// false : �I����Ă��Ȃ�
        /// </returns>
        const bool IsEndShotDelay(void) const
        {
            return shotDelayStep_ > shotDelay_;
        }

        /// <summary>
        /// �e�����\��
        /// </summary>
        /// <returns>
        /// true : �\
        /// false : �e�������
        /// </returns>
        const bool IsAbleShot(void) const
        {
            return shotNumMax_ > shotNum_;
        }
    };

    EnemyUnit01(void);
    virtual ~EnemyUnit01(void);

    /// <summary>
    /// �`�揈��
    /// </summary>
    void Draw(void) override;

protected:
    //�~�T�C���e
    ShotData missile_;

    //�Ǎ�����
    virtual void InitLoad(void) override;

    //���f��������̏�����
    virtual void InitTransform(void) override;

    //�R���C�_���̏�����
    virtual void InitCollider(void) override;

    //�����_�[�̏�����
    virtual void InitRender(void) override;
    
    //�S�Ă̏����I�����ɍs��������
    virtual void InitEnd(void) override;

    //�A�j���[�V�����̏�����
    virtual void InitAnimation(void) override;

    //��������
    virtual void InitSound(void) override;

    //�G�t�F�N�g������
    virtual void InitEffect(void) override;

    //�j�󎞂̏���
    virtual void Destroy(void) override;

    //�v���C���[�̍��N���b�N�e�ɔ�e
    virtual void HitPlayerShot1(void) override;

    //�v���C���[�̉E�N���b�N�e�ɔ�e
    virtual void HitPlayerShot2(void) override;

    virtual void HitPlayerShotUlt(void) override;

    virtual void InitShadow(void) override;

private:

    //���[�v���ԃJ�E���g�p
    float warpCnt_;

    //���[�v�n�_�̍��W
    VECTOR warpPos_;

    //���[�v����������������
    bool warpPosReady_;

    /// <summary>
    /// ���O�ɍX�V�����\���m�F
    /// </summary>
    /// <returns>
    /// true : ��Ԃ��Ƃ̍X�V��������Ԃ��G���[
    /// </returns>
    const bool IsErrorUpdate(void) const override;

    void DrawAttackBox(void);

    /// <summary>
    /// ��Ԃ��Ƃ̍X�V
    /// </summary>
    void UpdateState(void) override;

    void Change_None(void);
    void Change_Destroy(void);
    void Change_End(void);

    //��Ԃ��Ƃ̍X�V����
    std::function<void(void)> stateUpdate_;
    void Update_None(void);             //�ʏ�
    void Update_Destroy(void);          //�j��
    void Update_End(void);              //�I��

    //��Ԃ��Ƃ̏�ԑJ�ڂ�ǉ�
    void AddStateChange(void);

    //�U���̏�ԕω����ɍs����֐�
    virtual void AttackStateChange_None(void);          //�U�����Ă��Ȃ�
    virtual void AttackStateChange_CoolTime(void);      //�N�[���^�C����
    virtual void AttackStateChange_Missile(void);       //�~�T�C���e
    virtual void AttackStateChange_XYBox(void);         //XY���U��
    virtual void AttackStateChange_XZBox(void);         //XZ���U��
    virtual void AttackStateChange_ZYBox(void);         //ZY���U��

    //�U���̏�Ԃ��Ƃɍs����֐�
    virtual void AttackStateUpdate_None(void);          //�U�����Ă��Ȃ�
    virtual void AttackStateUpdate_CoolTime(void);      //�N�[���^�C����
    virtual void AttackStateUpdate_Missile(void);       //�~�T�C���e
    virtual void AttackStateUpdate_Box(void);           //���U��

    //�U���̏�ԑJ�ڂ�o�^
    virtual void AddAttackStateChange(void) override;

    //�ړ��p�^�[���̓o�^
    virtual void AddMovePattern(void) override;

    virtual void Change_Warp(void) override;

    virtual void Move_Warp(void) override;

    //�~�T�C���e�̐���
    virtual void CreateMissle(void);

    virtual void WarpLerp(void);

    virtual VECTOR GetAttackPosFromTable(const std::string& dataName,const int tableNo);

    virtual void InitIsHitUpdate(void) override;

};

