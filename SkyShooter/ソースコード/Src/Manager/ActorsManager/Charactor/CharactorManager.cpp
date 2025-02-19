
#include "../Object/Actor/Charactor/Player/PlayerUnit01.h"
#include "../Object/Actor/Charactor/Enemy/EnemyUnit01.h"
#include "../Object/Actor/Charactor/CharactorFactory.h"
#include "../Utility/MathUtility.h"
#include "CharactorManager.h"

CharactorManager* Singleton<CharactorManager>::instance_ = nullptr;

const int UNEXIST_CHARACTOR = MathUtility::INT_ZERO;		//キャラクターが存在していない

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
		//キャラクターが登録されていない
		return UNEXIST_CHARACTOR;
	}

	return charactorsNums_.at(objTag);
}

void CharactorManager::AddCharactor(std::function<std::shared_ptr<CharactorBase>()> func)
{
	//キャラクター配列に追加
	charactors_.emplace_back(func());
	
	auto objTag = charactors_.back()->GetObjectType();
	if (!charactorsNums_.contains(objTag))
	{
		//既存の配列に存在しない
		charactorsNums_.emplace(objTag, DEF_CHARACTOR_COUNT);
	}
	else
	{
		//存在しているため、数を加算
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
