#pragma once

#include <DxLib.h>
#include <variant>
#include "../Manager/UIManager/UIDataManager.h"
#include "UIType.h"

/// <summary>
/// std::variantのUIDataを指定した型に変換
/// </summary>
/// <typeparam name="T">型 : int,float,VECTOR</typeparam>
/// <param name="src">UIデータ名</param>
/// <returns>Tと型が一致している場合のみデータを返す</returns>
template<typename T>
const T GetUIDataToVariant(const SRC_UIData src)
{
	auto& uiMng = UIDataManager::GetInstance();
	const auto& data = uiMng.GetUIData(src);
	if (!std::holds_alternative<T>(data))
	{
		//型が一致するものがない
		return T();
	}

	return 	std::get<T>(data);
}
