
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../Manager/TimeManager.h"
#include "../Effect/EffectController.h"
#include "../Object/Common/Collider/Collider.h"
#include "../../Charactor/Enemy/EnemyBase.h"
#include "../../Charactor/Player/PlayerBase.h"
#include "../Observer/Observer.h"
#include "Enemy_StandardShot.h"

using namespace std;

const float INIT_SHOT_SPEED = 100.0f;		//’e‘¬
const VECTOR INIT_SCALE = { 0.5f,0.5f,0.5f };
const float TEST_COLLIDER_RADIUS = 20.0f;
const float ALIVE_COUNT = 50.0f;

Enemy_StandardShot::Enemy_StandardShot(const Transform& trans)
	:
	StandardShot(trans)
{
	moveSpeed_ = INIT_SHOT_SPEED;
	objType_ = OBJECT_TYPE::ENEMY_ATACK;

	AddChangeState();

	auto& chara = CharactorManager::GetInstance();
	const weak_ptr<CharactorBase> enemy = chara.GetCharactor<PlayerBase>(OBJECT_TYPE::PLAYER);
	targetPos_ = enemy.lock()->GetTransform().pos;

	InitShotVec();

	Init();
}

Enemy_StandardShot::~Enemy_StandardShot(void)
{
}

void Enemy_StandardShot::Draw(void)
{
	if (IsDestroy())
	{
		return;
	}
	DrawSphere3D(transform_.pos, TEST_COLLIDER_RADIUS, 4, 0xff0000, 0xff0000, false);
}

void Enemy_StandardShot::InitShotStartPos(void)
{
	auto& chara = CharactorManager::GetInstance();
	const weak_ptr<CharactorBase> enemy = chara.GetCharactor<EnemyBase>(OBJECT_TYPE::ENEMY);

	transform_.pos = enemy.lock()->GetTransform().pos;
}

const bool Enemy_StandardShot::IsAlive(void) const
{
	return state_ != State::End;
}

void Enemy_StandardShot::InitLoad(void)
{
}

void Enemy_StandardShot::InitTransform(void)
{


	//transform_.SetModel(resMngModel_.LoadModelDuplicate(SRC_MODEL::SHOT));
	transform_.scl = INIT_SCALE;
	//transform_.quaRotLocal = Quaternion::Euler(Math::Deg2RadF(90.0f), 0.0f, 0.0f);

	transform_.Update();
}

void Enemy_StandardShot::InitCollider(void)
{
	using ColData = Collider::ColliderData;
	ColData colData = ColData();
	colData.transform_ = transform_;
	colData.colType_ = COLLISION_TYPE::SPHERE;
	colData.radius_ = TEST_COLLIDER_RADIUS;
	colData.ableCol_ = true;

	collider_ = make_unique<Collider>();
	collider_->UpdateCollider(colData);
}

void Enemy_StandardShot::InitEnd(void)
{
	InitIsHitUpdate();

	ChangeState(State::None);
}

void Enemy_StandardShot::HitPlayer(void)
{
	Destroy();
}

void Enemy_StandardShot::AddChangeState(void)
{
	stateChange_.emplace(State::None,bind(&Enemy_StandardShot::Change_None, this));
	stateChange_.emplace(State::Blast, bind(&Enemy_StandardShot::Change_Blast, this));
	stateChange_.emplace(State::End, bind(&Enemy_StandardShot::Change_End, this));
}

void Enemy_StandardShot::Change_None(void)
{
	stateUpdate_ = bind(&Enemy_StandardShot::Update_None, this);
	stateDraw_ = bind(&Enemy_StandardShot::Draw_None, this);
}

void Enemy_StandardShot::Change_Blast(void)
{
	collider_->SetColAble(false);
	stateUpdate_ = bind(&Enemy_StandardShot::Update_Blast, this);
	stateDraw_ = bind(&Enemy_StandardShot::Draw_Blast, this);
}

void Enemy_StandardShot::Change_End(void)
{
	stateUpdate_ = bind(&Enemy_StandardShot::Update_End, this);
	stateDraw_ = bind(&Enemy_StandardShot::Draw_End, this);
}
