
#include "../../../Common/Collider/Collider.h"
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../../../../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../../../../Manager/TimeManager.h"
#include "../Effect/EffectController.h"
#include "../../../Actor/Charactor/CharactorBase.h"
#include "../Utility/MathUtility.h"
#include "../Observer/Observer.h"
#include "PlayerShot_Ult.h"

using Math = MathUtility;

using namespace std;

const float RADIUS = 500.0f;
const float SPEED = 100.0f;
const float LENGTH = 1500.0f;

const float DELAY_TIME = 2.0f;

const string EFFECT_ULT_ATTACK = "ULT";
const string EFFECT_ULT_CHARGE = "ULT_CHARGE";

const int EFFECT_ULT_CHARGE_START_CNT = 0;
const int EFFECT_ULT_CHARGE_END_CNT = 1;
const int EFFECT_ULT_ATTACK_CNT = 152;

const VECTOR EFFECT_LOCALPOS = { 0.0f,50.0f,500.0f };
const VECTOR EFFECT_SCALE = { RADIUS,RADIUS,100.0f };

const VECTOR COLLIDER_LOCALPOS_TOP = { 0.0f,0.0f,0.0f };
const VECTOR COLLIDER_LOCALPOS_BOTTOM = { 0.0f,0.0f,-5000.0f };

const VECTOR EFFECT_ULT_ROT_RAD = { 0.0f,0.0f,0.0f };
const VECTOR EFFECT_ULT_LOCALROT_RAD = { 0.0f,0.0f,0.0f };

PlayerShot_Ult::PlayerShot_Ult(const Transform& trans)
	: 
	UltBase(trans),
	step_(DELAY_TIME)
{
	objType_ = OBJECT_TYPE::PLAYER_ULTATTACK;

	AddStateChange();
	Init();
}

void PlayerShot_Ult::Draw(void)
{
	//DrawDebugModel();
}

void PlayerShot_Ult::InitShotStartPos(void)
{
}

void PlayerShot_Ult::Update_None(void)
{
	//エフェクト追従用
	movedPos_ = CalucuratePos();

	effectController_->Update(EFFECT_ULT_CHARGE);
	if (effectController_->IsEffectEnd(EFFECT_ULT_CHARGE))
	{
		//チャージエフェクト終了時に攻撃
		effectController_->Stop(EFFECT_ULT_CHARGE);
		ChangeState(State::Attack);
		return;
	}

	//制御情報の更新
	TransformUpdate_PosQua(LookRotateTransform());

	//エフェクトの更新
	effectController_->FollowTransformUpdate(EFFECT_ULT_CHARGE, transform_);
}

void PlayerShot_Ult::Update_Attack(void)
{
	//エフェクト追従用
	movedPos_ = CalucuratePos();

	if (effectController_->IsEffectEnd(EFFECT_ULT_ATTACK))
	{
		//エフェクト終了時に爆発
		ChangeState(State::Blast);
	}

	//制御情報の更新
	TransformUpdate_PosQua(LookRotateTransform());

	//エフェクトの更新
	effectController_->FollowTransformUpdate(EFFECT_ULT_ATTACK, transform_);
}

void PlayerShot_Ult::Update_Blast(void)
{
	if (effectController_->IsEffectEnd(EFFECT_ULT_ATTACK))
	{
		//終了
		collider_->SetColAble(false);
		ChangeState(State::End);
	}
}

void PlayerShot_Ult::Update_End(void)
{
}

void PlayerShot_Ult::InitLoad(void)
{
}

void PlayerShot_Ult::InitTransform(void)
{
	InitEffect();
}

void PlayerShot_Ult::InitCollider(void)
{
	Transform trans;

	Collider::ColliderData col;

	col.radius_ = RADIUS;
	col.colType_ = COLLISION_TYPE::CAPSULE;

	col.transform_.pos = CalucuratePos();
	col.localPosTop_ = COLLIDER_LOCALPOS_TOP;
	col.localPosBottom_ = COLLIDER_LOCALPOS_BOTTOM;
	col.ableCol_ = false;

	collider_ = make_unique<Collider>(col);	
}

void PlayerShot_Ult::InitEnd(void)
{
	InitIsHitUpdate();
	ChangeState(State::None);
}

void PlayerShot_Ult::InitEffect(void)
{
	EffectTransform effectTransform;

	effectTransform.pos = transform_.pos;
	effectTransform.localPos = EFFECT_LOCALPOS;
	effectTransform.scale = EFFECT_SCALE;
	effectTransform.quaRot = Quaternion::Euler(EFFECT_ULT_ROT_RAD);
	effectTransform.localQuaRot = Quaternion::Euler(EFFECT_ULT_LOCALROT_RAD);

	//攻撃エフェクト
	EffectPlayData effectPlayData(EFFECT_ULT_ATTACK_CNT);
	effectController_->Add(
		EFFECT_ULT_ATTACK,
		resMngEffect_.Load(SRC_EFFECT::ULT).GetHandleId(),
		effectTransform,
		effectPlayData
	);

	//チャージエフェクト
	effectPlayData = EffectPlayData(EFFECT_ULT_CHARGE_START_CNT, EFFECT_ULT_CHARGE_END_CNT, true);
	effectController_->Add(
		EFFECT_ULT_CHARGE,
		resMngEffect_.Load(SRC_EFFECT::ULT).GetHandleId(),
		effectTransform,
		effectPlayData
	);
}

void PlayerShot_Ult::HitEnemy(void)
{
}

void PlayerShot_Ult::DrawDebugModel(void)
{
	if (!IsAlive())
	{
		return;
	}
	collider_->Draw();
}

void PlayerShot_Ult::AddStateChange(void)
{
	//攻撃前
	stateChange_.emplace(State::None, bind(&PlayerShot_Ult::Change_None, this));

	//攻撃中
	stateChange_.emplace(State::Attack, bind(&PlayerShot_Ult::Change_Attack, this));

	//爆発
	stateChange_.emplace(State::Blast, bind(&PlayerShot_Ult::Change_Blast, this));

	//終了
	stateChange_.emplace(State::End, bind(&PlayerShot_Ult::Change_End, this));
}

void PlayerShot_Ult::Change_None(void)
{
	//チャージエフェクト
	effectController_->Draw(EFFECT_ULT_CHARGE);

	//敵の方向に向ける
	TransformUpdate_PosQua(LookRotateTransform());

	//エフェクトの座標情報更新
	effectController_->FollowTransformUpdate(EFFECT_ULT_CHARGE, transform_);

	stateUpdate_ = bind(&PlayerShot_Ult::Update_None, this);
}

void PlayerShot_Ult::Change_Attack(void)
{
	//爆発メッセージ送信
	SendPlayerMessage(Common_Message::ULT_ATTACK);

	//攻撃エフェクト
	effectController_->Draw(EFFECT_ULT_ATTACK);

	//敵の方向に向ける
	TransformUpdate_PosQua(LookRotateTransform());

	//エフェクトの座標情報更新
	effectController_->FollowTransformUpdate(EFFECT_ULT_ATTACK, transform_);

	//衝突判定有効化
	collider_->SetColAble(true);

	//コライダの座標情報更新
	collider_->UpdateTransformQuaRot(transform_.quaRot);

	stateUpdate_ = bind(&PlayerShot_Ult::Update_Attack, this);
}

void PlayerShot_Ult::Change_Blast(void)
{
	//衝突判定無効化
	collider_->SetColAble(false);

	//爆発メッセージ送信
	SendPlayerMessage(Common_Message::ULT_BLAST);

	stateUpdate_ = bind(&PlayerShot_Ult::Update_Blast, this);
}

void PlayerShot_Ult::Change_End(void)
{
	stateUpdate_ = bind(&PlayerShot_Ult::Update_End, this);
}

const VECTOR PlayerShot_Ult::CalucuratePos(void)
{
	Transform trans;

	//プレイヤーの前方から発射
	VECTOR pos = VScale(GetPlayerTransform().GetForward(), LENGTH);
	trans.pos = VAdd(GetPlayerTransform().pos, pos);

	return trans.pos;
}

const Transform& PlayerShot_Ult::GetPlayerTransform(void)
{
	auto& ins = CharactorManager::GetInstance();

	weak_ptr<CharactorBase> player = ins.GetCharactor<CharactorBase>(OBJECT_TYPE::PLAYER);

	if (player.expired())
	{
		//プレイヤーが存在していない
		return nullTransform_;
	}

	return player.lock()->GetTransform();
}

const Transform& PlayerShot_Ult::GetEnemyTransform(void)
{
	auto& ins = CharactorManager::GetInstance();

	weak_ptr<CharactorBase> enemy = ins.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);

	if (enemy.expired())
	{
		//プレイヤーが存在していない
		return nullTransform_;
	}

	return enemy.lock()->GetTransform();
}

const Transform PlayerShot_Ult::LookRotateTransform(void)
{
	Transform trans;

	VECTOR t = VSub(GetPlayerTransform().pos, GetEnemyTransform().pos);
	trans.pos = movedPos_;
	trans.quaRot = Quaternion::LookRotation(VNorm(t));

	return trans;
}

void PlayerShot_Ult::TransformUpdate_PosQua(const Transform& trans)
{
	transform_.pos = trans.pos;
	transform_.quaRot = trans.quaRot;
	transform_.Update();
}

void PlayerShot_Ult::SendPlayerMessage(const Common_Message message)
{
	auto& ins = CharactorManager::GetInstance();

	weak_ptr<CharactorBase> player = ins.GetCharactor<CharactorBase>(OBJECT_TYPE::PLAYER);

	if (player.expired())
	{
		//プレイヤーが存在していない
		return;
	}

	player.lock()->ReceiveCommonMessage(message);
}
