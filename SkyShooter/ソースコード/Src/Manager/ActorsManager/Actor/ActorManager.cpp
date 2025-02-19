
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
	//�L�����N�^�[�z��ɒǉ�
	actors_.emplace_back(createFunc());

	auto objTag = actors_.back()->GetObjectType();
	if (!actorsNums_.contains(objTag))
	{
		//�����̔z��ɑ��݂��Ȃ�
		actorsNums_.emplace(objTag, DEF_ACTOR_COUNT);
	}
	else
	{
		//���݂��Ă��邽�߁A�������Z
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
