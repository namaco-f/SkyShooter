#include "../Effect/EffectController.h"
#include "../Observer/Observer.h"
#include "StandardShot.h"

using namespace std;

StandardShot::StandardShot(const Transform& trans)
	:
	ShotBase(trans),
	state_(State::MAX),
	stateChange_{}
{
}

void StandardShot::Draw(void)
{
}

void StandardShot::ChangeState(const State state)
{
	if (!stateChange_.contains(state))
	{
		return;
	}

	state_ = state;
	stateChange_.at(state)();
}
const bool StandardShot::IsAlive(void) const
{
	return aliveCnt_ > 0.0f;
}

void StandardShot::InitShotVec(void)
{
	moveDir_ = VNorm(VSub(targetPos_, transform_.pos));
}

void StandardShot::UpdateState(void)
{
	if (!stateUpdate_)
	{
		return;
	}
	stateUpdate_();
}

void StandardShot::Move(void)
{
	if (aliveCnt_ < 0.0f)
	{
		Destroy();
		return;
	}

	transform_.quaRot = Quaternion::LookRotation(VNorm(moveDir_));

	//VECTOR vec = VNorm(VSub(targetPos_, transform_.pos));

	VECTOR movePow = VScale(moveDir_, moveSpeed_);

	movedPos_ = VAdd(transform_.pos, movePow);
}

void StandardShot::Change_Blast(void)
{
	stateUpdate_ = bind(&StandardShot::Update_Blast, this);
	stateDraw_ = bind(&StandardShot::Draw_Blast, this);
	aliveCnt_ = 0.0f;
}

void StandardShot::Change_End(void)
{
	stateUpdate_ = bind(&StandardShot::Update_End, this);
	stateDraw_ = bind(&StandardShot::Draw_End, this);
}

void StandardShot::Update_None(void)
{
	Move();
}

void StandardShot::Update_Blast(void)
{
}

void StandardShot::Update_End(void)
{
}

void StandardShot::Draw_None(void)
{
}

void StandardShot::Draw_Blast(void)
{	
}

void StandardShot::Draw_End(void)
{
}

void StandardShot::Destroy(void)
{
	ChangeState(State::Blast);
}

const bool StandardShot::IsDestroy(void) const
{
	return state_ == State::Blast || state_ == State::End;
}
