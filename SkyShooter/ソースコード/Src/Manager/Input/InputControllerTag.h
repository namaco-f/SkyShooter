#pragma once

#include <string>
#include "../../Common/Vector2/Vector2.h"
#include "../../Common/Vector2/Vector2f.h"

/// <summary>
/// ジョイパッドの番号
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
/// ジョイパッドボタン
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
/// ジョイパッドのスティック
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
/// ジョイパッドの種別
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
/// アクションテーブル
/// </summary>
enum class ACTION
{
	UN_REGIST,			//未登録
	SCENE_CHANGE,		//強制シーン遷移
	GAME_START,			//ゲームスタート
	
	MOVE_FORWARD,		//前移動
	MOVE_BACK,			//後移動
	MOVE_LEFT,			//左移動
	MOVE_RIGHT,			//右移動
	MOVE_UP,			//上移動
	
	CAMERA_UP,			//カメラ上
	CAMERA_DOWN,		//カメラ下
	CAMERA_LEFT,		//カメラ左
	CAMERA_RIGHT,		//カメラ右
	
	PLAYER_RAN,			//プレイヤー通常移動
	PLAYER_DASH,		//プレイヤーダッシュ
	PLAYER_IDLE,		//静止状態
	PLAYER_LOCK_SWITCH,	//プレイヤーロックオン
	PLAYER_ATTACK,		//プレイヤー攻撃
	PLAYER_ATTACK2,		//プレイヤー攻撃2
	PLAYER_ULTATTACK,	//プレイヤー攻撃ULT
	PLAYER_SHIELD,		//プレイヤーシールド

	MENU_OPEN,			//メニュー開
	MENU_YES,			//承諾
	MENU_NO,			//拒否
	MENU_ENTER,			//決定

	MENU_UP,			//メニュー上
	MENU_DOWN,			//メニュー下
	MENU_LEFT,			//メニュー左
	MENU_RIGHT,			//メニュー右

	MAX,
};

/// <summary>
/// 入力種別
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
/// 入力情報
/// </summary>
struct InputMapInfo
{
	//入力種別
	INPUT_TYPE type;

	//キー識別
	int buttonID_;
};

/// <summary>
/// コントローラーのタイプ
/// </summary>
enum class CONTROL_TYPE
{
	KEY,
	CONTROLLER,
	MAX
};