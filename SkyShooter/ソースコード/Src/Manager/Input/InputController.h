#pragma once

#include <unordered_map>
#include "InputControllerTag.h"
#include <DxLib.h>

class InputManager;

/// <summary>
/// 入力検知クラス
/// </summary>
class InputController
{
public:

	InputController(void);
	virtual ~InputController(void);

	/// <summary>
	/// 入力チェック
	/// </summary>
	/// <param name="action">アクションの名前</param>
	/// <returns>
	/// true : 押されている
	/// false : 押されてない
	/// </returns>
	bool IsPressed(const ACTION& action) const;

	/// <summary>
	/// 入力チェック
	/// </summary>
	/// <param name="action">調べたいアクション</param>
	/// <returns>
	/// true : 今押された
	/// false : 押されてないか押しっぱなし
	/// </returns>
	bool IsTriggered(const ACTION& action) const;

	/// <summary>
	/// 入力チェック
	/// </summary>
	/// <param name="action">調べたいアクション</param>
	/// <returns>
	/// true:今離された
	/// </returns>
	bool IsReleased(const ACTION& action) const;

private:

	//入力の登録情報
	InputManager& inputMng_;
};