#pragma once

#include <Dxlib.h>
#include "InputControllerTag.h"

/// <summary>
/// ���͏��̍X�V
/// </summary>
class InputUpdater
{

public:

	/// <summary>
	/// �W���C�p�b�h�̓��͏��
	/// </summary>
	struct JOYPAD_IN_STATE
	{
		unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];		//1f�O�̃{�^������
		unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];		//���t���[���̃{�^������

		bool aKeyOld[static_cast<int>(JOYPAD_STICK::MAX)];					//1f�O�̃X�e�B�b�N����
		bool aKeyNew[static_cast<int>(JOYPAD_STICK::MAX)];					//���t���[���̃X�e�B�b�N����

		Vector2 AkeyL = {};				//���݂�L�X�e�B�b�N�̓��͏��
		Vector2 AkeyLOld = {};			//1f�O��L�X�e�B�b�N�̓��͏��
		Vector2 AkeyR = {};				//���݂�R�X�e�B�b�N�̓��͏��
		Vector2 AkeyROld = {};			//1f�O��R�X�e�B�b�N�̓��͏��

		JOYPAD_IN_STATE()
			:
			ButtonsOld{},
			ButtonsNew{},
			aKeyOld{},
			aKeyNew{}
		{};
	};


	InputUpdater(void);
	virtual ~InputUpdater(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �Ώۂ̓��͂��s��ꂽ���m�F
	/// </summary>
	/// <param name="info">�L�[���</param>
	/// <returns>���茋��</returns>
	bool GetHitKey(const InputMapInfo& info);

private:

	//JoyPad�̓��͏��
	JOYPAD_IN_STATE joyPadState_;

	//DirectInput���瓾������̓f�[�^
	DINPUT_JOYSTATE dstate_;
	
	//XInput���瓾������̓f�[�^
	XINPUT_STATE xstate_;

	//�L�[���͏��
	char keyState_[256];

	//�p�b�h���͏��
	int padState_;

	//�}�E�X���͏��
	int mouseState_;

	//�X�e�B�b�N�̊��x
	float controllerSensitivity_;

	//�Ώۂ�JoyPad�̓��͏����X�V
	void SetJPadInState(JOYPAD_NO no);

	//���݂̃p�b�h�̓��͏����擾
	JOYPAD_IN_STATE GetJPadState(JOYPAD_NO no);

	//�W���C�p�b�h�̎�ނ��擾
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no);

	//DirectInput���瓾������̓f�[�^���擾
	DINPUT_JOYSTATE GetJPadDInputState(int no);
	
	//XInput���瓾������̓f�[�^���擾
	XINPUT_STATE GetJPadXInputState(int no);

	//JoyPad�̓��͌��m
	bool GetHitJPad(const InputMapInfo& info, const int& pad);

	//JoyPad�̃X�e�B�b�N�̓��̓f�[�^���擾
	bool GetHitJPadStick(const InputMapInfo& info, const int& pad);

	//JoyPad�̃X�e�B�b�N�̓��͂��m�F
	bool IsInputStick(const int stick, const int max);

	//JoyPad�̃X�e�B�b�N�̓��͂��m�F:�΂ߓ��͗p
	bool IsInputStick(const Vector2& stick, const int xMax, const int yMax);

};

