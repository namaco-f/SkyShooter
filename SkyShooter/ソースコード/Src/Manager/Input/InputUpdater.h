#pragma once

#include <Dxlib.h>
#include "InputControllerTag.h"

/// <summary>
/// 入力情報の更新
/// </summary>
class InputUpdater
{

public:

	/// <summary>
	/// ジョイパッドの入力情報
	/// </summary>
	struct JOYPAD_IN_STATE
	{
		unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];		//1f前のボタン入力
		unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];		//現フレームのボタン入力

		bool aKeyOld[static_cast<int>(JOYPAD_STICK::MAX)];					//1f前のスティック入力
		bool aKeyNew[static_cast<int>(JOYPAD_STICK::MAX)];					//現フレームのスティック入力

		Vector2 AkeyL = {};				//現在のLスティックの入力情報
		Vector2 AkeyLOld = {};			//1f前のLスティックの入力情報
		Vector2 AkeyR = {};				//現在のRスティックの入力情報
		Vector2 AkeyROld = {};			//1f前のRスティックの入力情報

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
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 対象の入力が行われたか確認
	/// </summary>
	/// <param name="info">キー情報</param>
	/// <returns>判定結果</returns>
	bool GetHitKey(const InputMapInfo& info);

private:

	//JoyPadの入力情報
	JOYPAD_IN_STATE joyPadState_;

	//DirectInputから得られる入力データ
	DINPUT_JOYSTATE dstate_;
	
	//XInputから得られる入力データ
	XINPUT_STATE xstate_;

	//キー入力情報
	char keyState_[256];

	//パッド入力情報
	int padState_;

	//マウス入力情報
	int mouseState_;

	//スティックの感度
	float controllerSensitivity_;

	//対象のJoyPadの入力情報を更新
	void SetJPadInState(JOYPAD_NO no);

	//現在のパッドの入力情報を取得
	JOYPAD_IN_STATE GetJPadState(JOYPAD_NO no);

	//ジョイパッドの種類を取得
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no);

	//DirectInputから得られる入力データを取得
	DINPUT_JOYSTATE GetJPadDInputState(int no);
	
	//XInputから得られる入力データを取得
	XINPUT_STATE GetJPadXInputState(int no);

	//JoyPadの入力検知
	bool GetHitJPad(const InputMapInfo& info, const int& pad);

	//JoyPadのスティックの入力データを取得
	bool GetHitJPadStick(const InputMapInfo& info, const int& pad);

	//JoyPadのスティックの入力を確認
	bool IsInputStick(const int stick, const int max);

	//JoyPadのスティックの入力を確認:斜め入力用
	bool IsInputStick(const Vector2& stick, const int xMax, const int yMax);

};

