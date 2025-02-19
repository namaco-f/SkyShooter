
#include "../../../../Manager/ResourceManager/ResourceManager.h"
#include "../../../../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../../../../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../../../../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../../../../Manager/TimeManager.h"
#include "../../../../Manager/Camera.h"
#include "../../../../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../../../../Manager/ActorsManager/Shot/ShotManager.h"
#include "../../../../Manager/UIManager/UIDataManager.h"
#include "../UI/GetUIDataToVariantTemplate.h"

#include "../Object/Common/AnimationController.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Collider2D_Circle.h"

#include "../Effect/EffectController.h"

#include "../../../../Utility/MathUtility.h"
#include "../../Shot/Player/PlayerShotTrigger.h"
#include "../../Shot/Player/PlayerShot_Standard.h"
#include "../../Shot/Player/PlayerShot_Missile.h"
#include "../../Shot/Player/PlayerShot_Ult.h"
#include "../../Shot/ShotFactory.h"
#include "../../Shot/DamageData.h"
#include "../Sound/SoundController.h"
#include "../Observer/Message.h"
#include "../../../Common/CircleShadow.h"
#include "PlayerUnit01.h"


using namespace std;

using Math = MathUtility;

const int MAX_HP = 1200;		//最大hp
const int HP = MAX_HP;		//hp

const int MAX_MP = 100;
const int MP = MAX_MP / 2;

const float UC = 0.0f;
const float MAX_UC = 1.0f;

const float INIT_SPEED = 20.0f;				//初期の移動速度
const float INIT_DASH_SPEED_PAR = 2.0f;		//ダッシュ時の移動速度倍率

const float TEST_COLLIDER_RADIUS = 50.0f;	//当たり判定の半径
const float COLLIDER_RADIUS_2D = 100.0f;	//当たり判定の半径

const float TIME_SHOT_COOL = 1.0f;			//弾発射のクールタイム
const float TIME_DELAY_SHOT = 0.05f;		//弾発射時のディレイ
const int MAX_SHOT_NUM = 3;					//1バーストの弾数

const int M_MP_COST = 2;					//弾のコスト
const float M_TIME_SHOT_COOL = 0.5f;		//弾発射のクールタイム
const float M_TIME_DELAY_SHOT = 0.0f;		//弾発射時のディレイ
const int M_MAX_SHOT_NUM = 6;				//1バーストの弾数
const VECTOR M_LOCAL_POS = { 0.0f,100.0f,0.0f };

const float TIME_SHIELD = 0.5f;						//シールドの発動時間上限
const float TIME_SHIELD_COOL_NOTCOUNTER = 1.0f;		//シールドカウンター非成立時のクールタイム
const float TIME_SHIELD_COOL_COUNTER = 2.0f;		//シールドカウンター成立時のクールタイム

const float DEAD_HEALTIME = 5.0f;

const string ANIMATION_RUN = "Run";			
const string ANIMATION_IDLE = "Idle";				//静止状態
const string ANIMATION_FLYING = "Flying";			//移動
const string ANIMATION_ATTACK = "Attack";			//攻撃
const string ANIMATION_ULTATTACK = "UltAttack";		//攻撃
const string ANIMATION_ULTCHARGE = "UltCharge";		//攻撃

const float INV_FRAME_MAX = 30.0f;				//無敵フレーム
const float ULT_FRAME_MAX = 180.0f;				//無敵フレーム

const VECTOR COLISION_LOCAL_POS_TOP = { 0.0f,50.0f,0.0f };
const VECTOR COLISION_LOCAL_POS_BOTTOM = { 0.0f,-10.0f,0.0f };

const string SHIELD_EFFECT = "SHIELD";
const string ULT_CHARGE_EFFECT = "ULT_CHARGE";

const string SE_ULTCHARGE = "UltCharge";		//チャージ音
const string SE_ULTATTACK = "UltAttack";		//発射音
const string SE_POWERUP = "PowerUp";			//シールド吸収音

const VECTOR INIT_POS = { 0.0f,0.0f,100.0f };
const VECTOR INIT_QUAROT_RAD = { 0.0f,Math::Deg2RadF(0.0f),0.0f };
const VECTOR INIT_QUAROTLOCAL_RAD = { 0.0f,Math::Deg2RadF(180.0f),0.0f };

const int COUNTER_ATTACK_CHARGE_MAX = 20;

const float ANIMATION_SPEED_SHIELD_COUNTER = 60.0f;

const float ANIMATION_ENDTIME_ULTCHARGE = 41.0f;
const float ANIMATION_SPEED_ULTCHARGE = 20.0f;

const float ANIMATION_STARTTIME_ULTATTACK = 42.0f;
const float ANIMATION_ENDTIME_ULTATTACK = 48.0f;
const float ANIMATION_SPEED_ULTATTACK = 30.0f;

const float UC_HEAL_HIT_ENEMYATTACK = 0.05f;
const float UC_HEAL_HIT_ENEMYBOXATTACK = 0.025f;

const VECTOR ULTATTACK_LOCALPOS = { 0.0f,0.0f,-300.0f };
const float ULTATTACK_POS_OFFSET = 10.0f;

const int MP_HEAL_SHIELD_COUNTER = 10;

PlayerUnit01::PlayerUnit01(void)
	:
	PlayerBase(),
	standardShot_(ShotData(TIME_DELAY_SHOT, MAX_SHOT_NUM, TIME_SHOT_COOL)),
	missile_(ShotData(M_TIME_DELAY_SHOT, M_MAX_SHOT_NUM, M_TIME_SHOT_COOL, M_MP_COST)),
	counterShot_(ShotData(M_TIME_DELAY_SHOT, 0,0.5f)),
	shieldTimeCnt_(0.0f),
	shieldTimeLimit_(3.0f),
	shieldTimeLimitCnt_(0.0f),
	shieldTimeCool_(0.0f),
	counterCnt_(0)
{
	initSpeed_ = INIT_SPEED;
	dashPar_ = INIT_DASH_SPEED_PAR;

	InitParameter();

	InitActionUpdate();
	InitIsHitUpdate();
	AddStateChange();
	AddAttackStateChange();
	Init();

	InitCommonMessageUpdate();

	shotTrigger_->AddShotPattern(PlayerShotPattern::Standard, standardShot_);
	shotTrigger_->AddShotPattern(PlayerShotPattern::Missile, missile_);
	shotTrigger_->AddShotPattern(PlayerShotPattern::CounterShot, counterShot_);

	movedPos_ = transform_.pos;
}

PlayerUnit01::~PlayerUnit01(void)
{
	MV1DeleteModel(transform_.modelId_);
}

void PlayerUnit01::InitLoad(void)
{
	resMngModel_.Load(SRC_MODEL::BOT_X);
	resMngEffect_.Load(SRC_EFFECT::SHIELD);
	
	//音読込
	resMngSound_.Load(SRC_SOUND::SE_PLAYER_DASH);
	resMngSound_.Load(SRC_SOUND::SE_PLAYER_ULT_CHAGE);
	resMngSound_.Load(SRC_SOUND::SE_PLAYER_SHOT_SPECIAL);
	resMngSound_.Load(SRC_SOUND::SE_SHIELDCOUNTER_CHARGE);
}

void PlayerUnit01::InitTransform(void)
{
	auto& modelRes = resMng_.GetModelResourcesInstance();

	transform_.SetModel(modelRes.LoadModelDuplicate(SRC_MODEL::BOT_X));
	transform_.pos = { 0.0f,0.0f,-1000.0f };
	transform_.quaRot = Quaternion({ 0.0f,Math::Deg2RadF(0.0f),0.0f });
	transform_.quaRotLocal = Quaternion({ 0.0f,Math::Deg2RadF(180.0f),0.0f });

	transform_.Update();
	circleShadow_->Update(transform_);

	uiDataMng_.SetUIData(SRC_UIData::PLAYER_POS, transform_.pos);

	EffectTransform effectTrans;
	effectTrans.pos = transform_.pos;
	effectTrans.localPos = { 0.0f,transform_.GetPosFrame(2).y, 0.0f};
	effectTrans.scale = { 25.0f,25.0f,25.0f };

	effectController_->Add(
		SHIELD_EFFECT, 
		resMngEffect_.Load(SRC_EFFECT::SHIELD).GetHandleId(),
		effectTrans);

	effectTrans.localPos = {};
	effectTrans.scale = { 100.0f,100.0f,100.0f };

	effectController_->Add(
		ULT_CHARGE_EFFECT,
		resMngEffect_.Load(SRC_EFFECT::ULT_CHARGE).GetHandleId(),
		effectTrans);
}

void PlayerUnit01::InitCollider(void)
{
	using ColliderData = Collider::ColliderData;

	//本体
	ColliderData colData = ColliderData();
	colData.transform_ = transform_;
	colData.colType_ = COLLISION_TYPE::SPHERE;
	colData.radius_ = TEST_COLLIDER_RADIUS;
	colData.ableCol_ = true;

	collider_ = make_unique<Collider>(colData);
	collider_->UpdateCollider(colData);
	collider_->SetLocalPos(COLISION_LOCAL_POS_TOP);

	//足元
	ColliderData colFoot = ColliderData();
	colFoot.transform_ = transform_;
	colFoot.colType_ = COLLISION_TYPE::LINE;
	colFoot.localPosTop_ = COLISION_LOCAL_POS_TOP;
	colFoot.localPosBottom_ = COLISION_LOCAL_POS_BOTTOM;
	colFoot.ableCol_ = true;

	colliderFoot_ = make_unique<Collider>(colFoot);
	colliderFoot_->UpdateCollider(colFoot);

	//xz平面
	ColliderData_2D circleData2D;
	circleData2D.colCenterPos = { transform_.pos.x,transform_.pos.z };
	circleData2D.radius = COLLIDER_RADIUS_2D;

	collider2D_ = make_unique<Collider2D_Circle>(circleData2D,true);
}

void PlayerUnit01::InitParameter(void)
{
	hp_ = HP;
	hpMax_ = MAX_HP;

	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_HP, hp_);
	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_HPMAX, hpMax_);

	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_MP, MP);
	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_MPMAX, MAX_MP);

	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_UC, UC);
	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_UCMAX, MAX_UC);

	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_SHILD_LIMIT, TIME_SHIELD);

	uiDataMng_.SetUIData<float>(
		SRC_UIData::PLAYER_SHILD_COOLPAR, 1.0f);
}

void PlayerUnit01::InitRender(void)
{
}

void PlayerUnit01::InitEnd(void)
{
	InitSound();
	InitAnimation();
	ChangeState(State::Idle);
}

void PlayerUnit01::InitSound(void)
{
	//ウルトチャージ音
	SoundInfo soundInfo = SoundInfo();
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_PLAYER_ULT_CHAGE);
	soundInfo.playType = DX_PLAYTYPE_BACK;

	soundController_->Add(SE_ULTCHARGE, soundInfo);

	//ウルト発射音
	soundInfo = SoundInfo();
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_PLAYER_SHOT_SPECIAL);
	soundInfo.playType = DX_PLAYTYPE_BACK;

	soundController_->Add(SE_ULTATTACK, soundInfo);

	//シールド吸収音
	soundInfo = SoundInfo();
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_SHIELDCOUNTER_CHARGE);
	soundInfo.playType = DX_PLAYTYPE_BACK;

	soundController_->Add(SE_POWERUP, soundInfo);

	//弾発射音
	standardShot_.shotSoundId_ = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_PLAYER_SHOT);
	missile_.shotSoundId_ = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_PLAYER_SHOT);
	counterShot_.shotSoundId_ = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_PLAYER_SHOT_SPECIAL);
}

void PlayerUnit01::Destroy(void)
{
	ChangeState(State::Destroy);
}

void PlayerUnit01::InitAnimation(void)
{
	using AData = AnimData;
	using AOption = AnimPlayOption;
	using ATime = AnimTime;

	animationController_ = make_unique<AnimationController>(transform_.modelId_);
	
	animationController_->AddAnimationData(
		ANIMATION_IDLE, AData(
			resMng_.GetModelResourcesInstance().LoadModelDuplicate(SRC_MODEL::Idle),
			0,
			ATime(),
			AOption(true)
		)
	);
	
	animationController_->AddAnimationData(
		ANIMATION_FLYING, AData(
			resMng_.GetModelResourcesInstance().LoadModelDuplicate(SRC_MODEL::Flying),
			0,
			ATime(),
			AOption(true)
		)
	);

	animationController_->AddAnimationData(
		ANIMATION_ATTACK, AData(
			resMng_.GetModelResourcesInstance().LoadModelDuplicate(SRC_MODEL::Attack),
			0,
			ATime(ANIMATION_SPEED_SHIELD_COUNTER),
			AOption(false)
		)
	);	
	
	animationController_->AddAnimationData(
		ANIMATION_ULTCHARGE, AData(
			resMng_.GetModelResourcesInstance().LoadModelDuplicate(SRC_MODEL::UltAttack),
			0,
			ATime(0.0f, ANIMATION_ENDTIME_ULTCHARGE, ANIMATION_SPEED_ULTCHARGE),
			AOption(false)
		)
	);

	animationController_->AddAnimationData(
		ANIMATION_ULTATTACK, AData(
			resMng_.GetModelResourcesInstance().LoadModelDuplicate(SRC_MODEL::UltAttack),
			0,
			ATime(
				ANIMATION_STARTTIME_ULTATTACK, ANIMATION_ENDTIME_ULTATTACK, ANIMATION_SPEED_ULTATTACK
			),
			AOption(true,false,true)
		)
	);
}

void PlayerUnit01::IsHit_EnemyShot1(void)
{
	if (IsAbleShield() && !IsShieldCounter() && !IsInvisible())
	{
		//シールド中かつ無敵時間ではない
		CounterCount();
		return;
	}

	if (!IsInvisible())
	{
		//被弾
		Damage(ENEMY_SHOT_STANDARD_DAMAGE);

		invFrameCnt_ = deltaTime_ * INV_FRAME_MAX;

		uiDataMng_.SetUIData(SRC_UIData::PLAYER_HP, hp_);

		UCHeal(UC_HEAL_HIT_ENEMYATTACK);
	}
}

void PlayerUnit01::IsHit_EnemyMissile(void)
{
	if (IsAbleShield() && !IsShieldCounter() && !IsInvisible())
	{
		//シールド中かつ無敵時間ではない
		CounterCount();
		return;
	}

	if (!IsInvisible())
	{
		//被弾
		Damage(ENEMY_SHOT_MISSILE_DAMAGE);
		invFrameCnt_ = deltaTime_ * INV_FRAME_MAX;

		UCHeal(UC_HEAL_HIT_ENEMYATTACK);

		uiDataMng_.SetUIData(SRC_UIData::PLAYER_HP, hp_);
	}
}

void PlayerUnit01::IsHit_EnemyBox(void)
{
	const int SHIELD_ADAPT_SCALE = 4;
	const int DAMAGE_ADAPT = ENEMY_BOX_DAMAGE / SHIELD_ADAPT_SCALE;

	if (IsInvisible())
	{
		//無敵時間中
		return;
	}

	if (IsAbleShield())
	{
		//シールド中被弾
		Damage(DAMAGE_ADAPT);
	}
	else
	{
		//被弾
		Damage(ENEMY_BOX_DAMAGE);
	}

	invFrameCnt_ = deltaTime_ * INV_FRAME_MAX;

	uiDataMng_.SetUIData(SRC_UIData::PLAYER_HP, hp_);

	UCHeal(UC_HEAL_HIT_ENEMYBOXATTACK);
}

const bool PlayerUnit01::InvalidGravity(void) const
{
	return IsAbleShield() || IsUltAttackAble();
}

void PlayerUnit01::UpdateState(void)
{
	if (!AbleUseSheild())
	{
		shieldCoolTime_ -= deltaTime_;
		if (shieldCoolTime_ < 0.0f)
		{
			shieldCoolTime_ = 0.0f;
		}
		uiDataMng_.SetUIData<float>(
			SRC_UIData::PLAYER_SHILD_COOLPAR,
			(TIME_SHIELD_COOL_NOTCOUNTER - shieldCoolTime_) / TIME_SHIELD_COOL_NOTCOUNTER);
	}

	stateUpdate_();
}

void PlayerUnit01::UpdateCreateShot(void)
{
	int mp = GetUIDataToVariant<int>(SRC_UIData::PLAYER_MP);
	shotTrigger_->UpdateTransform(transform_);
	shotTrigger_->Update(mp);

	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_MP, mp);
}

void PlayerUnit01::Run(void)
{
	ChangeState(State::Move);
}

void PlayerUnit01::Dash(void)
{
	ChangeState(State::Dash);
}

void PlayerUnit01::Idle(void)
{
	if (IsUltAble())
	{
		return;
	}

	ChangeState(State::Idle);
}

void PlayerUnit01::Attack_Left(void)
{
	ChangeAttackState(AttackState::Shot1);
}

void PlayerUnit01::Attack_Right(void)
{
	ChangeAttackState(AttackState::Shot2);
}

void PlayerUnit01::Shield(void)
{
	if (!AbleUseSheild() || IsUltAble())
	{
		return;
	}
	shieldTimeCnt_ = TIME_SHIELD;
	ChangeState(State::Shield);
}

void PlayerUnit01::UltAttack(void)
{
	Vector2f ucData = GetUCData();

	if (ucData.x < ucData.y)
	{
		return;
	}

	ChangeState(State::UltCharge);
}

void PlayerUnit01::CreateUltAttack(void)
{
	Transform trans;
	trans.localPos = ULTATTACK_LOCALPOS;
	trans.pos = VScale(transform_.quaRot.GetForward(), ULTATTACK_POS_OFFSET);

	shotMng_.AddShot([&] {return Create<PlayerShot_Ult>(trans); });
}

void PlayerUnit01::Change_Idle(void)
{
	VECTOR preEuler = transform_.quaRot.ToEuler();
	transform_.quaRot =
		Quaternion::Euler({ 0.0f,preEuler.y,0.0f });

	transform_.quaRot = transform_.quaRot.Mult(camera_.GetQuaRotOutX());
	transform_.Update();

	animationController_->Play(ANIMATION_IDLE);
	stateUpdate_ = bind(&PlayerUnit01::Update_Idle, this);
}

void PlayerUnit01::Change_Move(void)
{
	animationController_->Play(ANIMATION_FLYING);
	stateUpdate_ = bind(&PlayerUnit01::Update_Move, this);
}

void PlayerUnit01::Change_Dash(void)
{
	stateUpdate_ = bind(&PlayerUnit01::Update_Dash, this);
}

void PlayerUnit01::Change_Shot1(void)
{
	shotTrigger_->CreateShot(PlayerShotPattern::Standard);
}

void PlayerUnit01::Change_Shot2(void)
{
	shotTrigger_->CreateShot(PlayerShotPattern::Missile);
}

void PlayerUnit01::Change_Destroy(void)
{
	deadHealTime_ = DEAD_HEALTIME;
	stateUpdate_ = bind(&PlayerUnit01::Update_Destroy, this);
	++deadCnt_;
}

void PlayerUnit01::Change_Shield(void)
{
	Quaternion qua = camera_.GetQuaRotOutX();
	PlayerRotate(qua);

	animationController_->Play(ANIMATION_ATTACK);
	animationController_->Stop(ANIMATION_ATTACK);

	effectController_->FollowTransformUpdate(SHIELD_EFFECT,transform_);
	effectController_->Draw(SHIELD_EFFECT);

	shieldTimeLimitCnt_ = 0.0f;
	stateUpdate_ = bind(&PlayerUnit01::Update_Shield, this);
}

void PlayerUnit01::Change_ShieldCounterAttack(void)
{
	shieldCoolTime_ = TIME_SHIELD_COOL_COUNTER;
	invFrameCnt_ = INV_FRAME_MAX * deltaTime_;

	if (counterCnt_ > 0)
	{
		ShotData shotData = shotTrigger_->GetShotData(PlayerShotPattern::CounterShot);

		shotData.shotNumMax_ = counterCnt_;

		shotTrigger_->UpdateShotPattern(PlayerShotPattern::CounterShot, shotData);

		shotTrigger_->CreateShot(PlayerShotPattern::CounterShot);

		MpHeal(MP_HEAL_SHIELD_COUNTER);

		animationController_->Play(ANIMATION_ATTACK);
	}

	counterCnt_ = 0;

	stateUpdate_ = bind(&PlayerUnit01::Update_ShieldCounterAttack, this);
}

void PlayerUnit01::Change_UltCharge(void)
{
	soundController_->Play(SE_ULTCHARGE);

	UCActive();

	weak_ptr<CharactorBase> enemy = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);

	const VECTOR& pos = enemy.lock()->GetTransform().pos;

	const VECTOR vec = VNorm(VSub(pos, transform_.pos));

	transform_.quaRot = transform_.quaRot.Quaternion::LookRotation(vec);
	transform_.Update();

	CreateUltAttack();

	MpHealMax();

	effectController_->FollowTransformUpdate(ULT_CHARGE_EFFECT, transform_);
	effectController_->Draw(ULT_CHARGE_EFFECT);

	animationController_->Play(ANIMATION_ULTCHARGE);
	stateUpdate_ = bind(&PlayerUnit01::Update_UltCharge, this);
}

void PlayerUnit01::Change_UltAttack(void)
{
	animationController_->Play(ANIMATION_ULTATTACK);
	soundController_->Play(SE_ULTATTACK);

	stateUpdate_ = bind(&PlayerUnit01::Update_UltAttack, this);
}

void PlayerUnit01::Change_End(void)
{
	stateUpdate_ = bind(&PlayerUnit01::Update_End, this);
}

void PlayerUnit01::Update_Idle(void)
{
}

void PlayerUnit01::Update_Move(void)
{
}

void PlayerUnit01::Update_Dash(void)
{
}

void PlayerUnit01::Update_Destroy(void)
{
	deadHealTime_ -= deltaTime_;
	const int hpMax = GetUIDataToVariant<int>(SRC_UIData::PLAYER_HPMAX);

	int hp = Math::Lerp(0, hpMax, (DEAD_HEALTIME - deadHealTime_) / DEAD_HEALTIME);

	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_HP, hp);

	if (deadHealTime_ <= 0.0f)
	{
		ChangeState(State::End);
	}
}

void PlayerUnit01::Update_Shield(void)
{
	shieldTimeCnt_ -= deltaTime_;
	shieldTimeLimitCnt_ += deltaTime_;
	
	effectController_->FollowTransformUpdate(SHIELD_EFFECT, transform_);

	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_SHILD_TIME, shieldTimeCnt_);


	if (shieldTimeCnt_ < 0.0f || shieldTimeLimitCnt_ > shieldTimeLimit_)
	{
		if (counterCnt_ > 0)
		{
			effectController_->Stop(SHIELD_EFFECT);
			ChangeState(State::ShieldCounterAttack);
			return;
		}

		shieldCoolTime_ = TIME_SHIELD_COOL_NOTCOUNTER;
		animationController_->Play(ANIMATION_ATTACK);
		effectController_->Stop(SHIELD_EFFECT);
		ChangeState(State::Idle);
	}
}

void PlayerUnit01::Update_ShieldCounterAttack(void)
{
	int mp = GetMpData().x;
	shotTrigger_->UpdateTransform(transform_);
	shotTrigger_->Update(mp);
	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_MP, mp);

	invFrameCnt_ -= deltaTime_;

	if (invFrameCnt_ < 0.0f && 
		!animationController_->IsPlayAnim(ANIMATION_ATTACK) &&
		shotTrigger_->AbleTriggerShot())
	{
		ShotData shotData = shotTrigger_->GetShotData(PlayerShotPattern::CounterShot);
		shotData.shotNumMax_ = counterCnt_;
		shotTrigger_->UpdateShotPattern(PlayerShotPattern::CounterShot, shotData);

		invFrameCnt_ = INV_FRAME_MAX * deltaTime_;
		ChangeState(State::Idle);
	}
}

void PlayerUnit01::Update_UltCharge(void)
{
	effectController_->FollowTransformUpdate(ULT_CHARGE_EFFECT, transform_);

	weak_ptr<CharactorBase> enemy = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);

	const VECTOR& pos = enemy.lock()->GetTransform().pos;

	const VECTOR vec = VNorm(VSub(pos, transform_.pos));

	transform_.quaRot = transform_.quaRot.Quaternion::LookRotation(vec);
}

void PlayerUnit01::Update_UltAttack(void)
{
	weak_ptr<CharactorBase> enemy = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);

	const VECTOR& pos = enemy.lock()->GetTransform().pos;

	const VECTOR vec = VNorm(VSub(pos, transform_.pos));

	transform_.quaRot = transform_.quaRot.Quaternion::LookRotation(vec);
}

void PlayerUnit01::Update_End(void)
{
	hp_ = MAX_HP;
	MpHealMax();
	UCHeal(MAX_UC);

	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion(INIT_QUAROT_RAD);
	transform_.quaRotLocal = Quaternion(INIT_QUAROTLOCAL_RAD);

	UpdateTransform();

	ChangeState(State::Idle);
}

void PlayerUnit01::Message_UltAttack(void)
{
	ChangeState(State::UltAttack);
}

void PlayerUnit01::Message_UltEnd(void)
{
	ChangeState(State::Idle);
}

void PlayerUnit01::Message_Dash(void)
{
	moveSpeed_ = initSpeed_ * dashPar_;
}

void PlayerUnit01::Message_DashEnd(void)
{
	moveSpeed_ = initSpeed_;
}

void PlayerUnit01::AddStateChange(void)
{
	using P01 = PlayerUnit01;

	stateChange_.emplace(State::Idle, bind(&P01::Change_Idle, this));
	stateChange_.emplace(State::Move, bind(&P01::Change_Move, this));
	stateChange_.emplace(State::Dash, bind(&P01::Change_Dash, this));
	stateChange_.emplace(State::Shield, bind(&P01::Change_Shield, this));
	stateChange_.emplace(State::ShieldCounterAttack, bind(&P01::Change_ShieldCounterAttack, this));
	stateChange_.emplace(State::UltCharge, bind(&P01::Change_UltCharge, this));
	stateChange_.emplace(State::UltAttack, bind(&P01::Change_UltAttack, this));
	stateChange_.emplace(State::Destroy, bind(&P01::Change_Destroy, this));
	stateChange_.emplace(State::End, bind(&P01::Change_End, this));
}

void PlayerUnit01::AddAttackStateChange(void)
{
	attackStateChange_.emplace(AttackState::Shot1, bind(&PlayerUnit01::Change_Shot1, this));
	attackStateChange_.emplace(AttackState::Shot2, bind(&PlayerUnit01::Change_Shot2, this));
}


const bool PlayerUnit01::IsErrorUpdate(void) const
{
	return IsStateError() || !stateUpdate_;
}

const bool PlayerUnit01::AbleUseSheild(void) const
{
	return shieldCoolTime_ <= 0.0f;
}

void PlayerUnit01::InitIsHitUpdate(void)
{
	isHitUpdate_.emplace(OBJECT_TYPE::ENEMY, bind(&PlayerUnit01::IsHit_Enemy, this));
	isHitUpdate_.emplace(OBJECT_TYPE::ENEMY_ATACK, bind(&PlayerUnit01::IsHit_EnemyShot1, this));
	isHitUpdate_.emplace(OBJECT_TYPE::ENEMY_MISSILE, bind(&PlayerUnit01::IsHit_EnemyMissile, this));
	isHitUpdate_.emplace(OBJECT_TYPE::ENEMY_ATACK_BOX, bind(&PlayerUnit01::IsHit_EnemyBox, this));
}

void PlayerUnit01::InitCommonMessageUpdate(void)
{
	commonMessageUpdate_.emplace(Common_Message::ULT_ATTACK,
		bind(&PlayerUnit01::Message_UltAttack, this)
	);

	commonMessageUpdate_.emplace(Common_Message::ULT_BLAST,
		bind(&PlayerUnit01::Message_UltEnd, this)
	);

	commonMessageUpdate_.emplace(Common_Message::DASH ,
		bind(&PlayerUnit01::Message_Dash, this)
	);

	commonMessageUpdate_.emplace(Common_Message::DASH_END,
		bind(&PlayerUnit01::Message_DashEnd, this)
	);
}

void PlayerUnit01::InitActionUpdate(void)
{
	actionUpdate_.emplace(ACTION::MOVE_FORWARD, bind(&PlayerUnit01::MoveDir_Forward, this));
	actionUpdate_.emplace(ACTION::MOVE_BACK, bind(&PlayerUnit01::MoveDir_Back, this));
	actionUpdate_.emplace(ACTION::MOVE_LEFT, bind(&PlayerUnit01::MoveDir_Left, this));
	actionUpdate_.emplace(ACTION::MOVE_RIGHT, bind(&PlayerUnit01::MoveDir_Right, this));
	actionUpdate_.emplace(ACTION::MOVE_UP, bind(&PlayerUnit01::MoveDir_Up, this));

	actionUpdate_.emplace(ACTION::PLAYER_RAN, bind(&PlayerUnit01::Run, this));
	actionUpdate_.emplace(ACTION::PLAYER_DASH, bind(&PlayerUnit01::Dash, this));
	actionUpdate_.emplace(ACTION::PLAYER_LOCK_SWITCH, bind(&PlayerUnit01::LockSwitch, this));
	actionUpdate_.emplace(ACTION::PLAYER_ATTACK, bind(&PlayerUnit01::Attack_Left, this));
	actionUpdate_.emplace(ACTION::PLAYER_ATTACK2, bind(&PlayerUnit01::Attack_Right, this));
	actionUpdate_.emplace(ACTION::PLAYER_IDLE, bind(&PlayerUnit01::Idle, this));
	actionUpdate_.emplace(ACTION::PLAYER_ULTATTACK, bind(&PlayerUnit01::UltAttack, this));

	actionUpdate_.emplace(ACTION::PLAYER_SHIELD, bind(&PlayerUnit01::Shield, this));
}

void PlayerUnit01::CounterCount(void)
{

	soundController_->Play(SE_POWERUP);

	if (++counterCnt_ > COUNTER_ATTACK_CHARGE_MAX)
	{
		counterCnt_ = COUNTER_ATTACK_CHARGE_MAX;
	}
}

void PlayerUnit01::InitUpdateObserver(void)
{	
	messageUpdate_.emplace(Message::IsGroundPlayer,
		bind(&PlayerUnit01::UpdateObserver_IsGroundPlayer, this));
	
	messageUpdate_.emplace(Message::IsNotGroundPlayer,
		bind(&PlayerUnit01::UpdateObserver_IsNotGroundPlayer, this));
}
