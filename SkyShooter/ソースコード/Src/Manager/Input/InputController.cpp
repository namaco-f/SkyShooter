
#include "InputManager.h"
#include "InputController.h"

InputController::InputController(void)
    :
    inputMng_(InputManager::GetInstance())
{
}

InputController::~InputController(void)
{
}

bool InputController::IsPressed(const ACTION& action) const
{  
    //���݂̓��͏�񂩂画��
    return inputMng_.GetCurrentInput(action);
}

bool InputController::IsTriggered(const ACTION& action) const
{
    //���t���[���ŉ�����Ă��邩
    if (IsPressed(action)) 
    {
        //1f�O�ɉ�����Ă��Ȃ�������
        return !inputMng_.GetLastInput(action);   
    }

    return false;
}

bool InputController::IsReleased(const ACTION& action) const
{
    //���t���[���ŉ�����Ă��Ȃ�
    if (!IsPressed(action)) 
    {
        //1f�O�ɉ�����Ă�����
        return inputMng_.GetLastInput(action);
    }

    return false;
}
