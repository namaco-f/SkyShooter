#include <Dxlib.h>
#include "InputUpdater.h"
#include "InputManager.h"

using namespace std;

InputManager* Singleton<InputManager>::instance_ = nullptr;

const string KEY_NAME_UNKNOWN = "Unknown";

InputManager::InputManager(void)
    :
    inputUpdater_(make_unique<InputUpdater>()),
    inputActionMaps_{},
    currentInput_{},
    currentInputType_(INPUT_TYPE::MAX),
    lastInput_{},
    controlType_(CONTROL_TYPE::MAX)
{
    //���o�^
    inputActionMaps_[ACTION::UN_REGIST] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_0},
    };

    //�����V�[���J��
    inputActionMaps_[ACTION::SCENE_CHANGE] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_BACKSLASH},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::START},
    };

    //�Q�[���X�^�[�g
    inputActionMaps_[ACTION::GAME_START] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_RETURN},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::RIGHT},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::START},
        {INPUT_TYPE::MOUSE,MOUSE_INPUT_LEFT},
    };

    //�O�ړ�
    inputActionMaps_[ACTION::MOVE_FORWARD] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_W},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_UP},
    };

    //��ړ�
    inputActionMaps_[ACTION::MOVE_BACK] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_S},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_DOWN},
    };

    //���ړ�
    inputActionMaps_[ACTION::MOVE_LEFT] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_A},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_LEFT},
    };

    //�E�ړ�
    inputActionMaps_[ACTION::MOVE_RIGHT] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_D},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_RIGHT},
    };

    //��ړ�
    inputActionMaps_[ACTION::MOVE_UP] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_SPACE},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::DOWN},
    };

    //�J����
    //���]
    inputActionMaps_[ACTION::CAMERA_UP] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_UP},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_UP},
    };

    //����]
    inputActionMaps_[ACTION::CAMERA_DOWN] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_DOWN},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_DOWN},
    };

    //����]
    inputActionMaps_[ACTION::CAMERA_LEFT] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_LEFT},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_LEFT},
    };

    //�E��]
    inputActionMaps_[ACTION::CAMERA_RIGHT] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_RIGHT},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_RIGHT},
    };

    //�_�b�V��
    inputActionMaps_[ACTION::PLAYER_DASH] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_LCONTROL},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::L_TRIGGER},
    };   

    //���b�N�I�����[�h�̐ؑ�
    inputActionMaps_[ACTION::PLAYER_LOCK_SWITCH] =
    {
        {INPUT_TYPE::KEYBOARD,KEY_INPUT_R},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::R3_TRIGGER},
    };

    //�v���C���[�̍U��
    inputActionMaps_[ACTION::PLAYER_ATTACK] =
    {
        {INPUT_TYPE::MOUSE,MOUSE_INPUT_LEFT},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::LEFT},
    };

    //�v���C���[�̍U��2
    inputActionMaps_[ACTION::PLAYER_ATTACK2] =
    {
        {INPUT_TYPE::MOUSE,MOUSE_INPUT_RIGHT},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::RIGHT},
    }; 
    
    //�v���C���[�V�[���h
    inputActionMaps_[ACTION::PLAYER_SHIELD] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_B},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::ZL_TRIGGER},
    };

    //�v���C���[�V�[���h
    inputActionMaps_[ACTION::PLAYER_ULTATTACK] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_Q},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::L3_TRIGGER},
    };
    
    //���j���[���J��
    inputActionMaps_[ACTION::MENU_OPEN] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_M},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::START},
    };
    
    //����
    inputActionMaps_[ACTION::MENU_YES] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_Y},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::RIGHT},
        {INPUT_TYPE::MOUSE,MOUSE_INPUT_LEFT},
    };   

    //����
    inputActionMaps_[ACTION::MENU_NO] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_N},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::DOWN},
        {INPUT_TYPE::MOUSE,MOUSE_INPUT_RIGHT},
    };
    
    //����
    inputActionMaps_[ACTION::MENU_ENTER] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_RETURN},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::RIGHT},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::START},
        {INPUT_TYPE::MOUSE,MOUSE_INPUT_LEFT},
    };

    //���j���[��
    inputActionMaps_[ACTION::MENU_UP] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_UP},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::TEN_UP},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_UP},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_UP},
    };

    //���j���[��
    inputActionMaps_[ACTION::MENU_DOWN] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_DOWN},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::TEN_DOWN},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_DOWN},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_DOWN},
    };

    //���j���[��
    inputActionMaps_[ACTION::MENU_LEFT] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_LEFT},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::TEN_LEFT},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_LEFT},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_LEFT},
    };

    //���j���[�E
    inputActionMaps_[ACTION::MENU_RIGHT] =
    {
        {INPUT_TYPE::KEYBOARD ,KEY_INPUT_RIGHT},
        {INPUT_TYPE::GAMEPAD_BUTTON,(int)JOYPAD_BTN::TEN_RIGHT},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_RIGHT},
        {INPUT_TYPE::GAMEPAD_STICK,(int)JOYPAD_STICK::R_STICK_RIGHT},
    };
}

InputManager::~InputManager(void)
{
    for (auto& iam : inputActionMaps_)
    {
        iam.second.clear();
    }
    inputActionMaps_.clear();

    currentInput_.clear();
    lastInput_.clear();
}

void InputManager::Update(void)
{
    //1f�O�̓��͏�ԂƂ��Ċi�[
    lastInput_ = currentInput_;

    //���͏��X�V
    inputUpdater_->Update(); 
    
    //���ꂼ��̃A�N�V�������Ɋ��蓖�ĂĂ���S�Ă̓��̓`�F�b�N
    for (const auto& mapInfo : inputActionMaps_)
    {
        bool isPressed = false;
        for (const auto& inputInfo : mapInfo.second)
        {            
            isPressed = inputUpdater_->GetHitKey(inputInfo);
            if (isPressed)
            {
                //���͂��Ă���
                currentInputType_ = inputInfo.type;
                break;
            }
        }

        //���͂��Ă��Ȃ�
        currentInput_[mapInfo.first] = isPressed;
    }
}

JOYPAD_TYPE InputManager::GetJPadType(JOYPAD_NO no)
{
    return (JOYPAD_TYPE)(GetJoypadType((int)no));
}

void InputManager::GetMousePos(Vector2& pos)
{
    GetMousePoint(&pos.x, &pos.y);
}

void InputManager::SetControlType(CONTROL_TYPE type)
{
    controlType_ = type;
}

CONTROL_TYPE InputManager::GetControlType(void)
{
    return controlType_;
}

int InputManager::GetJPadNum(void)
{
    return GetJoypadNum();
}

const std::unordered_map<ACTION, bool>& InputManager::GetCurrentInput(void)
{
   return currentInput_;
}

const bool InputManager::GetCurrentInput(ACTION act)
{
    if (!currentInput_.contains(act))
    {
        //�A�N�V�������o�^����Ă��Ȃ�
        return false;
    }

    return currentInput_.at(act);
}

const std::unordered_map<ACTION, bool>& InputManager::GetLastInput(void)
{
    return lastInput_;
}

const bool InputManager::GetLastInput(ACTION act)
{
    if (!lastInput_.contains(act))
    {
        //�A�N�V�������o�^����Ă��Ȃ�
        return false;
    }

    return lastInput_.at(act);
}




