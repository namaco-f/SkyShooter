#include "InputUpdater.h"

const int DEF_STICK_NEUTRAL_XBOX_ONE = 0;
const int DEF_STICK_X_LEFT_XBOX_ONE = -1000;
const int DEF_STICK_X_RIGHT_XBOX_ONE = 1000;
const int DEF_STICK_Y_UP_XBOX_ONE = -1000;
const int DEF_STICK_Y_DOWN_XBOX_ONE = 1000;

const float DEF_CONTROLLER_SENSITIVITY = 0.4f;

const int IDX_XBOX_ONE_BUTTON_A = 0;
const int IDX_XBOX_ONE_BUTTON_B = 1;
const int IDX_XBOX_ONE_BUTTON_X = 2;
const int IDX_XBOX_ONE_BUTTON_Y = 3;
const int IDX_XBOX_ONE_BUTTON_L = 4;
const int IDX_XBOX_ONE_BUTTON_R = 5;
const int IDX_XBOX_ONE_BUTTON_CENTER = 6;
const int IDX_XBOX_ONE_BUTTON_START = 7;

InputUpdater::InputUpdater(void)
    :
    joyPadState_{},
    dstate_{},
    xstate_{},
    keyState_{},
    padState_{},
    mouseState_{},
    controllerSensitivity_(DEF_CONTROLLER_SENSITIVITY)
{
}

InputUpdater::~InputUpdater(void)
{
}

void InputUpdater::Update(void)
{
    SetJPadInState(JOYPAD_NO::PAD1);

    //�ЂƂ܂��S�Ă̓��͏����擾
    GetHitKeyStateAll(keyState_);

    padState_ = GetJoypadInputState(DX_INPUT_PAD1);
    mouseState_ = GetMouseInput();
}

bool InputUpdater::GetHitKey(const InputMapInfo& info)
{
    //�L�[�{�[�h
    //�R���g���[���[(�{�^��)
    //�R���g���[���[(�X�e�B�b�N)
    //�}�E�X
    return
        (info.type == INPUT_TYPE::KEYBOARD && keyState_[info.buttonID_]) 
        || (GetHitJPad(info, padState_))
        || (GetHitJPadStick(info, padState_)) 
        || (info.type == INPUT_TYPE::MOUSE && mouseState_ & info.buttonID_);
}

void InputUpdater::SetJPadInState(JOYPAD_NO no)
{
    auto stateNew = GetJPadState(no);

    auto& stateNow = joyPadState_;

    //�{�^���̓��͏����X�V
    int max = static_cast<int>(JOYPAD_BTN::MAX);
    for (int n = 0; n < max; n++)
    {
        stateNow.ButtonsOld[n] = stateNow.ButtonsNew[n];
        stateNow.ButtonsNew[n] = stateNew.ButtonsNew[n];

        stateNow.AkeyL.x = stateNew.AkeyL.x;
        stateNow.AkeyL.y = stateNew.AkeyL.y;
        stateNow.AkeyR.x = stateNew.AkeyR.x;
        stateNow.AkeyR.y = stateNew.AkeyR.y;
    }

    //�X�e�B�b�N�̓��͏����X�V
    max = static_cast<int>(JOYPAD_STICK::MAX);
    for (int n = 0; n < max; n++)
    {
        stateNow.aKeyOld[n] = stateNew.aKeyNew[n];
        stateNow.aKeyNew[n] = stateNew.aKeyNew[n];
    }
}

InputUpdater::JOYPAD_IN_STATE InputUpdater::GetJPadState(JOYPAD_NO no)
{
    JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

    //�W���C�p�b�h�̎�ʂ��擾
    auto type = GetJPadType(no);

    if (type == JOYPAD_TYPE::XBOX_ONE)
    {
        //XboxOne�R���g���[���[
        auto d = GetJPadDInputState((int)no);
        auto x = GetJPadXInputState((int)no);

        int idx;

        //Y
        idx = static_cast<int>(JOYPAD_BTN::TOP);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_Y];

        //X
        idx = static_cast<int>(JOYPAD_BTN::LEFT);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_X];

        //B
        idx = static_cast<int>(JOYPAD_BTN::RIGHT);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_B];

        //A
        idx = static_cast<int>(JOYPAD_BTN::DOWN);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_A];

        //L
        idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_L];

        //R
        idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_R];

        //XBox�{�^��
        idx = static_cast<int>(JOYPAD_BTN::CENTER);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_CENTER];

        //Start
        idx = static_cast<int>(JOYPAD_BTN::START);
        ret.ButtonsNew[idx] = d.Buttons[IDX_XBOX_ONE_BUTTON_START];

        //ZR
        idx = static_cast<int>(JOYPAD_BTN::ZR_TRIGGER);
        ret.ButtonsNew[idx] = x.RightTrigger;

        //ZL
        idx = static_cast<int>(JOYPAD_BTN::ZL_TRIGGER);
        ret.ButtonsNew[idx] = x.LeftTrigger;

        //�\���L�[��
        idx = static_cast<int>(JOYPAD_BTN::TEN_UP);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_UP];

        //�\���L�[��
        idx = static_cast<int>(JOYPAD_BTN::TEN_DOWN);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_DOWN];

        //�\���L�[��
        idx = static_cast<int>(JOYPAD_BTN::TEN_LEFT);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_LEFT];

        //�\���L�[�E
        idx = static_cast<int>(JOYPAD_BTN::TEN_RIGHT);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_RIGHT];

        //L3
        idx = static_cast<int>(JOYPAD_BTN::L3_TRIGGER);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_LEFT_THUMB];

        //R3
        idx = static_cast<int>(JOYPAD_BTN::R3_TRIGGER);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_RIGHT_THUMB];

        // ���X�e�B�b�N
        ret.AkeyL.x = d.X;
        ret.AkeyL.y = d.Y;

        // �E�X�e�B�b�N
        ret.AkeyR.x = d.Rx;
        ret.AkeyR.y = d.Ry;


        //���X�e�B�b�N�֘A
        //������
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_LEFT);
        bool isHit = IsInputStick(ret.AkeyL.x, DEF_STICK_X_LEFT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //�E����
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_RIGHT);
        isHit = IsInputStick(ret.AkeyL.x, DEF_STICK_X_RIGHT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //�����
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_UP);
        isHit = IsInputStick(ret.AkeyL.y, DEF_STICK_Y_UP_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //�㍶����
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_UPLEFT);
        isHit = IsInputStick(ret.AkeyL, DEF_STICK_X_LEFT_XBOX_ONE, DEF_STICK_Y_UP_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //��E����
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_UPRIGHT);
        isHit = IsInputStick(ret.AkeyL, DEF_STICK_X_RIGHT_XBOX_ONE, DEF_STICK_Y_UP_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //������
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_DOWN);
        isHit = IsInputStick(ret.AkeyL.y, DEF_STICK_Y_DOWN_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //��������
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_DOWNLEFT);
        isHit = IsInputStick(ret.AkeyL, DEF_STICK_X_LEFT_XBOX_ONE, DEF_STICK_Y_DOWN_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //���E����
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_DOWNRIGHT);
        isHit = IsInputStick(ret.AkeyL, DEF_STICK_X_RIGHT_XBOX_ONE, DEF_STICK_Y_DOWN_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //�X�e�B�b�N�֘A
        //������
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_LEFT);
        isHit = IsInputStick(ret.AkeyR.x, DEF_STICK_X_LEFT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //�E����
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_RIGHT);
        isHit = IsInputStick(ret.AkeyR.x, DEF_STICK_X_RIGHT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //�����
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_UP);
        isHit = IsInputStick(ret.AkeyR.y, DEF_STICK_Y_UP_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //������
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_DOWN);
        isHit = IsInputStick(ret.AkeyR.y, DEF_STICK_Y_DOWN_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;
    }

    return ret;
}

JOYPAD_TYPE InputUpdater::GetJPadType(JOYPAD_NO no)
{
    return (JOYPAD_TYPE)(GetJoypadType((int)no));
}

DINPUT_JOYSTATE InputUpdater::GetJPadDInputState(int no)
{
    GetJoypadDirectInputState(no, &dstate_);
    return dstate_;
}

XINPUT_STATE InputUpdater::GetJPadXInputState(int no)
{
    GetJoypadXInputState(no, &xstate_);
    return xstate_;
}

bool InputUpdater::GetHitJPad(const InputMapInfo& info, const int& pad)
{
    bool ret = false;

    if (info.type != INPUT_TYPE::GAMEPAD_BUTTON)
    {
        return ret;
    }

    if (joyPadState_.ButtonsNew[info.buttonID_])
    {
        //�{�^�����o�^����Ă���
        ret = true;
    }

    return ret;
}

bool InputUpdater::GetHitJPadStick(const InputMapInfo& info, const int& pad)
{
    if (info.type != INPUT_TYPE::GAMEPAD_STICK)
    {
        //�X�e�B�b�N����ł͂Ȃ�
        return false;
    }

    return joyPadState_.aKeyNew[info.buttonID_];
}

bool InputUpdater::IsInputStick(const int stick, const int max)
{
    auto stickMax = (int)((float)max * controllerSensitivity_);

    //�����̈Ⴂ���m�F
    return
        stickMax < DEF_STICK_NEUTRAL_XBOX_ONE
        ? stick < stickMax
        : stick > stickMax;
}

bool InputUpdater::IsInputStick(const Vector2& stick, const int xMax, const int yMax)
{
    //x����y�����ꂼ�ꔻ��
    return IsInputStick(stick.x, xMax) && IsInputStick(stick.y, yMax);
}
