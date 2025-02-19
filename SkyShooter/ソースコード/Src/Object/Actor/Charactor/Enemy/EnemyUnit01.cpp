
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Manager/ResourceManager/Shader/ShaderManager.h"
#include "../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../Manager/RandManager.h"
#include "../Manager/ActorsManager/Shot/ShotManager.h"
#include "../Manager/JSON/JSONManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/TimeManager.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Collider2D_Circle.h"
#include "../Object/Common/Collider/Collision.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Common/CircleShadow.h"
#include "../../Shot/Enemy/Enemy_StandardShot.h"
#include "../../Shot/Enemy/Enemy_Missile.h"
#include "../../Shot/DamageData.h"
#include "BoxAttack/EnemyAttack_BoxXY.h"
#include "BoxAttack/EnemyAttack_BoxXZ.h"
#include "BoxAttack/EnemyAttack_BoxZY.h"
#include "../../Shot/ShotFactory.h"
#include "../../../../Utility/MathUtility.h"
#include "../Renderer/Material/VertexsMaterial.h"
#include "../Renderer/Material/PixelMaterial.h"
#include "../Renderer/Renderer.h"
#include "BoxAttack/EnemyAttack_Boxes.h"
#include "../Effect/EffectController.h"
#include "../Sound/SoundController.h"
#include "../Observer/Observer.h"
#include "../../Stage/StageSize.h"
#include "EnemyUnit01.h"

using namespace std;

using Math = MathUtility;

const int MAX_HP = 30000000;		//�ő�hp
const int HP = MAX_HP;				//hp

//�������W
const VECTOR INIT_POS = { 0.0f,1000.0f,3000.0f };

//�����X�P�[��
const VECTOR INIT_SCALE = { 3.0f,2.5f,6.0f };

//������]
const VECTOR INIT_QUAROT_RAD = { 0.0f,Math::Deg2RadF(180.0f),0.0f };

//�������[�J����]
const VECTOR INIT_QUAROT_LOCAL_RAD = { 0.0f,Math::Deg2RadF(90.0f),0.0f };

//�R���C�_�̔��a
const float COLLIDER_RADIUS = 700.0f;		

//�R���C�_�̒���
const float COLLIDER_LENGTH = 1500.0f;		

//�R���C�_�̃��[�J����]
const VECTOR COLLIDER_QUAROT_LOCAL_RAD = { 0.0f,0.0f,Math::Deg2RadF(90.0f) };

//���ʏ�̃R���C�_�̔��a
const float COLLIDER_RADIUS_2D = 200.0f;	

//�~�T�C���e���˂̃N�[���^�C��
const float M_TIME_SHOT_COOL = 3.0f;		

//�~�T�C���e���ˎ��̃f�B���C
const float M_TIME_DELAY_SHOT = 0.0f;		

//�~�T�C��1�o�[�X�g�̒e��
const int M_MAX_SHOT_NUM = 8;

//�~�T�C�����ˎ��̃��[�J�����W
const VECTOR M_LOCAL_POS = { 0.0f,100.0f,0.0f };

const float WARP_TIME = 0.1f;
const float WARP_READY_TIME = 2.0f;

const float ATTACK_COOLTIME = 1.5f;
const float ATTACK_COOLTIME_DELAY = 0.5f;

const float BOXATTACK_DELAY_TIME = 5.0f;
const float BOXATTACK_TOTAL_TIME = 3.0f;

const string EFFECT_WARP = "WARP";
const VECTOR EFFECT_WARP_SCALE = { 150.0f,150.0f,200.0f };

const string EFFECT_SLASTER = "SLASTER";
const VECTOR EFFECT_SLASTER_SCALE = { 150.0f,150.0f,250.0f };
const VECTOR EFFECT_SLASTER_LOCALPOS = { 0.0f,0.0f,-1500.0f };
const VECTOR EFFECT_SLASTER_LOCALROT_RAD = { 0.0f, Math::Deg2RadF(180.0f), 0.0f };

const string JSONDATA_POSTABLE = "BoxAttackPosTable";		//���U���̍��WJSON�f�[�^
const string JSONDATA_XY_BOXATTACK = "XYData";		//XY���U���̍��W�e�[�u��
const string JSONDATA_XZ_BOXATTACK = "XZData";		//XZ���U���̍��W�e�[�u��
const string JSONDATA_ZY_BOXATTACK = "ZYData";		//ZY���U���̍��W�e�[�u��

const float INV_COUNT = 30.0f;

const VECTOR SHADOW_SIZE = { COLLIDER_RADIUS * 2.0f ,0.0f,COLLIDER_LENGTH * 2.0f };

const float SHADOW_OFFSET_Y = -950.0f;

const string SE_WARP = "Warp";
const string SE_ATTACKREADY = "AttackReady";

const int WARP_HITDAMAGE_SCALE = 2;

//���[�v�\�͈�
const VECTOR WARP_ABLEPOS_MIN = { -9000.0f,800.0f,-9000.0f };
const VECTOR WARP_ABLEPOS_MAX = { 9000.0f,1500.0f,9000.0f };

const float SHOTPOS_Y_OFFSET = -200.0f;

const VECTOR SHOTPOS_LOCAL = { 0.0f,0.0f,0.0f };

const Vector2f BOXATTACK_XY_SIZE = { STAGE_MOVELIMIT_Z * 2.0f, 300.0f };
const Vector2f BOXATTACK_XZ_SIZE = { 1000.0f, STAGE_MOVELIMIT_Y * 2.0f };
const Vector2f BOXATTACK_ZY_SIZE = { STAGE_MOVELIMIT_X * 2.0f, 500.0f };

const FLOAT4 INIT_FLOAT4 = { 1.0f,1.0f,1.0f,1.0f };

const int BOXATTACK_POSTABLE_NORMALMODE_IDX = 0;
const int BOXATTACK_POSTABLE_HARDMODE_IDX = 1;

const VECTOR INIT_SHOT_RAD = {
	0.0f,
	90.0f,
	0.0f
};

const string JSON_X = "x";
const string JSON_Y = "y";
const string JSON_Z = "z";

//�U���̃����_���e�[�u��
const int ATTACKRANDOM_TABLE = (int)(EnemyUnit01::AttackState::MAX) - 2;

//�U���̃����_���e�[�u�����珜�O���鐔(�������p�ƃN�[���^�C���p�����O)
const int ATTACKRANDOM_TABLE_DIFF = (int)(EnemyUnit01::AttackState::CoolTime) + 1;

EnemyUnit01::EnemyUnit01(void)
	:
	EnemyBase(),
	missile_(ShotData(M_TIME_DELAY_SHOT, M_MAX_SHOT_NUM))
{
	InitCollider();

	hp_ = HP;
	hpMax_ = MAX_HP;

	uiDataMng_.SetUIData(SRC_UIData::ENEMY_HP, hp_);
	uiDataMng_.SetUIData(SRC_UIData::ENEMY_HPMAX, hpMax_);

	objType_ = OBJECT_TYPE::ENEMY;

	AddMovePattern();
	AddStateChange();
	AddAttackStateChange();

	InitIsHitUpdate();

	Init();
}

EnemyUnit01::~EnemyUnit01(void)
{
	effectController_->Stop(EFFECT_SLASTER);
	effectController_->Stop(EFFECT_WARP);
}

void EnemyUnit01::Draw(void)
{
	auto& standard = resMngShader_.Load(SRC_SHADER::STANDARD_MODEL);
	const weak_ptr<PixelMaterial> ps = standard.GetPS();
	const weak_ptr<Renderer> renderer = standard.GetRenderer();

	if (movePattern_ != MOVE_PATTERN::Warp)
	{
		//���[�v���ł͂Ȃ�
		if (warpCnt_ > 0.0f)
		{
			//���[�v��
			warpCnt_ -= deltaTime_;

			float cola = lerp(0.0f, 1.0f, (WARP_TIME - warpCnt_) / WARP_TIME);

			ps.lock()->SetConstBuf(0, { 1.0f,1.0f,1.0f,cola });
		}
	}

	renderer.lock()->DrawRenderer([&] {MV1DrawModel(transform_.modelId_); });

	circleShadow_->Draw();
	
	if (effectController_->IsEffectEnd(EFFECT_SLASTER))
	{
		effectController_->Draw(EFFECT_SLASTER);
	}

	//�X���X�^�[�G�t�F�N�g�Ɋւ���X�V
	effectController_->FollowTransformUpdate(EFFECT_SLASTER, transform_);

	DrawAttackBox();
}

void EnemyUnit01::InitLoad(void)
{
	//�G�̃��f��
	resMngModel_.Load(SRC_MODEL::ENEMY);
	
	//JSON�f�[�^�Ǎ�
	auto& jsonMng = JSONManager::GetInstance();
	jsonMng.Load(SRC_JSON::BOX_ATTACK_POS_DATA);

	//��
	resMngSound_.Load(SRC_SOUND::SE_ENEMY_WARP);
	resMngSound_.Load(SRC_SOUND::SE_ENEMY_COOLTIME_END);

	//�G�t�F�N�g
	resMngEffect_.Load(SRC_EFFECT::WARP);
	resMngEffect_.Load(SRC_EFFECT::SLASTER);
}

void EnemyUnit01::InitTransform(void)
{
	transform_.SetModel(resMngModel_.Load(SRC_MODEL::ENEMY).GetHandleId());
	transform_.pos = INIT_POS;
	transform_.scl = INIT_SCALE;
	transform_.quaRot = Quaternion::Euler(INIT_QUAROT_RAD);
	transform_.quaRotLocal = Quaternion::Euler(INIT_QUAROT_LOCAL_RAD);
	
	//���f��������ɔ��f
	transform_.Update();

	preQuaRot = transform_.quaRot;

	uiDataMng_.SetUIData(SRC_UIData::ENEMY_POS, transform_.pos);
}

void EnemyUnit01::InitRender(void)
{
	auto& standard = resMngShader_.Load(SRC_SHADER::STANDARD_MODEL);
	const weak_ptr<VertexsMaterial>& vs = standard.GetVS();
	vs.lock()->AddConstBuf(INIT_FLOAT4);
	const VECTOR ligPos = GetLightPosition();
	const VECTOR ligDir = GetLightDirection();
	vs.lock()->AddConstBuf(FLOAT4(ligPos.x, ligPos.y, ligPos.z, 1.0f));
	vs.lock()->AddConstBuf(FLOAT4(ligDir.x, ligDir.y, ligDir.z, 1.0f));

	const weak_ptr<PixelMaterial>& ps = standard.GetPS();
	ps.lock()->SetTextureAddress(PixelMaterial::TEXADDRESS::CLAMP);
	ps.lock()->AddTextureBuf(transform_.modelId_);
	ps.lock()->AddConstBuf(INIT_FLOAT4);

	standard.MakeRenderer();
}

void EnemyUnit01::InitEnd(void)
{
	InitEffect();

	InitShadow();

	InitSound();

	ChangeMovePattern(MOVE_PATTERN::Start);
	ChangeAttackState(AttackState::None);
	ChangeState(State::None);
}

void EnemyUnit01::Destroy(void)
{
	ChangeState(State::Destroy);
}

void EnemyUnit01::HitPlayerShot1(void)
{
	if (movePattern_ == MOVE_PATTERN::Warp)
	{
		//���[�v���_���[�W2�{
		Damage(PLAYER_SHOT_STANDARD_DAMAGE * WARP_HITDAMAGE_SCALE);
	}

	Damage(PLAYER_SHOT_STANDARD_DAMAGE);

	uiDataMng_.SetUIData(SRC_UIData::ENEMY_HP, hp_);
}

void EnemyUnit01::HitPlayerShot2(void)
{
	if (movePattern_ == MOVE_PATTERN::Warp)
	{
		//���[�v���_���[�W2�{
		Damage(PLAYER_SHOT_MISSILE_DAMAGE * WARP_HITDAMAGE_SCALE);
	}

	Damage(PLAYER_SHOT_MISSILE_DAMAGE);

	uiDataMng_.SetUIData(SRC_UIData::ENEMY_HP, hp_);
}

void EnemyUnit01::HitPlayerShotUlt(void)
{
	if (invCnt_ > 0.0f)
	{
		return;
	}

	if (movePattern_ == MOVE_PATTERN::Warp)
	{
		//���[�v���_���[�W2�{
		Damage(PLAYER_SHOT_MISSILE_DAMAGE * WARP_HITDAMAGE_SCALE);
	}

	Damage(PLAYER_SHOT_MISSILE_DAMAGE);
	invCnt_ = INV_COUNT * deltaTime_;

	uiDataMng_.SetUIData(SRC_UIData::ENEMY_HP, hp_);
}

void EnemyUnit01::InitShadow(void)
{
	circleShadow_ = make_unique<CircleShadow>(transform_, SHADOW_SIZE, SHADOW_OFFSET_Y);

	circleShadow_->Update(transform_);
}

void EnemyUnit01::InitAnimation(void)
{
}

void EnemyUnit01::InitSound(void)
{
	//���[�v�o����
	SoundInfo soundInfo = SoundInfo();
	soundInfo.playType = DX_PLAYTYPE_BACK;
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_ENEMY_WARP);
	soundController_->Add(SE_WARP, soundInfo);

	//�N�[���^�C���I��
	soundInfo = SoundInfo();
	soundInfo.playType = DX_PLAYTYPE_BACK;
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_ENEMY_COOLTIME_END);
	soundController_->Add(SE_ATTACKREADY, soundInfo);
}

void EnemyUnit01::InitEffect(void)
{
	//���[�v�G�t�F�N�g
	EffectTransform effectTransform = EffectTransform();
	effectTransform.scale = EFFECT_WARP_SCALE;
	effectController_->Add(
		EFFECT_WARP,
		resMngEffect_.Load(SRC_EFFECT::WARP).GetHandleId(), effectTransform);

	//�X���X�^�[�G�t�F�N�g
	effectTransform.scale = EFFECT_SLASTER_SCALE;
	effectTransform.localPos = EFFECT_SLASTER_LOCALPOS;
	effectTransform.localQuaRot = Quaternion::Euler(EFFECT_SLASTER_LOCALROT_RAD);
	effectController_->Add(
		EFFECT_SLASTER,
		resMngEffect_.Load(SRC_EFFECT::SLASTER).GetHandleId(), effectTransform);
}

const bool EnemyUnit01::IsErrorUpdate(void) const
{
	return IsStateError() || !stateUpdate_;
}

void EnemyUnit01::DrawAttackBox(void)
{
	enemyAttackboxes_->Draw();
}

void EnemyUnit01::UpdateState(void)
{
	if (IsErrorUpdate())
	{
		//�G���[
		return;
	}

	stateUpdate_();
}

void EnemyUnit01::Change_None(void)
{
	stateUpdate_ = bind(&EnemyUnit01::Update_None, this);
}

void EnemyUnit01::Change_Destroy(void)
{
	stateUpdate_ = bind(&EnemyUnit01::Update_Destroy, this);
}

void EnemyUnit01::Change_End(void)
{
	stateUpdate_ = bind(&EnemyUnit01::Update_End, this);

	observer_.SendMessageList(Message::Clear);
}

void EnemyUnit01::Update_None(void)
{
}

void EnemyUnit01::Update_Destroy(void)
{
	ChangeState(State::End);
}

void EnemyUnit01::Update_End(void)
{
}

void EnemyUnit01::AddStateChange(void)
{
	stateChange_.reserve((size_t)State::MAX);

	stateChange_.emplace(State::None, bind(&EnemyUnit01::Change_None, this));
	stateChange_.emplace(State::Destroy, bind(&EnemyUnit01::Change_Destroy, this));
	stateChange_.emplace(State::End, bind(&EnemyUnit01::Change_End, this));
}

void EnemyUnit01::AttackStateChange_None(void)
{
	attackStateUpdate_ = bind(&EnemyUnit01::AttackStateUpdate_None, this);
}

void EnemyUnit01::AttackStateChange_CoolTime(void)
{
	attackCoolTime_ = ATTACK_COOLTIME;
	attackCoolTimeDelay_ = ATTACK_COOLTIME_DELAY;
	attackStateUpdate_ = bind(&EnemyUnit01::AttackStateUpdate_CoolTime, this);
}

void EnemyUnit01::AttackStateChange_Missile(void)
{
	attackStateUpdate_ = bind(&EnemyUnit01::AttackStateUpdate_Missile, this);
}

void EnemyUnit01::AttackStateChange_XYBox(void)
{
	const VECTOR CENTER_POS = GetAttackPosFromTable(JSONDATA_XY_BOXATTACK, BOXATTACK_POSTABLE_NORMALMODE_IDX);
	const VECTOR CENTER_POS2 = GetAttackPosFromTable(JSONDATA_XY_BOXATTACK,BOXATTACK_POSTABLE_HARDMODE_IDX);

	enemyAttackboxes_->CreateEnemyAttackBox<EnemyAttack_BoxXY>(
		CENTER_POS, BOXATTACK_XY_SIZE, BOXATTACK_DELAY_TIME,BOXATTACK_TOTAL_TIME
		);
	
	if (IsChangeHardMode())
	{
		enemyAttackboxes_->CreateEnemyAttackBox<EnemyAttack_BoxXY>(
			CENTER_POS2, BOXATTACK_XY_SIZE, BOXATTACK_DELAY_TIME, BOXATTACK_TOTAL_TIME
			);
	}

	attackStateUpdate_ = bind(&EnemyUnit01::AttackStateUpdate_Box, this);
}

void EnemyUnit01::AttackStateChange_XZBox(void)
{
	const VECTOR CENTER_POS = GetAttackPosFromTable(JSONDATA_XZ_BOXATTACK, BOXATTACK_POSTABLE_NORMALMODE_IDX);
	const VECTOR CENTER_POS2 = GetAttackPosFromTable(JSONDATA_XZ_BOXATTACK, BOXATTACK_POSTABLE_HARDMODE_IDX);

	enemyAttackboxes_->CreateEnemyAttackBox<EnemyAttack_BoxXZ>(
		CENTER_POS, BOXATTACK_XZ_SIZE, BOXATTACK_DELAY_TIME, BOXATTACK_TOTAL_TIME
		);

	if (IsChangeHardMode())
	{
		enemyAttackboxes_->CreateEnemyAttackBox<EnemyAttack_BoxXZ>(
			CENTER_POS2, BOXATTACK_XZ_SIZE, BOXATTACK_DELAY_TIME, BOXATTACK_TOTAL_TIME
			);
	}

	attackStateUpdate_ = bind(&EnemyUnit01::AttackStateUpdate_Box, this);
}

void EnemyUnit01::AttackStateChange_ZYBox(void)
{
	const VECTOR CENTER_POS = GetAttackPosFromTable(JSONDATA_ZY_BOXATTACK, BOXATTACK_POSTABLE_NORMALMODE_IDX);
	const VECTOR CENTER_POS2 = GetAttackPosFromTable(JSONDATA_ZY_BOXATTACK, BOXATTACK_POSTABLE_HARDMODE_IDX);

	enemyAttackboxes_->CreateEnemyAttackBox<EnemyAttack_BoxZY>(
		CENTER_POS, BOXATTACK_ZY_SIZE, BOXATTACK_DELAY_TIME, BOXATTACK_TOTAL_TIME
		);

	if (IsChangeHardMode())
	{
		enemyAttackboxes_->CreateEnemyAttackBox<EnemyAttack_BoxZY>(
			CENTER_POS2, BOXATTACK_ZY_SIZE, BOXATTACK_DELAY_TIME, BOXATTACK_TOTAL_TIME
			);
	}

	attackStateUpdate_ = bind(&EnemyUnit01::AttackStateUpdate_Box, this);
}

void EnemyUnit01::AttackStateUpdate_None(void)
{
	//�����Ȃ�
}

void EnemyUnit01::AttackStateUpdate_CoolTime(void)
{
	attackCoolTime_ -= deltaTime_;
	
	if (attackCoolTime_ > 0.0f)
	{
		//�N�[���^�C�����I�����Ă��Ȃ�
		return;
	}

	if (attackCoolTimeDelay_ >= ATTACK_COOLTIME_DELAY)
	{		
		//�N�[���^�C���I�������Œʒm
		soundController_->Play(SE_ATTACKREADY);
	}

	attackCoolTimeDelay_ -= deltaTime_;

	if (attackCoolTimeDelay_ > 0.0f)
	{
		//�U���J�n�f�B���C���I�����Ă��Ȃ�
		return;
	}

	//�����_���U���J�n
	auto& randMng = RandManager::GetInstance();

	int rand = randMng.GetRand_mt(0, 1000);
	const auto state = (AttackState)(rand % ATTACKRANDOM_TABLE + ATTACKRANDOM_TABLE_DIFF)
		;
	ChangeAttackState(state);
}

void EnemyUnit01::AttackStateUpdate_Missile(void)
{
	missile_.shotDelayStep_ += deltaTime_;
	if (missile_.IsEndShotDelay() && missile_.IsAbleShot())
	{
		CreateMissle();
	}
}

void EnemyUnit01::AttackStateUpdate_Box(void)
{
	enemyAttackboxes_->Update();

	if (enemyAttackboxes_->IsBoxesAttackEnd())
	{
		//���U�����S�ďI�����Ă���
		ChangeAttackState(AttackState::CoolTime);
	}
}

void EnemyUnit01::AddAttackStateChange(void)
{
	//
	attackStateChange_.emplace(
		AttackState::None, bind(&EnemyUnit01::AttackStateChange_None, this)
	);

	//�N�[���^�C����
	attackStateChange_.emplace(
		AttackState::CoolTime, bind(&EnemyUnit01::AttackStateChange_CoolTime, this)
	);

	//�~�T�C���e
	attackStateChange_.emplace(
		AttackState::Missile, bind(&EnemyUnit01::AttackStateChange_Missile, this)
	);
	
	//XZ���U��
	attackStateChange_.emplace(
		AttackState::XZBox, bind(&EnemyUnit01::AttackStateChange_XZBox, this)
	);

	//ZY���U��
	attackStateChange_.emplace(
		AttackState::ZYBox, bind(&EnemyUnit01::AttackStateChange_ZYBox, this)
	);

	//XY���U��
	attackStateChange_.emplace(
		AttackState::XYBox, bind(&EnemyUnit01::AttackStateChange_XYBox, this)
	);
}

void EnemyUnit01::AddMovePattern(void)
{
	movePatternChange_.emplace(
		MOVE_PATTERN::Start, bind(&EnemyUnit01::Change_Start, this));
	movePatternChange_.emplace(
		MOVE_PATTERN::Standard, bind(&EnemyUnit01::Change_Standard, this));
	movePatternChange_.emplace(
		MOVE_PATTERN::Octagon, bind(&EnemyUnit01::Change_Octagon, this));
	movePatternChange_.emplace(
		MOVE_PATTERN::PlayerDir, bind(&EnemyUnit01::Change_PlayerDir, this));

	movePatternChange_.emplace(
		MOVE_PATTERN::Warp , bind(&EnemyUnit01::Change_Warp, this));
}

void EnemyUnit01::Change_Warp(void)
{
	soundController_->Play(SE_WARP);

	warpPosReady_ = false;
	moveTimeCnt_ = WARP_READY_TIME;
	warpCnt_ = 0.0f;

	effectController_->Draw(EFFECT_WARP);

	effectController_->FollowTransformUpdate(EFFECT_WARP, transform_);

	patternMove_ = bind(&EnemyUnit01::Move_Warp, this);
}

void EnemyUnit01::Move_Warp(void)
{
	moveTimeCnt_ -= deltaTime_;

	auto& standard = resMngShader_.Load(SRC_SHADER::STANDARD_MODEL);
	
	weak_ptr<PixelMaterial> ps =  standard.GetPS();

	//���l��ω�(1.0f��0.0f)
	float cola = lerp(1.0f, 0.0f, (WARP_READY_TIME - moveTimeCnt_) / WARP_READY_TIME);

	//�s�N�Z���V�F�[�_�̒萔�o�b�t�@�ɔ��f
	ps.lock()->SetConstBuf(0, { 1.0f,1.0f,1.0f,cola });

	if (moveTimeCnt_ < 0.0f)
	{
		//�ړ��p�^�[���ω�

		if (!warpPosReady_)
		{
			//���[�v��̍��W���i�[����Ă��Ȃ�

			//���݈ʒu�̑Ίp����Ƀ��[�v
			warpPos_ = VScale(transform_.pos, -1.0f);
			warpPos_.y = transform_.pos.y;

			////���[�v�\�͈�
			warpPos_ = Math::ClampVECTOR(warpPos_, WARP_ABLEPOS_MIN, WARP_ABLEPOS_MAX);

			warpPosReady_ = !warpPosReady_;
		}

		//���[�v���ړ�����
		WarpLerp();

		if (warpCnt_ >= WARP_TIME)
		{
			//�������A�y�у��[�v�I��
			effectController_->Draw(EFFECT_WARP);	
			effectController_->FollowTransformUpdate(EFFECT_WARP, transform_);
			
			soundController_->Play(SE_WARP);

			ChangeMovePattern(MOVE_PATTERN::PlayerDir);
		}
	}
}

void EnemyUnit01::CreateMissle(void)
{
	const float shotDiffDeg = 360.0f / missile_.shotNumMax_;

	//�e�̉�]����W���i�[
	Transform trans;

	//�e�̉�]
	//���@�̉�]�𔽉f
	trans.quaRot = transform_.quaRot;

	//�e�̔��˕�����6������
	trans.quaRot = trans.quaRot.Mult(
		Quaternion::Euler(
			{
				0.0f,
				(float)missile_.shotNum_* Math::Deg2RadF(-shotDiffDeg),
				0.0f,
			}
		)
	);

	//�e�̔��ˈʒu�̉�]
	Quaternion qua = Quaternion::Euler(
		{
			0.0f,
			0.0f,
			(float)missile_.shotNum_* Math::Deg2RadF(shotDiffDeg),
		}
	);

	//���ˈʒu�̃��[�J�����W
	VECTOR localVec = qua.PosAxis(SHOTPOS_LOCAL);

	//���ˈʒu�ɉ�]�𔽉f
	VECTOR localPos = transform_.quaRot.PosAxis(localVec);

	trans.pos = VAdd(transform_.pos, localPos);
	trans.pos.y += SHOTPOS_Y_OFFSET;

	shotMng_.AddShot([&] {return Create<Enemy_Missile>(trans); });

	missile_.shotDelayStep_ = 0.0f;

	++missile_.shotNum_;

	if (!missile_.IsAbleShot())
	{
		missile_.Init();
		ChangeAttackState(AttackState::CoolTime);
	}
}

void EnemyUnit01::WarpLerp(void)
{
	warpCnt_ += deltaTime_;
	movedPos_ = Math::Lerp(transform_.pos, warpPos_, warpCnt_ / WARP_TIME);
}

VECTOR EnemyUnit01::GetAttackPosFromTable(const std::string& dataName, const int tableNo)
{
	//JSON�f�[�^�擾
	auto& jsonMng = JSONManager::GetInstance();
	const auto& json = jsonMng.Load(SRC_JSON::BOX_ATTACK_POS_DATA).GetJSONData();

	//���W�f�[�^
	const auto& table = json[JSONDATA_POSTABLE];

	//�Ώۂ̍��W�e�[�u��
	const auto& data = table[dataName];

	//VECTOR�ɕϊ�
	return Math::IntToVECTOR(
		data[tableNo][JSON_X], 
		data[tableNo][JSON_Y],
		data[tableNo][JSON_Z]
	);
}

void EnemyUnit01::InitIsHitUpdate(void)
{
	isHitUpdate_.emplace(OBJECT_TYPE::PLAYER_ATTACK, bind(&EnemyUnit01::IsHit_PlayerShot1, this));
	isHitUpdate_.emplace(OBJECT_TYPE::PLAYER_ATTACK2, bind(&EnemyUnit01::IsHit_PlayerShot2, this));
	isHitUpdate_.emplace(OBJECT_TYPE::PLAYER_ULTATTACK, bind(&EnemyUnit01::HitPlayerShotUlt, this));
}

void EnemyUnit01::InitCollider(void)
{
	//�{�̂̋��̔���
	using ColData = Collider::ColliderData;
	ColData colData = ColData();
	colData.transform_ = transform_;
	colData.colType_ = COLLISION_TYPE::CAPSULE;
	colData.radius_ = COLLIDER_RADIUS;
	colData.ableCol_ = true;
	colData.localQuaRot_ = Quaternion::Euler(COLLIDER_QUAROT_LOCAL_RAD);
	colData.localPosTop_ = {0.0f,COLLIDER_LENGTH,0.0f};
	colData.localPosBottom_ = {0.0f,-COLLIDER_LENGTH ,0.0f};

	collider_ = make_unique<Collider>(colData);
	collider_->UpdateCollider(colData);

	//XZ���ʏ�̉~�`����
	ColliderData_2D circleData2D;
	circleData2D.colCenterPos = { transform_.pos.x,transform_.pos.z };
	circleData2D.radius = COLLIDER_RADIUS_2D;
	
	collider2D_ = make_unique<Collider2D_Circle>(circleData2D,true);
	collider2D_->UpdateTransform(transform_);
}
