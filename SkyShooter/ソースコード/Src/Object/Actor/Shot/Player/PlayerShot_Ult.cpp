
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
	//�G�t�F�N�g�Ǐ]�p
	movedPos_ = CalucuratePos();

	effectController_->Update(EFFECT_ULT_CHARGE);
	if (effectController_->IsEffectEnd(EFFECT_ULT_CHARGE))
	{
		//�`���[�W�G�t�F�N�g�I�����ɍU��
		effectController_->Stop(EFFECT_ULT_CHARGE);
		ChangeState(State::Attack);
		return;
	}

	//������̍X�V
	TransformUpdate_PosQua(LookRotateTransform());

	//�G�t�F�N�g�̍X�V
	effectController_->FollowTransformUpdate(EFFECT_ULT_CHARGE, transform_);
}

void PlayerShot_Ult::Update_Attack(void)
{
	//�G�t�F�N�g�Ǐ]�p
	movedPos_ = CalucuratePos();

	if (effectController_->IsEffectEnd(EFFECT_ULT_ATTACK))
	{
		//�G�t�F�N�g�I�����ɔ���
		ChangeState(State::Blast);
	}

	//������̍X�V
	TransformUpdate_PosQua(LookRotateTransform());

	//�G�t�F�N�g�̍X�V
	effectController_->FollowTransformUpdate(EFFECT_ULT_ATTACK, transform_);
}

void PlayerShot_Ult::Update_Blast(void)
{
	if (effectController_->IsEffectEnd(EFFECT_ULT_ATTACK))
	{
		//�I��
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

	//�U���G�t�F�N�g
	EffectPlayData effectPlayData(EFFECT_ULT_ATTACK_CNT);
	effectController_->Add(
		EFFECT_ULT_ATTACK,
		resMngEffect_.Load(SRC_EFFECT::ULT).GetHandleId(),
		effectTransform,
		effectPlayData
	);

	//�`���[�W�G�t�F�N�g
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
	//�U���O
	stateChange_.emplace(State::None, bind(&PlayerShot_Ult::Change_None, this));

	//�U����
	stateChange_.emplace(State::Attack, bind(&PlayerShot_Ult::Change_Attack, this));

	//����
	stateChange_.emplace(State::Blast, bind(&PlayerShot_Ult::Change_Blast, this));

	//�I��
	stateChange_.emplace(State::End, bind(&PlayerShot_Ult::Change_End, this));
}

void PlayerShot_Ult::Change_None(void)
{
	//�`���[�W�G�t�F�N�g
	effectController_->Draw(EFFECT_ULT_CHARGE);

	//�G�̕����Ɍ�����
	TransformUpdate_PosQua(LookRotateTransform());

	//�G�t�F�N�g�̍��W���X�V
	effectController_->FollowTransformUpdate(EFFECT_ULT_CHARGE, transform_);

	stateUpdate_ = bind(&PlayerShot_Ult::Update_None, this);
}

void PlayerShot_Ult::Change_Attack(void)
{
	//�������b�Z�[�W���M
	SendPlayerMessage(Common_Message::ULT_ATTACK);

	//�U���G�t�F�N�g
	effectController_->Draw(EFFECT_ULT_ATTACK);

	//�G�̕����Ɍ�����
	TransformUpdate_PosQua(LookRotateTransform());

	//�G�t�F�N�g�̍��W���X�V
	effectController_->FollowTransformUpdate(EFFECT_ULT_ATTACK, transform_);

	//�Փ˔���L����
	collider_->SetColAble(true);

	//�R���C�_�̍��W���X�V
	collider_->UpdateTransformQuaRot(transform_.quaRot);

	stateUpdate_ = bind(&PlayerShot_Ult::Update_Attack, this);
}

void PlayerShot_Ult::Change_Blast(void)
{
	//�Փ˔��薳����
	collider_->SetColAble(false);

	//�������b�Z�[�W���M
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

	//�v���C���[�̑O�����甭��
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
		//�v���C���[�����݂��Ă��Ȃ�
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
		//�v���C���[�����݂��Ă��Ȃ�
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
		//�v���C���[�����݂��Ă��Ȃ�
		return;
	}

	player.lock()->ReceiveCommonMessage(message);
}
