
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
    //現在の入力情報から判定
    return inputMng_.GetCurrentInput(action);
}

bool InputController::IsTriggered(const ACTION& action) const
{
    //現フレームで押されているか
    if (IsPressed(action)) 
    {
        //1f前に押されていないか判定
        return !inputMng_.GetLastInput(action);   
    }

    return false;
}

bool InputController::IsReleased(const ACTION& action) const
{
    //現フレームで押されていない
    if (!IsPressed(action)) 
    {
        //1f前に押されていたか
        return inputMng_.GetLastInput(action);
    }

    return false;
}
