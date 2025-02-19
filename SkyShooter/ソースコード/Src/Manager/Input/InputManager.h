#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <Dxlib.h>
#include "InputControllerTag.h"
#include "../../Singleton.h"

class InputUpdater;

/// <summary>
/// 入力制御の登録
/// </summary>
class InputManager
	: public Singleton<InputManager>
{
public:

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// ジョイパッドの種類を取得
	/// </summary>
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no);

	/// <summary>
	/// マウス座標を取得
	/// </summary>
	/// <param name="pos">マウス座標を書き込むVector2</param>
	void GetMousePos(Vector2& pos);

	/// <summary>
	/// 操作形式を指定
	/// </summary>
	void SetControlType(CONTROL_TYPE type);

	/// <summary>
	/// 操作形式を取得
	/// </summary>
	CONTROL_TYPE GetControlType(void);

	/// <summary>
	/// 現在の入力情報配列を取得
	/// </summary>
	/// <returns>currentInput_</returns>
	const std::unordered_map<ACTION, bool>& GetCurrentInput(void);

	/// <summary>
	/// 指定したアクションの入力情報を取得
	/// </summary>
	/// <param name="act">アクション</param>
	/// <returns>入力判定</returns>
	const bool GetCurrentInput(ACTION act);
	
	/// <summary>
	/// 1f前の入力情報配列を取得
	/// </summary>
	/// <returns>lastInput_</returns>
	const std::unordered_map<ACTION, bool>& GetLastInput(void);

	/// <summary>
	/// 指定したアクションの1f前の入力情報を取得
	/// </summary>
	/// <param name="act">アクション</param>
	/// <returns>入力判定</returns>
	const bool GetLastInput(ACTION act);

private:

	friend class Singleton<InputManager>;

	//入力の更新
	std::unique_ptr<InputUpdater> inputUpdater_;

	//入力とアクションの対応テーブル
	std::unordered_map<ACTION, std::vector<InputMapInfo>> inputActionMaps_;

	//現在の入力状態
	std::unordered_map<ACTION, bool> currentInput_;

	//入力の種類
	INPUT_TYPE currentInputType_;

	//直前の入力状態
	std::unordered_map<ACTION, bool> lastInput_;

	//操作形式
	CONTROL_TYPE controlType_;

	InputManager(void);
	virtual ~InputManager(void) override;
	
	//接続されているコントローラーの数を返す
	int GetJPadNum(void);

};
