#pragma once
#pragma once

#include <memory>
#include "../Object/Common/Transform.h"
class ActorBase;


/// <summary>
/// ƒLƒƒƒ‰‚Ì“o˜^
/// </summary>
/// <typeparam name="T"></typeparam>
template<class T>
std::shared_ptr<ActorBase> CreateActor(void)
{
	return std::move(std::make_shared<T>());
}


