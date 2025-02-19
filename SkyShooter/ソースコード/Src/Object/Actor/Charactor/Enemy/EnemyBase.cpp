

#include "../../../../Manager/TimeManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../../Manager/RandManager.h"
#include "../Manager/ActorsManager/Shot/ShotManager.h"
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../../../../Manager/UIManager/UIDataManager.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Collider2D.h"
#include "../Object/Common/Collider/Collision.h"
#include "../Object/Common/CircleShadow.h"
#include "../Utility/MathUtility.h"
#include "../Utility/DebugUtility.h"
#include "../Enemy/BoxAttack/EnemyAttack_Boxes.h"
#include "../Sound/SoundController.h"
#include "../Observer/Observer.h"
#include "EnemyBase.h"

using namespace std;

using ObjTag = OBJECT_TYPE;
using Math = MathUtility;

const int MIN_HP = 0;		//hp下限
const int INIT_HP = 100;		//hp初期値
const int INIT_MAX_HP = 100;		//最大hp初期値

const float START_MOVE_SPEED = 10.0f;
const float START_MOVE_TIME = 5.0f;

const float OCTAGON_MOVE_TURN_TIME = 3.0f;
const float OCTAGON_MOVE_TIME = 20.0f;
const float OCTAGON_MOVE_SPEED = 15.0f;
const VECTOR OCTAGON_MOVE_TURNROT_DEG = { 0.0f,45.0f,0.0f };

const float TURNTIME_PAR = 100.0f;

const float STANDARD_MOVE_TIME = 10.0f;
const float STANDARD_MOVE_SPEED = 30.0f;

const float PLAYERDIR_MOVE_TIME = 5.0f;
const float PLAYERDIR_MOVE_SPEED = 15.0f;

const float ATTACK_COOLTIME = 3.0f;

EnemyBase::EnemyBase(void)
	: 
	CharactorBase(),
	shotMng_(ShotManager::GetInstance()),
	state_(State::MAX),
	movePattern_(MOVE_PATTERN::Standard),
	attackState_(AttackState::MAX),
	circleShadow_(nullptr),
	moveTimeCnt_(START_MOVE_TIME),
	moveTurnCnt_(0.0f),
	attackCoolTime_(0.0f),
	deltaTime_(TimeManager::GetInstance().GetDeltaTime()),
	turnTime_(0.0f),
	preQuaRot{},
	invCnt_(0.0f)
{
	auto& rand = RandManager::GetInstance();
		
	moveSpeed_ = START_MOVE_SPEED;
	moveDir_ = transform_.quaRot.GetLeft();
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Update(void)
{
	if (soundController_ != nullptr)
	{
		soundController_->Update();
	}

	deltaTime_ = TimeManager::GetInstance().GetDeltaTime();

	if (invCnt_ > 0.0f)
	{
		invCnt_ -= deltaTime_;
	}

	if (IsErrorUpdate())
	{
		return;
	}

	UpdateBefore();

	UpdateAttack();

	UpdateState();

	UpdateRotate();

	UpdatePos();

	UpdateTransform();
}

void EnemyBase::ChangeState(const State state)
{
	if (IsStateError(state) || state_ == state) 
	{
		//指定した状態がエラー、または同じ状態
		return; 
	}

	state_ = state;

	if (!stateChange_.contains(state_))
	{
		//状態変化時の関数が登録されていない
		return;
	}

	stateChange_.at(state_)();
}

void EnemyBase::ChangeMovePattern(const MOVE_PATTERN movePattern)
{
	if (movePattern_ == movePattern)
	{
		return;
	}

	movePattern_ = movePattern;

	if (movePatternChange_.contains(movePattern_))
	{
		preQuaRot = transform_.quaRot;
		movePatternChange_.at(movePattern_)();
	}
}

void EnemyBase::ChangeAttackState(const AttackState attackState)
{
	if (IsAttackStateError(attackState) || attackState_ == attackState)
	{
		//指定した状態がエラー、または同じ状態
		return;
	}

	attackState_ = attackState;

	if (!attackStateChange_.contains(attackState_))
	{
		//状態変化時の関数が登録されていない
		return;
	}

	attackStateChange_.at(attackState_)();
}

void EnemyBase::UpdateBefore(void)
{
	movedPos_ = transform_.pos;
	prePos_ = movedPos_;
}

void EnemyBase::UpdateAttack(void)
{
	if (!attackStateUpdate_)
	{
		//attackStateUpdate_が実行不可能
		return;
	}
	attackStateUpdate_();
}

void EnemyBase::UpdateRotate(void)
{
}

void EnemyBase::UpdatePos(void)
{
	if (!patternMove_)
	{
		//patternMove_が使用不可
		return;
	}
	patternMove_();

	movedPos_ = VAdd(movedPos_, VScale(moveDir_, moveSpeed_));
}

void EnemyBase::UpdateTransform(void)
{
	transform_.pos = movedPos_;
	transform_.Update();

	if (collider_ != nullptr)
	{
		collider_->UpdateTransform(transform_);
	}

	if (collider2D_ != nullptr)
	{
		collider2D_->UpdateTransform(transform_);
	}

	if (circleShadow_ != nullptr)
	{
		circleShadow_->Update(transform_);
	}

	uiDataMng_.SetUIData(SRC_UIData::ENEMY_POS, transform_.pos);
}

void EnemyBase::DrawDebug(void)
{
	auto& debug = SceneManager::GetInstance().GetDebugUtility();
	debug.DebugStringFormat(0, "colPos = %.2f,%.2f,%.2f", 0xff0000, collider_->GetTrasformPos());
}

void EnemyBase::Destroy(void)
{
	ChangeState(State::Destroy);
}

void EnemyBase::IsHit_PlayerShot1(void)
{
	HitPlayerShot1();
}

void EnemyBase::IsHit_PlayerShot2(void)
{
	HitPlayerShot2();
}

void EnemyBase::Change_Start(void)
{
	moveTimeCnt_ = START_MOVE_TIME;
	moveSpeed_ = START_MOVE_SPEED;
	patternMove_ = bind(&EnemyBase::Move_Start, this);
}

void EnemyBase::Change_Standard(void)
{
	moveTimeCnt_ = START_MOVE_TIME;
	moveSpeed_ = STANDARD_MOVE_SPEED;
	patternMove_ = bind(&EnemyBase::Move_Standard, this);
}

void EnemyBase::Change_Circle(void)
{
	patternMove_ = bind(&EnemyBase::Move_Circle, this);
}

void EnemyBase::Change_Octagon(void)
{
	moveTimeCnt_ = OCTAGON_MOVE_TIME;
	moveTurnCnt_ = OCTAGON_MOVE_TURN_TIME;
	patternMove_ = bind(&EnemyBase::Move_Octagon, this);
}

void EnemyBase::Change_PlayerDir(void)
{
	moveTimeCnt_ = PLAYERDIR_MOVE_TIME;
	moveSpeed_ = PLAYERDIR_MOVE_SPEED;
	patternMove_ = bind(&EnemyBase::Move_PlayerDir, this);
}

void EnemyBase::Change_Warp(void)
{
	patternMove_ = bind(&EnemyBase::Move_Warp, this);
}

void EnemyBase::Move_Standard(void)
{
	moveTimeCnt_ -= deltaTime_;
	if (moveTimeCnt_ < 0.0f)
	{
		ChangeMovePattern(MOVE_PATTERN::Warp);
	}

	moveDir_ = transform_.GetForward();
}

void EnemyBase::Move_Circle(void)
{
}

void EnemyBase::Move_Octagon(void)
{
	moveTimeCnt_ -= deltaTime_;

	if (moveTimeCnt_ < 0.0f)
	{
		ChangeMovePattern(MOVE_PATTERN::Standard);
		return;
	}

	moveTurnCnt_ -= deltaTime_;

	if (moveTurnCnt_ <= 0.0f)
	{
		preQuaRot = transform_.quaRot.Mult(
			Quaternion::Euler(
				Math::Deg2RadF(OCTAGON_MOVE_TURNROT_DEG.x),
				Math::Deg2RadF(OCTAGON_MOVE_TURNROT_DEG.y), 
				Math::Deg2RadF(OCTAGON_MOVE_TURNROT_DEG.z)
			)
		);
		
		moveTurnCnt_ = OCTAGON_MOVE_TURN_TIME;

		turnTime_ = 0.0f;
	}

	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, preQuaRot, turnTime_ / TURNTIME_PAR);

	moveDir_ = transform_.GetForward();

	turnTime_ += deltaTime_;
}

void EnemyBase::Move_PlayerDir(void)
{
	moveTimeCnt_ -= deltaTime_;

	if (moveTimeCnt_ < 0.0f)
	{
		ChangeMovePattern(MOVE_PATTERN::Octagon);
		return;
	}

	auto& charaMng = CharactorManager::GetInstance();
	weak_ptr<CharactorBase> chara = charaMng.GetCharactor<CharactorBase>(OBJECT_TYPE::PLAYER);
	VECTOR playerPos = chara.lock()->GetTransform().pos;

	VECTOR vec = VSub(playerPos,transform_.pos);

	vec.y = 0.0f;

	transform_.quaRot = transform_.quaRot.LookRotation(vec);

	moveDir_ = transform_.GetForward();
}

void EnemyBase::Move_Start(void)
{
	moveTimeCnt_ -= deltaTime_;
	if (moveTimeCnt_ < 0.0f)
	{
		ChangeMovePattern(MOVE_PATTERN::Octagon);
		ChangeAttackState(AttackState::CoolTime);
		return;
	}

	auto& charaMng = CharactorManager::GetInstance();
	weak_ptr<CharactorBase> chara = charaMng.GetCharactor<CharactorBase>(OBJECT_TYPE::PLAYER);
	VECTOR playerPos = chara.lock()->GetTransform().pos;

	VECTOR vec = VSub(playerPos, transform_.pos);

	vec.y = 0.0f;

	transform_.quaRot = transform_.quaRot.LookRotation(vec);

	moveDir_ = transform_.GetForward();
}

void EnemyBase::Move_Warp(void)
{
}

const bool EnemyBase::IsStateError(void) const
{
	return state_ == State::MAX;
}

const bool EnemyBase::IsStateError(const State state) const
{
	return state == State::MAX;
}

const bool EnemyBase::IsAttackStateError(void)
{
	return attackState_ == AttackState::MAX;
}

const bool EnemyBase::IsAttackStateError(const AttackState attackState)
{
	return attackState == AttackState::MAX;
}

void EnemyBase::EnemyRotate(Quaternion qua)
{

}

const bool EnemyBase::IsChangeHardMode(void)
{
	return hp_ < (hpMax_ / 2);
}


