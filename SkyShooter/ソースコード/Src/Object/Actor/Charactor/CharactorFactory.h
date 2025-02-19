#pragma once

#include <memory>
class CharactorBase;

/// <summary>
/// �L�����̓o�^
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template<class T>
std::shared_ptr<CharactorBase> Create(void)
{
	return std::move(std::make_shared<T>());
}
