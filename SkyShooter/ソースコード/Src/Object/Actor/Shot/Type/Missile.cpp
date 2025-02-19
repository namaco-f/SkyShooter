
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../Manager/TimeManager.h"
#include "../Effect/EffectController.h"
#include "../Object/Common/Collider/Collider.h"
#include "../../Charactor/Enemy/EnemyBase.h"
#include "../../Charactor/Player/PlayerBase.h"
#include "../Trail.h"
#include "../../../../Utility/MathUtility.h"
#include "../Observer/Observer.h"
#include "Missile.h"

using namespace std;

using Math = MathUtility;

const float INIT_SHOT_SPEED = 10.0f;		//’e‘¬
const float TEST_COLLIDER_RADIUS = 20.0f;

const float TIME_REDIRECTION = 0.5f;


Missile::Missile(const Transform& trans)
	:
	ShotBase(trans),
	state_(State::MAX),
	redirectionStep_(TIME_REDIRECTION)
{
}

Missile::~Missile(void)
{
}

void Missile::Draw(void)
{
	if (IsDestroy())
	{
		return;
	}
	MV1DrawModel(transform_.modelId_);

	trail_->Draw();
}

void Missile::ChangeState(const State state)
{
	if (state_ == state) { return; }

	state_ = state;
	stateChange_[state_]();
}

const bool Missile::IsAlive(void) const
{
	return aliveCnt_ > 0.0f;
}

void Missile::AddStateChange(void)
{

}

void Missile::InitShotStartPos(void)
{
}

void Missile::UpdateState(void)
{
	stateUpdate_();
}

void Missile::UpdateEnd(void)
{
	trail_->Update(transform_);
}

void Missile::Move(void)
{
	aliveCnt_ -= deltaTime_;

	if (!IsAlive())
	{
		Destroy();
	}

	VECTOR vec = VNorm(VSub(targetPos_, transform_.pos));

	VECTOR movePow = VScale(vec, moveSpeed_);

	movedPos_ = VAdd(transform_.pos, movePow);
}

void Missile::Destroy(void)
{
	ChangeState(State::Blast);
}

void Missile::DrawDebugModel(void)
{
	const auto& dir = transform_.GetForward();

	DrawLine3D(transform_.pos, VAdd(transform_.pos, VScale(dir, 200.0f)), 0xff0000);
}

const bool Missile::IsDestroy(void) const
{
	return state_ == State::Blast || state_ == State::End;
}

