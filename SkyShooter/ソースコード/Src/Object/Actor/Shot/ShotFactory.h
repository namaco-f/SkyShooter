#pragma once

#include <memory>
#include "../Object/Common/Transform.h"
class ShotBase;
class Observer;

/// <summary>
/// ƒLƒƒƒ‰‚Ì“o˜^
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template<class T>
std::unique_ptr<ShotBase> Create(const Transform& trans)
{
	return std::move(std::make_unique<T>(trans));
}
