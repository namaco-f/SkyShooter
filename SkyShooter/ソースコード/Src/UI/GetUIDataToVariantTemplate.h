#pragma once

#include <DxLib.h>
#include <variant>
#include "../Manager/UIManager/UIDataManager.h"
#include "UIType.h"

/// <summary>
/// std::variant‚ÌUIData‚ğw’è‚µ‚½Œ^‚É•ÏŠ·
/// </summary>
/// <typeparam name="T">Œ^ : int,float,VECTOR</typeparam>
/// <param name="src">UIƒf[ƒ^–¼</param>
/// <returns>T‚ÆŒ^‚ªˆê’v‚µ‚Ä‚¢‚éê‡‚Ì‚İƒf[ƒ^‚ğ•Ô‚·</returns>
template<typename T>
const T GetUIDataToVariant(const SRC_UIData src)
{
	auto& uiMng = UIDataManager::GetInstance();
	const auto& data = uiMng.GetUIData(src);
	if (!std::holds_alternative<T>(data))
	{
		//Œ^‚ªˆê’v‚·‚é‚à‚Ì‚ª‚È‚¢
		return T();
	}

	return 	std::get<T>(data);
}
