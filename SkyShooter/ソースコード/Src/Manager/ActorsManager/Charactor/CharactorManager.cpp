
#include "../Object/Actor/Charactor/Player/PlayerUnit01.h"
#include "../Object/Actor/Charactor/Enemy/EnemyUnit01.h"
#include "../Object/Actor/Charactor/CharactorFactory.h"
#include "../Utility/MathUtility.h"
#include "CharactorManager.h"

CharactorManager* Singleton<CharactorManager>::instance_ = nullptr;

const int UNEXIST_CHARACTOR = MathUtility::INT_ZERO;		//�L�����N�^�[�����݂��Ă��Ȃ�

const int DEF_CHARACTOR_COUNT = 1;

void CharactorManager::Update(void)
{
}

void CharactorManager::Draw(void)
{
}

const int CharactorManager::GetCharactorNum(const OBJECT_TYPE objTag) const
{
	if (!charactorsNums_.contains(objTag))
	{
		//�L�����N�^�[���o�^����Ă��Ȃ�
		return UNEXIST_CHARACTOR;
	}

	return charactorsNums_.at(objTag);
}

void CharactorManager::AddCharactor(std::function<std::shared_ptr<CharactorBase>()> func)
{
	//�L�����N�^�[�z��ɒǉ�
	charactors_.emplace_back(func());
	
	auto objTag = charactors_.back()->GetObjectType();
	if (!charactorsNums_.contains(objTag))
	{
		//�����̔z��ɑ��݂��Ȃ�
		charactorsNums_.emplace(objTag, DEF_CHARACTOR_COUNT);
	}
	else
	{
		//���݂��Ă��邽�߁A�������Z
		charactorsNums_.at(objTag)++;	
	}
}

CharactorManager::CharactorManager(void)
	:
	charactors_{},
	charactorsNums_{}
{
}

CharactorManager::~CharactorManager(void)
{
	ClearCharactors();
}
