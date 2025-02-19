#pragma once

#include <DxLib.h>
#include <variant>
#include "../Manager/UIManager/UIDataManager.h"
#include "UIType.h"

/// <summary>
/// std::variant��UIData���w�肵���^�ɕϊ�
/// </summary>
/// <typeparam name="T">�^ : int,float,VECTOR</typeparam>
/// <param name="src">UI�f�[�^��</param>
/// <returns>T�ƌ^����v���Ă���ꍇ�̂݃f�[�^��Ԃ�</returns>
template<typename T>
const T GetUIDataToVariant(const SRC_UIData src)
{
	auto& uiMng = UIDataManager::GetInstance();
	const auto& data = uiMng.GetUIData(src);
	if (!std::holds_alternative<T>(data))
	{
		//�^����v������̂��Ȃ�
		return T();
	}

	return 	std::get<T>(data);
}
