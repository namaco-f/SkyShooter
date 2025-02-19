
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "../Effect/EffectController.h"
#include "../Object/Common/Collider/Collider.h"
#include "../../Charactor/Enemy/EnemyBase.h"
#include "../../Charactor/Player/PlayerBase.h"
#include "../Trail.h"
#include "../../../../Utility/MathUtility.h"
#include "../Observer/Observer.h"
#include "PlayerShot_Missile.h"

using namespace std;

using Math = MathUtility;

const float INIT_SHOT_SPEED = 50.0f;		//’e‘¬
const float TEST_COLLIDER_RADIUS = 20.0f;

const VECTOR INIT_SCALE = { 0.5f,0.5f,0.5f };
const VECTOR INIT_QUAROT_RAD = { Math::Deg2RadF(30.0f),0.0f,0.0f };
const VECTOR INIT_QUAROTLOCAL_RAD = { Math::Deg2RadF(90.0f),Math::Deg2RadF(180.0f),0.0f };

const float TIME_REDIRECTION = 20.0f;

const VECTOR TRAIL_COLOR = { 0.0f,0.5f,1.0f };

const float MP_HEAL_AMOUNT = 0.025f;

const string EFFECT_HIT = "Hit";
const VECTOR EFFECT_HIT_INIT_SCALE = { 10.0f,10.0f,10.0f };

PlayerShot_Missile::PlayerShot_Missile(const Transform& trans)
	:
	Missile(trans)
{
	moveSpeed_ = INIT_SHOT_SPEED;

	objType_ = OBJECT_TYPE::PLAYER_ATTACK2;

	AddStateChange();

	auto& chara = CharactorManager::GetInstance();

	const auto& charas = chara.GetCharactors();

	redirectionStep_ = TIME_REDIRECTION * deltaTime_;

	Init();
}

PlayerShot_Missile::~PlayerShot_Missile(void)
{
}

void PlayerShot_Missile::InitLoad(void)
{
	resMngModel_.Load(SRC_MODEL::SHOT);
}

void PlayerShot_Missile::InitTransform(void)
{	
	transform_.SetModel(resMngModel_.LoadModelDuplicate(SRC_MODEL::SHOT));
	transform_.scl = INIT_SCALE;
	transform_.quaRot = transform_.quaRot.Mult(Quaternion::Euler(INIT_QUAROT_RAD));
	transform_.quaRotLocal = Quaternion::Euler(INIT_QUAROTLOCAL_RAD);

	transform_.Update();
}

void PlayerShot_Missile::InitCollider(void)
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

void PlayerShot_Missile::InitRender(void)
{
}

void PlayerShot_Missile::InitEnd(void)
{
	EffectTransform effectTrans;

	effectTrans.scale = EFFECT_HIT_INIT_SCALE;

	effectController_->Add(
		EFFECT_HIT, resMngEffect_.Load(SRC_EFFECT::HIT).GetHandleId(),
		effectTrans);

	InitIsHitUpdate();

	trail_ = make_unique<Trail>(transform_, TRAIL_COLOR);

	ChangeState(State::None);
}

void PlayerShot_Missile::Change_None(void)
{
	stateUpdate_ = bind(&PlayerShot_Missile::Update_None, this);
}

void PlayerShot_Missile::Change_Blast(void)
{
	stateUpdate_ = bind(&PlayerShot_Missile::Update_Blast, this);
	aliveCnt_ = 0.0f;
	collider_->SetColAble(false);

	effectController_->Draw(EFFECT_HIT);
}

void PlayerShot_Missile::Change_End(void)
{
	stateUpdate_ = bind(&PlayerShot_Missile::Update_End, this);
}

void PlayerShot_Missile::Update_None(void)
{
	redirectionStep_ -= deltaTime_;
	if (redirectionStep_ < 0.0f)
	{
		//Ä—U“±
		auto& chara = CharactorManager::GetInstance();
		const weak_ptr<CharactorBase> enemy = chara.GetCharactor<EnemyBase>(OBJECT_TYPE::ENEMY);
		if (enemy.expired())
		{
			return;
		}

		targetPos_ = enemy.lock()->GetTransform().pos;

		VECTOR vec = VNorm(VSub(targetPos_, transform_.pos));

		transform_.quaRot = Quaternion::LookRotation(vec);

		VECTOR movePow = VScale(vec, moveSpeed_);

		movedPos_ = VAdd(transform_.pos, movePow);

		redirectionStep_ = TIME_REDIRECTION * deltaTime_;
	}
	else
	{
		movedPos_ = VAdd(movedPos_, VScale(transform_.quaRot.GetForward(), moveSpeed_));
	}
}

void PlayerShot_Missile::Update_Blast(void)
{
	effectController_->FollowTransformUpdate(EFFECT_HIT, transform_);
	if (effectController_->IsEffectEnd(EFFECT_HIT))
	{
		effectController_->Stop(EFFECT_HIT);
		ChangeState(State::End);
	}
}

void PlayerShot_Missile::Update_End(void)
{
}

void PlayerShot_Missile::HitEnemy(void)
{
	auto& uiData = UIDataManager::GetInstance();
	float uc = GetUIDataToVariant<float>(SRC_UIData::PLAYER_UC);
	float ucMax = GetUIDataToVariant<float>(SRC_UIData::PLAYER_UCMAX);

	if (uc < ucMax)
	{
		uc += MP_HEAL_AMOUNT;
		if (uc >= ucMax)
		{
			uc = ucMax;
		}
	}

	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_UC, uc);

	Destroy();
}

void PlayerShot_Missile::AddStateChange(void)
{
	stateChange_.emplace(State::None, bind(&PlayerShot_Missile::Change_None, this));
	stateChange_.emplace(State::Blast, bind(&PlayerShot_Missile::Change_Blast, this));
	stateChange_.emplace(State::End, bind(&PlayerShot_Missile::Change_End, this));
}
