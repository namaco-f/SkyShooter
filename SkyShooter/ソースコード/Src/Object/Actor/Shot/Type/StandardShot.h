#pragma once
#include <unordered_Map>
#include "../ShotBase.h"

/// <summary>
/// �ʏ�e�̊��N���X
/// </summary>
class StandardShot
	: 
	public ShotBase
{

public:

    /// <summary>
    /// ���
    /// </summary>
    enum class State
    {
        None,       //�ʏ�
        Blast,      //����
        End,        //�I��
        MAX,        //�ő吔�J�E���g�p
    };

	StandardShot(void) = default;
    virtual ~StandardShot(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="trans">���f��������</param>
    explicit StandardShot(const Transform& trans);

    /// <summary>
    /// �`�揈��
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// ��ԑJ��
    /// </summary>
    /// <param name="state">���</param>
    virtual void ChangeState(const State state);

    virtual const bool IsAlive(void) const override;

protected:

    //���
    State state_;

    //��ԕω����̊֐��̓o�^
    void AddStateChange(void);

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
    void Change_None(void);     //�ʏ�
    void Change_Blast(void);    //����
    void Change_End(void);      //�I��

    //��Ԃ��Ƃ̍X�V����
    std::function<void(void)> stateUpdate_;
    void Update_None(void);
    virtual void Update_Blast(void);
    void Update_End(void);

    //��Ԃ��Ƃ̍X�V����
    std::function<void(void)> stateDraw_;
    virtual void Draw_None(void);
    virtual void Draw_Blast(void);
    virtual void Draw_End(void);

    //�e�̔j�󏈗�
    void Destroy(void) override;

    virtual void AddChangeState(void) = 0;

    //�j�󂳂�Ă��邩
    virtual const bool IsDestroy(void) const override;

private:
};

