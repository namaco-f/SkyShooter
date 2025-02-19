
#include <algorithm>
#include "../Object/Actor/Shot/ShotFactory.h"
#include "ShotManager.h"

ShotManager* Singleton<ShotManager>::instance_ = nullptr;

const int DEF_SHOT_COUNT = 1;

void ShotManager::Update(void)
{
	if (shots_.empty()) 
	{
		//�z�񂪋�
		return; 
	}

	//�g���Ă��Ȃ��v�f�̍폜
	std::erase_if(shots_, [&](std::unique_ptr<ShotBase>& shot) {return !shot->IsAlive(); });
}

void ShotManager::AddShot(std::function<std::unique_ptr<ShotBase>()> createFunc)
{
	//�L�����N�^�[�z��ɒǉ�
	shots_.emplace_back(createFunc());

	auto objTag = shots_.back()->GetObjectType();
	if (!shotsNums_.contains(objTag))
	{
		//�����̔z��ɑ��݂��Ȃ�
		shotsNums_.emplace(objTag, DEF_SHOT_COUNT);
	}
	else
	{
		//���݂��Ă��邽�߁A�������Z
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
