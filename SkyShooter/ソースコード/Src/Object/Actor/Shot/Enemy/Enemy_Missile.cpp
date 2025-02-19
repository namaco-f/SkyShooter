
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
#include "Enemy_Missile.h"

using namespace std;

using Math = MathUtility;

const float INIT_SHOT_SPEED = 40.0f;		//’e‘¬
const float TEST_COLLIDER_RADIUS = 20.0f;	//‹…‘ÌƒRƒ‰ƒCƒ_

const float TIME_REDIRECTION = 0.5f;

const VECTOR INIT_SCALE = { 0.5f,0.5f,0.5f };
const VECTOR INIT_DEGLOCAL = { Math::Deg2RadF(90.0f),Math::Deg2RadF(180.0f),0.0f };
const float LOCALPOS_LENGTH = 500.0f;

const VECTOR TRAIL_COLOR = { 1.0f,0.5f,0.0f };

Enemy_Missile::Enemy_Missile(const Transform& trans)
	:
	Missile(trans)
{
	moveSpeed_ = INIT_SHOT_SPEED;

	objType_ = OBJECT_TYPE::ENEMY_MISSILE;

	AddStateChange();

	Init();
}

Enemy_Missile::~Enemy_Missile(void)
{
}

void Enemy_Missile::InitLoad(void)
{
	resMngModel_.Load(SRC_MODEL::SHOT);
}

void Enemy_Missile::InitTransform(void)
{
	transform_.SetModel(resMngModel_.LoadModelDuplicate(SRC_MODEL::SHOT));
	transform_.scl = INIT_SCALE;
	transform_.quaRotLocal = Quaternion::Euler(INIT_DEGLOCAL);
	transform_.pos = VAdd(
		transform_.pos, VScale(transform_.GetForward(), LOCALPOS_LENGTH));

	transform_.Update();
}

void Enemy_Missile::InitCollider(void)
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

void Enemy_Missile::InitRender(void)
{
}

void Enemy_Missile::InitEnd(void)
{
	InitIsHitUpdate();

	trail_ = make_unique<Trail>(transform_,TRAIL_COLOR);

	ChangeState(State::None);
}

void Enemy_Missile::Change_None(void)
{
	stateUpdate_ = bind(&Enemy_Missile::Update_None, this);
}

void Enemy_Missile::Change_Blast(void)
{
	stateUpdate_ = bind(&Enemy_Missile::Update_Blast, this);
	aliveCnt_ = 0.0f;
}

void Enemy_Missile::Change_End(void)
{
	stateUpdate_ = bind(&Enemy_Missile::Update_End, this);
}

void Enemy_Missile::Update_None(void)
{
	redirectionStep_ -= deltaTime_;
	if (redirectionStep_ < 0.0f)
	{
		//Ä—U“±
		auto& chara = CharactorManager::GetInstance();
		const weak_ptr<CharactorBase> player = chara.GetCharactor<PlayerBase>(OBJECT_TYPE::PLAYER);
		if (player.expired())
		{
			//player‚ª‘¶Ý‚µ‚È‚¢
			return;
		}
		
		targetPos_ = player.lock()->GetTransform().pos;

		VECTOR vec = VNorm(VSub(targetPos_, transform_.pos));

		transform_.quaRot = Quaternion::LookRotation(vec);

		VECTOR movePow = VScale(vec, moveSpeed_);

		movedPos_ = VAdd(transform_.pos, movePow);

		redirectionStep_ = TIME_REDIRECTION;
	}
	else
	{
		movedPos_ = VAdd(movedPos_, VScale(transform_.quaRot.GetForward(), moveSpeed_));
	}
}

void Enemy_Missile::Update_Blast(void)
{
}

void Enemy_Missile::Update_End(void)
{
}

void Enemy_Missile::HitPlayer(void)
{
	collider_->SetColAble(false);
	Destroy();
}

void Enemy_Missile::AddStateChange(void)
{
	stateChange_.emplace(State::None, bind(&Enemy_Missile::Change_None, this));
	stateChange_.emplace(State::Blast, bind(&Enemy_Missile::Change_Blast, this));
	stateChange_.emplace(State::End, bind(&Enemy_Missile::Change_End, this));
}
