#pragma once

#include <unordered_Map>
#include <list>
#include "../ShotBase.h"

class Trail;

/// <summary>
/// �~�T�C�����N���X
/// </summary>
class Missile :
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
        MAX,        //�ő吔�J�E���g�p : �������p
    };

    Missile(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="trans">���f��������</param>
    Missile(const Transform& trans);
    virtual ~Missile(void);

    /// <summary>
    /// �`�揈��
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// ��ԑJ��
    /// </summary>
    virtual void ChangeState(const State state);

    virtual const bool IsAlive(void) const override;

protected:

    //���
    State state_;

    //�ėU���̃J�E���g�p
    float redirectionStep_;

    //�g���C��
    std::unique_ptr<Trail> trail_;

    //��ԕω����̊֐��̓o�^
    virtual void AddStateChange(void) = 0;

    //�e���˂̐ݒ�
    void InitShotStartPos(void);

    //��Ԃ��Ƃ̍X�V����
    void UpdateState(void);

    //�S�Ă̍X�V�����I����̏���
    void UpdateEnd(void) override;

    //�ړ�����
    void Move(void) override;

    //�e�̔j�󏈗�
    void Destroy(void) override;

    //��ԕω����ɍs����֐�
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //��Ԃ��Ƃ̍X�V����
    std::function<void(void)> stateUpdate_;

    //���f���֘A�̃f�o�b�O�`��
    void DrawDebugModel(void);

    //�j�󂳂�Ă��邩
    virtual const bool IsDestroy(void) const override;

private:

};

