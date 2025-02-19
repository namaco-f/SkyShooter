#pragma once

#include <functional>
#include "../Manager/Input/InputControllerTag.h"
#include "../CharactorBase.h"

class CharactorManager;
class ShotManager;
class Camera;
class InputController;
class PlayerInputComponent;
class Collision2D;
class PlayerShotTrigger;
class SoundController;
class CircleShadow;

/// <summary>
/// ���@���N���X
/// </summary>
class PlayerBase :
    public CharactorBase
{

public:

    /// <summary>
    /// ���
    /// </summary>
    enum class State
    {
        None,                   //�����l�܂��̓G���[�l
        Idle,                   //�Î~
        Move,                   //�ړ�
        Dash,                   //�_�b�V���ړ�
        Shield,                 //�V�[���h
        ShieldCounterAttack,    //�V�[���h�J�E���^�[
        UltCharge,              //�E���g�`���[�W
        UltAttack,              //�E���g
        Destroy,                //�j��
        End,                    //�I��
        MAX,                    //�ő吔�J�E���g�p
    };

    /// <summary>
    /// �U���̏��
    /// </summary>
    enum class AttackState
    {
        None,
        Shot1,
        Shot2,
        MAX,
    };

    PlayerBase(void);
    virtual ~PlayerBase(void);

    /// <summary>
    /// �X�V����
    /// </summary>
    virtual void Update(void) override final;

    /// <summary>
    /// �`�揈��
    /// </summary>
    /// <param name=""></param>
    virtual void Draw(void) override final;

    /// <summary>
    /// �񕜏���
    /// </summary>
    /// <param name="amount">�񕜗�</param>
    void Heal(const int amount);

    void Step(void);

    /// <summary>
    /// �A�N�V�����̒ʒm
    /// </summary>
    /// <param name="act">�A�N�V����</param>
    void NoticeAction(const ACTION act);

    /// <summary>
    /// �w��̏�ԂɑJ��  
    /// </summary>
    virtual void ChangeState(const State state);

    /// <summary>
    /// �w��̍U����ԂɑJ��
    /// </summary>
    virtual void ChangeAttackState(const AttackState state);

    /// <summary>
    /// �V�[���h���L��������Ă��邩
    /// </summary>
    virtual const bool IsAbleShield(void) const;

    /// <summary>
    /// �V�[���h�J�E���^�[����
    /// </summary>
    virtual const bool IsShieldCounter(void) const;

    /// <summary>
    /// �E���g����
    /// </summary>
    virtual const bool IsUltAttackAble(void) const;
    virtual const bool IsUltChargeAble(void) const;
    virtual const bool IsUltAble(void) const;

protected:

    CharactorManager& charaMng_;
    ShotManager& shotMng_;
    Camera& camera_;

    //���
    State state_;

    AttackState attackState_;

    //���ʂ̏Փ˔���p
    std::unique_ptr<Collision2D> col2d_;

    //�v���C���[�̓��͌��m
    std::unique_ptr<PlayerInputComponent> playerInput_;

    //�e�̃g���K�[
    std::unique_ptr<PlayerShotTrigger> shotTrigger_;

    //�ۉe
    std::unique_ptr<CircleShadow> circleShadow_;

    //�f���^�^�C��
    float deltaTime_;

    float shieldCoolTime_;

    //�_�b�V���̉�����
    float dashPar_;

    //���G����
    float invFrameCnt_;

    //���S���̉񕜎���
    float deadHealTime_;

    //���S�񐔃J�E���g
    int deadCnt_;

    Quaternion preQuaRot_;

    float mpHealTime_;

    float ucHealTime_;

    /// <summary>
    /// �A�b�v�f�[�g�\������
    /// </summary>
    virtual const bool IsErrorUpdate(void) const = 0;

    /// <summary>
    /// �R���C�_�[������
    /// </summary>
    virtual void InitCollider(void) override;

    virtual void InitParameter(void);

    /// <summary>
    /// �X�V�J�n�O�̐ݒ�
    /// </summary>
    virtual void UpdateBefore(void);

    /// <summary>
    /// ��Ԃ��Ƃ̍X�V
    /// </summary>
    virtual void UpdateState(void) = 0;

    virtual void UpdateCreateShot(void) = 0;

    virtual void UpdateMP(void);

    /// <summary>
    /// ��]����
    /// </summary>
    virtual void UpdateRotate(void);

    /// <summary>
    /// �ړ�����
    /// </summary>
    virtual void UpdateMove(void);

    /// <summary>
    /// Transform�̍X�V
    /// </summary>
    virtual void UpdateTransform(void);

    /// <summary>
    /// �e�`��
    /// </summary>
    virtual void DrawShadow(void);

    //��ԑJ�ڎ��Ɏ��s�����֐�
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //��Ԃ��Ƃ̍X�V����
    std::function<void(void)> stateUpdate_;

    //���͌��m�����A�N�V�����ɉ����Ď��s�����֐�
    std::unordered_map<ACTION, std::function<void(void)>> actionUpdate_;
    virtual void MoveDir_Forward(void);         //�O�ړ�
    virtual void MoveDir_Back(void);            //��ړ�
    virtual void MoveDir_Left(void);            //���ړ�
    virtual void MoveDir_Right(void);           //�E�ړ�
    virtual void MoveDir_Up(void);              //��ړ�
    virtual void Run(void);                     //�ʏ�ړ�
    virtual void Dash(void);                    //�_�b�V��
    virtual void UltAttack(void);               //�E�N���b�N�U��
    virtual void Shield(void);                  //�V�[���h
    virtual void LockSwitch(void);              //���b�N�I�����[�h
    virtual void Idle(void);                    //Idle���


    //�u�ړ��Ƌ����o����U���v�̏�ԕω����Ɏ��s�����֐�
    std::unordered_map<AttackState, std::function<void(void)>> attackStateChange_;
   
    //�u�ړ��Ƌ����o����U���v�̏�Ԃ��Ƃ̍X�V����
    std::function<void(void)> attackStateUpdate_;
    
    virtual void Attack_Left(void);             //���N���b�N�U��
    virtual void Attack_Right(void);            //�E�N���b�N�U��

    /// <summary>
    /// �Փ˂����I�u�W�F�N�g�ɉ����ČĂ΂��֐�
    /// </summary>
    virtual void IsHit_Enemy(void);             //�G�ƏՓ�
    virtual void IsHit_EnemyShot1(void);        //�G�̒e�ƏՓ�
    virtual void IsHit_EnemyMissile(void);      //�G�̒e�ƏՓ�
    virtual void IsHit_EnemyBox(void);          //���U���ƏՓ�

    //��Ԃ��Ƃ̏�ԑJ�ڂ�ǉ�
    virtual void AddStateChange(void) = 0;
    virtual void AddAttackStateChange(void) = 0;

    //state_���G���[�l��
    virtual const bool IsStateError(void) const;
    virtual const bool IsStateError(const State state) const;
    
    virtual const bool IsAttackStateError(const AttackState state) const;
    virtual const bool IsNotNecessaryAttackStateUpdate(void) const;

    virtual const bool IsInvisible(void) const;

    void PlayerRotate(const Quaternion& qua);

    virtual void InitIsHitUpdate(void) = 0;

    virtual void InitActionUpdate(void) = 0;

    virtual void MoveDir(const VECTOR& deg, const VECTOR& dirAxis);

    /// <summary>
    /// ���݂�MP�ƍő�MP���擾
    /// </summary>
    /// <returns>
    /// x:���݂�MP
    /// y:�ő�MP
    /// </returns>
    virtual Vector2 GetMpData(void) const;

    /// <summary>
    /// ���݂�UC�ƍő�UC���擾
    /// </summary>
    /// <returns>
    /// x:���݂�UC
    /// y:�ő�UC
    /// </returns>
    virtual Vector2f GetUCData(void) const;

    /// <summary>
    /// ���݂�HP�ƍő�HP���擾
    /// </summary>
    /// <returns>
    /// x:���݂�HP
    /// y:�ő�HP
    /// </returns>
    virtual Vector2 GetHPData(void) const;

    /// <summary>
    /// MP��
    /// </summary>
    /// <param name="healVal">�񕜗�</param>
    virtual void MpHeal(const int healVal);

    /// <summary>
    /// MP�S��
    /// </summary>
    virtual void MpHealMax(void);

    /// <summary>
    /// UC�Q�[�W��
    /// </summary>
    /// <param name="healVal">�񕜗�</param>
    virtual void UCHeal(const float healVal);
    
    /// <summary>
    /// UC�Q�[�W�g�p
    /// </summary>
    virtual void UCActive(void);

    /// <summary>
    /// HP��
    /// </summary>
    virtual void HpHeal(const int healVal);

    /// <summary>
    /// �ڒn�ʒm���̏���
    /// </summary>
    virtual void UpdateObserver_IsGroundPlayer(void);

    /// <summary>
    /// ��ڒn�ʒm���̏���
    /// </summary>
    virtual void UpdateObserver_IsNotGroundPlayer(void);

private:
};

