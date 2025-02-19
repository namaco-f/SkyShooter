#pragma once

#include <string>
#include "../../Common/Vector2/Vector2.h"
#include "../../Common/Vector2/Vector2f.h"

/// <summary>
/// �W���C�p�b�h�̔ԍ�
/// </summary>
enum class JOYPAD_NO
{
	KEY_PAD1,
	PAD1,
	PAD2,
	PAD3,
	PAD4,
	INPUT_KEY = 4096,
};


/// <summary>
/// �W���C�p�b�h�{�^��
/// </summary>
enum class JOYPAD_BTN
{
	LEFT = 0,
	RIGHT,
	TOP,
	DOWN,
	R_TRIGGER,
	ZR_TRIGGER,
	R3_TRIGGER,
	L_TRIGGER,
	ZL_TRIGGER,
	L3_TRIGGER,
	START,
	CENTER,
	TEN_UP,
	TEN_DOWN,
	TEN_LEFT,
	TEN_RIGHT,
	MAX
};

/// <summary>
/// �W���C�p�b�h�̃X�e�B�b�N
/// </summary>
enum class JOYPAD_STICK
{
	L_STICK_UP = 0,
	L_STICK_UPLEFT,
	L_STICK_UPRIGHT,
	L_STICK_DOWN,
	L_STICK_DOWNLEFT,
	L_STICK_DOWNRIGHT,
	L_STICK_LEFT,
	L_STICK_RIGHT,

	R_STICK_UP,
	R_STICK_DOWN,
	R_STICK_LEFT,
	R_STICK_RIGHT,

	MAX
};

/// <summary>
/// �W���C�p�b�h�̎��
/// </summary>
enum class JOYPAD_TYPE
{
	OTHER = 0,
	XBOX_360,
	XBOX_ONE,			//XboxOne
	DUAL_SHOCK_3,
	DUAL_SHOCK_4,
	DUAL_SENCE,
	SWICH_JOY_CON_L,
	SWICH_JOY_CON_R,
	SWICH_JOY_PRO_CTRL,
	SWICH_HORI_PAD,
	MAX,
};

/// <summary>
/// �A�N�V�����e�[�u��
/// </summary>
enum class ACTION
{
	UN_REGIST,			//���o�^
	SCENE_CHANGE,		//�����V�[���J��
	GAME_START,			//�Q�[���X�^�[�g
	
	MOVE_FORWARD,		//�O�ړ�
	MOVE_BACK,			//��ړ�
	MOVE_LEFT,			//���ړ�
	MOVE_RIGHT,			//�E�ړ�
	MOVE_UP,			//��ړ�
	
	CAMERA_UP,			//�J������
	CAMERA_DOWN,		//�J������
	CAMERA_LEFT,		//�J������
	CAMERA_RIGHT,		//�J�����E
	
	PLAYER_RAN,			//�v���C���[�ʏ�ړ�
	PLAYER_DASH,		//�v���C���[�_�b�V��
	PLAYER_IDLE,		//�Î~���
	PLAYER_LOCK_SWITCH,	//�v���C���[���b�N�I��
	PLAYER_ATTACK,		//�v���C���[�U��
	PLAYER_ATTACK2,		//�v���C���[�U��2
	PLAYER_ULTATTACK,	//�v���C���[�U��ULT
	PLAYER_SHIELD,		//�v���C���[�V�[���h

	MENU_OPEN,			//���j���[�J
	MENU_YES,			//����
	MENU_NO,			//����
	MENU_ENTER,			//����

	MENU_UP,			//���j���[��
	MENU_DOWN,			//���j���[��
	MENU_LEFT,			//���j���[��
	MENU_RIGHT,			//���j���[�E

	MAX,
};

/// <summary>
/// ���͎��
/// </summary>
enum class INPUT_TYPE
{
	KEYBOARD,
	GAMEPAD_BUTTON,
	MOUSE,
	GAMEPAD_STICK,
	MAX,
};

/// <summary>
/// ���͏��
/// </summary>
struct InputMapInfo
{
	//���͎��
	INPUT_TYPE type;

	//�L�[����
	int buttonID_;
};

/// <summary>
/// �R���g���[���[�̃^�C�v
/// </summary>
enum class CONTROL_TYPE
{
	KEY,
	CONTROLLER,
	MAX
};