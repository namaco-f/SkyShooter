#pragma once

#pragma once

#include <memory>

class UIBase;

/// <summary>
/// ƒLƒƒƒ‰‚Ì“o˜^
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template<class T>
std::unique_ptr<UIBase> CreateUI(void)
{
	return std::move(std::make_unique<T>());
}

