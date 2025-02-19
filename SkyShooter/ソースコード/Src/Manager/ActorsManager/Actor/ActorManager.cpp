
#include "../Object/Actor/ActorFactory.h"
#include "../Object/Actor/ActorBase.h"
#include "ActorManager.h"

ActorManager* Singleton<ActorManager>::instance_ = nullptr;

const int DEF_ACTOR_COUNT = 1;

void ActorManager::Update(void)
{
}

void ActorManager::Draw(void)
{
}

void ActorManager::AddActor(std::function<std::shared_ptr<ActorBase>()> createFunc)
{
	//キャラクター配列に追加
	actors_.emplace_back(createFunc());

	auto objTag = actors_.back()->GetObjectType();
	if (!actorsNums_.contains(objTag))
	{
		//既存の配列に存在しない
		actorsNums_.emplace(objTag, DEF_ACTOR_COUNT);
	}
	else
	{
		//存在しているため、数を加算
		actorsNums_.at(objTag)++;
	}
}

ActorManager::ActorManager(void)
	:
	actors_{},
	actorsNums_{}
{
}

ActorManager::~ActorManager(void)
{
	ClearActors();
}
