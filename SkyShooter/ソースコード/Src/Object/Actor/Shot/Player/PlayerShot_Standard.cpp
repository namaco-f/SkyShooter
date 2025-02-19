
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "../Effect/EffectController.h"
#include "../Object/Common/Collider/Collider.h"
#include "../../Charactor/Enemy/EnemyBase.h"
#include "../../Charactor/Player/PlayerBase.h"
#include "../../../../Utility/MathUtility.h"
#include "../Observer/Observer.h"
#include "PlayerShot_Standard.h"

using namespace std;

using Math = MathUtility;

const float INIT_SHOT_SPEED = 100.0f;		//íeë¨
const VECTOR INIT_SCALE = { 0.8f,0.8f,0.8f };
const VECTOR INIT_QUAROT_RAD = { Math::Deg2RadF(90.0f), Math::Deg2RadF(180.0f),0.0f };

const float TEST_COLLIDER_RADIUS = 20.0f;	//îºåa

const string EFFECT_HIT = "Hit";
const VECTOR EFFECT_HIT_INIT_SCALE = { 10.0f,10.0f,10.0f };

const float UCHEAL_AMOUNT = 0.01f;
const float ALIVE_TIME = 2.0f;

PlayerShot_Standard::PlayerShot_Standard(const Transform& trans)
	:
	StandardShot(trans)
{
	aliveCnt_ = ALIVE_TIME;
	moveSpeed_ = INIT_SHOT_SPEED;
	objType_ = OBJECT_TYPE::PLAYER_ATTACK;

	InitShotStartPos();
	AddChangeState();

	//ìGÇÃç¿ïWÇéÊìæÇµÅAñ⁄ïWÇ∆ÇµÇƒê›íË
	auto& chara = CharactorManager::GetInstance();
	const weak_ptr<CharactorBase> enemy = chara.GetCharactor<EnemyBase>(OBJECT_TYPE::ENEMY);
	targetPos_ = enemy.lock()->GetTransform().pos;

	InitShotVec();

	Init();
}

PlayerShot_Standard::~PlayerShot_Standard(void)
{
}

void PlayerShot_Standard::Draw(void)
{
	if (!stateDraw_)
	{
		return;
	}
	stateDraw_();

	if (IsDestroy())
	{
		//íeÇ™éÄñSèÛë‘
		return;
	}
	MV1DrawModel(transform_.modelId_);
	//DrawDebugModel();
}

void PlayerShot_Standard::InitShotStartPos(void)
{
}

void PlayerShot_Standard::InitLoad(void)
{
	resMngModel_.Load(SRC_MODEL::SHOT);
}

void PlayerShot_Standard::InitTransform(void)
{
	transform_.SetModel(resMngModel_.LoadModelDuplicate(SRC_MODEL::SHOT));
	transform_.scl = INIT_SCALE;
	transform_.quaRotLocal = Quaternion::Euler(INIT_QUAROT_RAD);
	
	transform_.Update();
}

void PlayerShot_Standard::InitCollider(void)
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

void PlayerShot_Standard::InitEnd(void)
{
	EffectTransform effectTrans;

	effectTrans.scale = EFFECT_HIT_INIT_SCALE;

	effectController_->Add(
		EFFECT_HIT, resMngEffect_.Load(SRC_EFFECT::HIT).GetHandleId(),
		effectTrans);

	InitIsHitUpdate();

	ChangeState(State::None);
}

void PlayerShot_Standard::HitEnemy(void)
{
	Destroy();
}

void PlayerShot_Standard::DrawDebugModel(void)
{
	const auto& dir = transform_.GetForward();

	DrawLine3D(transform_.pos, VAdd(transform_.pos,VScale(dir, 200.0f)),0xff0000);
}

void PlayerShot_Standard::AddChangeState(void)
{
	stateChange_.emplace(State::None, bind(&PlayerShot_Standard::Change_None, this));
	stateChange_.emplace(State::Blast, bind(&PlayerShot_Standard::Change_Blast, this));
	stateChange_.emplace(State::End, bind(&PlayerShot_Standard::Change_End, this));
}

void PlayerShot_Standard::Change_None(void)
{
	stateUpdate_ = bind(&PlayerShot_Standard::Update_None, this);
	stateDraw_ = bind(&PlayerShot_Standard::Draw_None, this);
}

void PlayerShot_Standard::Change_Blast(void)
{
	auto& uiData = UIDataManager::GetInstance();
	int mp = GetUIDataToVariant<int>(SRC_UIData::PLAYER_MP);
	int mpMax = GetUIDataToVariant<int>(SRC_UIData::PLAYER_MPMAX);
	
	if (++mp > mpMax)
	{
		mp = mpMax;
	}

	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_MP, mp);

	float uc = GetUIDataToVariant<float>(SRC_UIData::PLAYER_UC);
	float ucMax = GetUIDataToVariant<float>(SRC_UIData::PLAYER_UCMAX);

	if (uc < ucMax)
	{
		uc += UCHEAL_AMOUNT;
		if (uc >= ucMax)
		{
			uc = ucMax;
		}
	}

	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_UC, uc);

	stateUpdate_ = bind(&PlayerShot_Standard::Update_Blast, this);
	stateDraw_ = bind(&PlayerShot_Standard::Draw_Blast, this);
	collider_->SetColAble(false);

	effectController_->Draw(EFFECT_HIT);
}

void PlayerShot_Standard::Change_End(void)
{
	stateUpdate_ = bind(&PlayerShot_Standard::Update_End, this);
	stateDraw_ = bind(&PlayerShot_Standard::Draw_End, this);
}

void PlayerShot_Standard::Update_Blast(void)
{
	effectController_->FollowTransformUpdate(EFFECT_HIT, transform_);
	if (effectController_->IsEffectEnd(EFFECT_HIT))
	{
		effectController_->Stop(EFFECT_HIT);
		ChangeState(State::End);
	}
}

void PlayerShot_Standard::Draw_Blast(void)
{
}
