#include "../Effect/EffectController.h"
#include "../Observer/Observer.h"
#include "UltBase.h"

using namespace std;

UltBase::UltBase(const Transform& transform)
	: 
	ShotBase(transform),
	state_(State::None)
{
}

void UltBase::Draw(void)
{
}

void UltBase::ChangeState(const State state)
{
	state_ = state;
	stateChange_[state_]();
}

void UltBase::InitShotVec(void)
{
	moveDir_ = VNorm(VSub(targetPos_, transform_.pos));
}

void UltBase::UpdateState(void)
{
	if (!stateUpdate_)
	{
		return;
	}
	stateUpdate_();
}

void UltBase::Move(void)
{
}

void UltBase::Destroy(void)
{
	ChangeState(State::Blast);
}

const bool UltBase::IsDestroy(void) const
{
	return state_ == State::Blast || state_ == State::End;
}
