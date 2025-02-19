#pragma once
#include <unordered_Map>
#include <list>
#include "../ShotBase.h"


class UltBase :
    public ShotBase
{
public:

    /// <summary>
    /// ���
    /// </summary>
    enum class State
    {
        None,       //�������p
        Charge,     //�U���O
        Attack,     //�U����
        Blast,      //����
        End,        //�I��
        MAX,        //�ő吔�J�E���g�p
    };

    UltBase(void) = default;
    virtual ~UltBase(void) = default;

    explicit UltBase(const Transform& transform);

    virtual void Draw(void) override;

    /// <summary>
    /// ��ԑJ��
    /// </summary>
    /// <param name="state">���</param>
    virtual void ChangeState(const State state);

protected:


    //���
    State state_;

    //��ԕω����̊֐��̓o�^
    virtual void AddStateChange(void) = 0;

    //�e���˂̐ݒ�
    virtual void InitShotStartPos(void) = 0;

    //�e�̔��˕�����ݒ�
    virtual void InitShotVec(void);

    //��Ԃ��Ƃ̍X�V
    void UpdateState(void) override;

    //�ړ�����
    void Move(void) override;

    //��ԕω����ɍs����֐�
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //��Ԃ��Ƃ̍X�V����
    std::function<void(void)> stateUpdate_;

    //�e�̔j�󏈗�
    void Destroy(void) override;

    //�j�󂳂�Ă��邩
    virtual const bool IsDestroy(void) const override;
};

