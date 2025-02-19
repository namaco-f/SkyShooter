#pragma once

#include <functional>
#include <memory>

class PlayerBase;
class InputController;

/// <summary>
/// �v���C���[�̓��͌��m
/// </summary>
class PlayerInputComponent
{

public:

    PlayerInputComponent(void);
    virtual ~PlayerInputComponent(void);

    /// <summary>
    /// �X�V����
    /// </summary>
    /// <param name="player">���͂�ʒm���鎩�@�̎Q��</param>
    virtual void Update(PlayerBase& player);

private:

    //���͐���
    std::unique_ptr<InputController> inputController_;

    //�f���^�^�C��
    float deltaTime_;

    //�ړ��֘A
    void InputUpdateMove(PlayerBase& player);

    //�U���֘A
    void InputUpdateAttack(PlayerBase& player);     

    //���ɃL�[��������Ă��Ȃ��ꍇ
    const bool IsNotMoving(void);
};

