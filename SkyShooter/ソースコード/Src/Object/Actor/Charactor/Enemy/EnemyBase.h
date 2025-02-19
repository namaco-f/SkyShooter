#pragma once

#include <functional>
#include "../CharactorBase.h"

class ShotManager;
class CircleShadow;

/// <summary>
/// �G���N���X
/// </summary>
class EnemyBase :
    public CharactorBase
{

public:

    /// <summary>
    /// �G�̏��
    /// </summary>
    enum class State
    {
        None,       //�����l�܂��̓G���[�l
        Destroy,
        End,
        MAX,        //��Ԃ̐�
    };

    /// <summary>
    /// �ړ��p�^�[��
    /// </summary>
    enum class MOVE_PATTERN
    {
        None,           //�ړ����Ȃ�
        Start,          //�J�n��
        Standard,       //���i
        Octagon,        //���p�`
        PlayerDir,      //�v���C���[�Ɍ����Ĉړ�
        Warp,           //���[�v
        MAX,
    };

    /// <summary>
    /// �U���p�^�[��
    /// </summary>
    enum class AttackState
    {
        None,           //�U�����Ȃ�
        CoolTime,       //�N�[���^�C����
        XZBox,          //XZ���ʔ��U��
        ZYBox,          //ZY���ʔ��U��
        XYBox,          //XY���ʔ��U��         
        Missile,        //�~�T�C���e
        MAX,
    };

    EnemyBase(void);
    virtual ~EnemyBase(void);

    /// <summary>
    /// �X�V����
    /// </summary>
    /// <param name=""></param>
    virtual void Update(void) override final;

    //void IsHitNotice(const OBJECT_TYPE obj) override;

    /// <summary>
    /// ��ԑJ��
    /// </summary>
    virtual void ChangeState(const State state);

    /// <summary>
    /// �ړ��p�^�[���̑J��
    /// </summary>
    /// <param name="movePattern"></param>
    virtual void ChangeMovePattern(const MOVE_PATTERN movePattern);

    /// <summary>
    /// �U���p�^�[���̑J��
    /// </summary>
    /// <param name="attackState"></param>
    virtual void ChangeAttackState(const AttackState attackState);

protected:

    //�e
    ShotManager& shotMng_;

    //�G�̏��
    State state_;

    //�ړ����
    MOVE_PATTERN movePattern_;

    //�U�����
    AttackState attackState_;

    //�e
    std::unique_ptr<CircleShadow> circleShadow_;

    //�ړ����̎��ԃJ�E���g�p
    float moveTimeCnt_;

    //��]�̎��ԃJ�E���g�p
    float moveTurnCnt_;

    //�U���I����̃N�[���^�C��
    float attackCoolTime_;

    //�N�[���^�C���I����U���J�n�܂ł̃f�B���C
    float attackCoolTimeDelay_;

    //�f���^�^�C��
    float deltaTime_;

    //��]����
    float turnTime_;

    //�ړ���ԕω����̉�]���i�[
    Quaternion preQuaRot;

    //���[�v��������Ԏ���
    float invCnt_;

    /// <summary>
    /// �X�V�����\���m�F
    /// </summary>
    /// <returns>
    /// true : �X�V�������s�\
    /// false : �X�V�����\
    /// </returns>
    virtual const bool IsErrorUpdate(void) const = 0;

    /// <summary>
    /// �X�V�J�n�O�̐ݒ�
    /// </summary>
    virtual void UpdateBefore(void);

    /// <summary>
    /// �U���̍X�V
    /// </summary>
    virtual void UpdateAttack(void);

    /// <summary>
    /// ��Ԃ��Ƃ̍X�V
    /// </summary>
    virtual void UpdateState(void) = 0;

    /// <summary>
    /// ��]����
    /// </summary>
    virtual void UpdateRotate(void);

    /// <summary>
    /// �ړ�����
    /// </summary>
    virtual void UpdatePos(void);

    /// <summary>
    /// Transform�̍X�V
    /// </summary>
    virtual void UpdateTransform(void);

    /// <summary>
    /// �f�o�b�O�`��
    /// </summary>
    virtual void DrawDebug(void);
    
    //hp�����ȉ�
    virtual void Destroy(void) override;

    //�v���C���[�̍U��1�ƏՓ�
    void IsHit_PlayerShot1(void);       
    void IsHit_PlayerShot2(void);       

    //�v���C���[�̍U��1�ƏՓ˂����ۂ̏���
    virtual void HitPlayerShot1(void) = 0;
    virtual void HitPlayerShot2(void) = 0;
    virtual void HitPlayerShotUlt(void) = 0;

    //��ԑJ�ڎ��Ɏ��s�����֐�
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //�ړ��p�^�[���̕ύX
    std::unordered_map<MOVE_PATTERN, std::function<void(void)>> movePatternChange_;
    virtual void Change_Start(void);        //�X�^�[�g��
    virtual void Change_Standard(void);     //�ʏ�
    virtual void Change_Circle(void);       //�~�`
    virtual void Change_Octagon(void);      //���p
    virtual void Change_PlayerDir(void);    //�v���C���[�����֑O�i
    virtual void Change_Warp(void);         //���[�v

    //�p�^�[�����Ƃ̈ړ�
    std::function<void(void)> patternMove_;
    virtual void Move_Standard(void);       //�ʏ�
    virtual void Move_Circle(void);         //�~�`
    virtual void Move_Octagon(void);        //���p
    virtual void Move_PlayerDir(void);      //���p
    virtual void Move_Start(void);          //�Q�[���X�^�[�g��
    virtual void Move_Warp(void);          //�Q�[���X�^�[�g��

    //�ړ��p�^�[���̓o�^
    virtual void AddMovePattern(void) = 0;

    //�U���̏�ԕω����Ɏ��s�����֐�
    std::unordered_map<AttackState, std::function<void(void)>> attackStateChange_;

    //�U���̏�Ԃ��ƂɎ��s�����֐�
    std::function<void(void)> attackStateUpdate_;

    //�U����Ԃ̓o�^
    virtual void AddAttackStateChange(void) = 0;

    //state_���G���[�l��
    virtual const bool IsStateError(void) const;

    //state���G���[�l��
    virtual const bool IsStateError(const State state) const;

    //�U����Ԃ��G���[
    virtual const bool IsAttackStateError(void);

    //�U����Ԃ��G���[
    virtual const bool IsAttackStateError(const AttackState attackState);

    void EnemyRotate(Quaternion qua);

    //HP�������ȉ�
    virtual const bool IsChangeHardMode(void);

    //isHitUpdate_�̏�����
    virtual void InitIsHitUpdate(void) = 0;

    /// <summary>
    /// �e�̏�����
    /// </summary>
    virtual void InitShadow(void) = 0;

private:

};

