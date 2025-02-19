
#include <algorithm>
#include "../Object/Actor/Shot/ShotFactory.h"
#include "ShotManager.h"

ShotManager* Singleton<ShotManager>::instance_ = nullptr;

const int DEF_SHOT_COUNT = 1;

void ShotManager::Update(void)
{
	if (shots_.empty()) 
	{
		//配列が空
		return; 
	}

	//使われていない要素の削除
	std::erase_if(shots_, [&](std::unique_ptr<ShotBase>& shot) {return !shot->IsAlive(); });
}

void ShotManager::AddShot(std::function<std::unique_ptr<ShotBase>()> createFunc)
{
	//キャラクター配列に追加
	shots_.emplace_back(createFunc());

	auto objTag = shots_.back()->GetObjectType();
	if (!shotsNums_.contains(objTag))
	{
		//既存の配列に存在しない
		shotsNums_.emplace(objTag, DEF_SHOT_COUNT);
	}
	else
	{
		//存在しているため、数を加算
		shotsNums_.at(objTag)++;
	}
}

ShotManager::ShotManager(void)
{
}

ShotManager::~ShotManager(void)
{
	ClearShots();
}
