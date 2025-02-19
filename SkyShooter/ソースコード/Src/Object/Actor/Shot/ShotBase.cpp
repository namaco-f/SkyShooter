
#include "../Manager/TimeManager.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Effect/EffectController.h"
#include "../Utility/MathUtility.h"
#include "../Observer/Observer.h"
#include "ShotBase.h"

using namespace std;

using Math = MathUtility;

const float INIT_SHOTSPEED = 30.0f;
const float INIT_SHOT_AlIVE_CNT = 3.0f;

ShotBase::ShotBase(const Transform& trans)
	:
	ActorBase(),
	timeMng_(TimeManager::GetInstance()),
	aliveCnt_(INIT_SHOT_AlIVE_CNT),
	deltaTime_(TimeManager::GetInstance().GetDeltaTime()),
	targetPos_{}
{
	transform_ = trans;
}

void ShotBase::Update(void)
{
	deltaTime_ = timeMng_.GetDeltaTime();

	movedPos_ = transform_.pos;
	
	aliveCnt_ -= deltaTime_;
	
	if (!IsAlive() && !IsDestroy())
	{
		Destroy();
	}

	UpdateState();

	UpdateTransform();

	UpdateEnd();
}

void ShotBase::SetTargetPos(const VECTOR& targetPos)
{
	targetPos_ = targetPos;
}

void ShotBase::SetShotPos(const VECTOR& shotPos)
{
	transform_.pos = shotPos;
}

void ShotBase::UpdateTransform(void)
{
	transform_.pos = movedPos_;
	transform_.Update();
	collider_->UpdateTransform(transform_);
}

void ShotBase::UpdateEnd(void)
{
}

void ShotBase::IsHit_Enemy(void)
{
	HitEnemy();
}

void ShotBase::IsHit_Player(void)
{
	HitPlayer();
}

void ShotBase::InitIsHitUpdate(void)
{
	isHitUpdate_.emplace(OBJECT_TYPE::ENEMY, bind(&ShotBase::IsHit_Enemy, this));
	isHitUpdate_.emplace(OBJECT_TYPE::PLAYER, bind(&ShotBase::IsHit_Player, this));
}

void ShotBase::HitEnemy(void)
{
	//èàóùÇ»Çµ
}

void ShotBase::HitPlayer(void)
{
	//èàóùÇ»Çµ
}

const bool ShotBase::IsDestroy(void) const
{
	return false;
}
