

#include "../../../../Manager/Scenemanager.h"
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../../../../Manager/TimeManager.h"
#include "../../../../Manager/Input/InputController.h"
#include "../../../../Manager/Camera.h"
#include "../Manager/ActorsManager/Shot/ShotManager.h"
#include "../../../../Manager/UIManager/UIDataManager.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "../Input/Player/PlayerInputComponent.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Collider2D.h"
#include "../Object/Common/Collider/Collider2D/Collision2D.h"
#include "../Object/Common/Collider/Collision.h"
#include "../Object/Common/AnimationController.h"
#include "../../Shot/Player/PlayerShotTrigger.h"
#include "../Enemy/EnemyBase.h"
#include "../../../../Utility/MathUtility.h"
#include "../Enemy/BoxAttack/EnemyAttack_Boxes.h"
#include "../../../Common/CircleShadow.h"
#include "../Sound/SoundController.h"
#include "../Observer/Observer.h"
#include "PlayerBase.h"

using namespace std;

using Math = MathUtility;
using ObjTag = OBJECT_TYPE;

const int MIN_HP = 0;		//hp下限
const int INIT_HP = 100;		//hp初期値
const int INIT_MAX_HP = 100;		//最大hp初期値
const VECTOR SHADOW_SIZE = { 128.0f,0.0f,128.0f };
const float OFFSET_POLY_POS_Y = 5.0f;

const float INIT_DASH_SPEED_PAR = 2.0f;		//ダッシュ時の移動速度倍率

const float INIT_MP_HEAL_TIME = 1.0f;
const float INIT_UC_HEAL_TIME = 1.0f;

const int MP_TIME_HEAL_AMOUNT = 1;
const float UC_TIME_HEAL_AMOUNT = 0.001f;

const VECTOR DIRROT_MOVE_FORWARD_RAD = { 0.0f,0.0f,0.0f };
const VECTOR DIRROT_MOVE_BACK_RAD = { 0.0f,Math::Deg2RadF(-180.0f),0.0f };
const VECTOR DIRROT_MOVE_LEFT_RAD = { 0.0f,Math::Deg2RadF(-90.0f),0.0f };
const VECTOR DIRROT_MOVE_RIGHT_RAD = { 0.0f,Math::Deg2RadF(90.0f),0.0f };
const float DIRROT_MOVE_UP = Math::Deg2RadF(-30.0f);

PlayerBase::PlayerBase(void)
	:
	CharactorBase(),
	charaMng_(CharactorManager::GetInstance()),
	shotMng_(ShotManager::GetInstance()),
	camera_(SceneManager::GetInstance().GetCamera()),
	state_(State::None),
	attackState_(AttackState::None),
	col2d_(make_unique<Collision2D>()),
	playerInput_(make_unique<PlayerInputComponent>()),
	shotTrigger_(make_unique<PlayerShotTrigger>(transform_)),
	circleShadow_(make_unique<CircleShadow>(transform_, SHADOW_SIZE, OFFSET_POLY_POS_Y)),
	deltaTime_(TimeManager::GetInstance().GetDeltaTime()),
	shieldCoolTime_(0.0f),
	dashPar_(1.0f),
	invFrameCnt_(0.0f),
	deadHealTime_(0.0f),
	deadCnt_(0),
	preQuaRot_{},
	mpHealTime_(INIT_MP_HEAL_TIME),
	ucHealTime_(INIT_UC_HEAL_TIME),
	stateChange_{},
	stateUpdate_{}
{
	objType_ = ObjTag::PLAYER;
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::Update(void)
{
	deltaTime_ = TimeManager::GetInstance().GetDeltaTime();

	if (soundController_ != nullptr)
	{
		soundController_->Update();
	}

	UpdateObserver();

	if (IsErrorUpdate()) 
	{
		//更新開始前エラー
		return;
	}

	UpdateBefore();

	UpdateState();

	UpdateCreateShot();

	UpdateRotate();

	UpdateMove();

	UpdateGravity();

	UpdateTransform();

	if (animationController_ != nullptr)
	{
		animationController_->Update();
	}
}

void PlayerBase::Draw(void)
{
	MV1DrawModel(transform_.modelId_);

	DrawShadow();
}

void PlayerBase::Heal(const int amount)
{
	hp_ += amount;

	if (hp_ > hpMax_)
	{
		hp_ = hpMax_;
	}
}

void PlayerBase::Step(void)
{
}

void PlayerBase::NoticeAction(const ACTION act)
{
	if (actionUpdate_.contains(act))
	{
		actionUpdate_.at(act)();
	}
}

void PlayerBase::ChangeState(const State state)
{
	if (IsStateError(state)) { return; }

	if (state_ == state) { return; }

	state_ = state;

	if (!stateChange_.contains(state_))
	{
		return;
	}
	stateChange_.at(state_)();
}

void PlayerBase::ChangeAttackState(const AttackState state)
{
	if (IsAttackStateError(state)) { return; }

	attackState_ = state;

	if (!attackStateChange_.contains(attackState_))
	{
		return;
	}
	attackStateChange_.at(attackState_)();
}

void PlayerBase::InitCollider(void)
{
}

void PlayerBase::InitParameter(void)
{
}

void PlayerBase::UpdateBefore(void)
{
	//プレイヤーの向き
	actorDir_ = Math::VECTOR_ZERO;

	prePos_ = movedPos_;

	moveSpeed_ = initSpeed_;

	UpdateMP();

	if (IsInvisible())
	{
		invFrameCnt_ -= deltaTime_;
	}

	if (state_ == State::Destroy || state_ == State::End)
	{
		return;
	}

	//プレイヤーの入力検知
	playerInput_->Update(*this);
}

void PlayerBase::UpdateMP(void)
{
	mpHealTime_ += deltaTime_;
	if (mpHealTime_ > INIT_MP_HEAL_TIME)
	{
		MpHeal(MP_TIME_HEAL_AMOUNT);
		mpHealTime_ = 0.0f;
	}

	ucHealTime_ += deltaTime_;
	if (ucHealTime_ > INIT_UC_HEAL_TIME)
	{
		UCHeal(UC_TIME_HEAL_AMOUNT);
		ucHealTime_ = 0.0f;
	}
}

void PlayerBase::UpdateRotate(void)
{
}

void PlayerBase::UpdateMove(void)
{
	moveDir_ = Math::VECTOR_ZERO;

	//X軸回転以外のカメラの角度を取得
	Quaternion cameraRot = camera_.GetQuaRotOutX();
	
	if (!camera_.IsLockOnCamera())
	{
		if (VSize(actorDir_) > 0.0f)
		{
			actorDir_ = VNorm(actorDir_);
		}

		//カメラの向きによって移動方向を変化
		moveDir_ = cameraRot.PosAxis(actorDir_);
	}
	else
	{
		moveDir_ = actorDir_;
	}

	//重力
	movedPos_ = VAdd(movedPos_, gravityPow_);

	//移動を加算
	movedPos_ = VAdd(movedPos_,VScale(moveDir_, moveSpeed_));

	collision_->IsCollisionMoveLimit(movedPos_);
}

void PlayerBase::UpdateTransform(void)
{
	transform_.pos = movedPos_;
	transform_.Update();
	circleShadow_->Update(transform_);

	collider_->UpdateTransform(transform_);
	collider2D_->UpdateTransform(transform_);
	colliderFoot_->UpdateTransform(transform_);

	//UI座標情報更新
	auto& uiData = UIDataManager::GetInstance();
	uiData.SetUIData(SRC_UIData::PLAYER_POS, transform_.pos);
}

void PlayerBase::DrawShadow(void)
{
	if (!isGround_)
	{
		//接地していない時は描画しない
		return;
	}

	circleShadow_->Draw();
}

void PlayerBase::MoveDir_Forward(void)
{
	if (IsUltAttackAble()) { return; }

	preQuaRot_ = transform_.quaRot;
	
	Quaternion qua = Quaternion::Euler(DIRROT_MOVE_FORWARD_RAD);
	Quaternion cameraQua = camera_.GetQuaRotOutX();
	PlayerRotate(cameraQua.Mult(qua));

	if (camera_.IsLockOnCamera())
	{
		actorDir_ = transform_.GetForward();
	}
	else
	{
		actorDir_ = VAdd(actorDir_, Math::DIR_F);
	}

	if (IsUltChargeAble())
	{
		return;
	}
	ChangeState(State::Move);
}

void PlayerBase::MoveDir_Back(void)
{
	if (IsUltAttackAble()) 
	{ 
		return; 
	}

	preQuaRot_ = transform_.quaRot;

	Quaternion qua = Quaternion::Euler(DIRROT_MOVE_BACK_RAD);
	Quaternion cameraQua = camera_.GetQuaRotOutX();
	PlayerRotate(cameraQua.Mult(qua));

	if (camera_.IsLockOnCamera())
	{
		actorDir_ = transform_.GetForward();
	}
	else
	{
		actorDir_ = VAdd(actorDir_, Math::DIR_B);
	}

	if (IsUltChargeAble())
	{
		return;
	}

	ChangeState(State::Move);
}

void PlayerBase::MoveDir_Left(void)
{
	if (IsUltAttackAble()) { return; }

	preQuaRot_ = transform_.quaRot;

	Quaternion qua = Quaternion::Euler(DIRROT_MOVE_LEFT_RAD);
	Quaternion cameraQua = camera_.GetQuaRotOutX();
	PlayerRotate(cameraQua.Mult(qua));

	if (camera_.IsLockOnCamera())
	{
		actorDir_ = transform_.GetForward();
	}
	else
	{
		actorDir_ = VAdd(actorDir_, Math::DIR_L);
	}

	if (IsUltChargeAble())
	{
		return;
	}
	ChangeState(State::Move);
}

void PlayerBase::MoveDir_Right(void)
{
	if (IsUltAttackAble()) { return; }

	preQuaRot_ = transform_.quaRot;

	Quaternion qua = Quaternion::Euler(DIRROT_MOVE_RIGHT_RAD);
	Quaternion cameraQua = camera_.GetQuaRotOutX();
	PlayerRotate(cameraQua.Mult(qua));

	if (camera_.IsLockOnCamera())
	{
		actorDir_ = transform_.GetForward();
	}
	else
	{
		actorDir_ = VAdd(actorDir_, Math::DIR_R);
	}

	if (IsUltChargeAble())
	{
		return;
	}
	ChangeState(State::Move);
}

void PlayerBase::MoveDir_Up(void)
{
	if (IsUltAttackAble()) { return; }

	VECTOR preEuler = transform_.quaRot.ToEuler();
	Quaternion qua = Quaternion::Euler({ DIRROT_MOVE_UP,preEuler.y,preEuler.z });

	PlayerRotate(qua);

	actorDir_ = VAdd(actorDir_, Math::DIR_U);

	movedPos_ = VAdd(movedPos_,VScale(actorDir_, initSpeed_));


	if (IsUltChargeAble())
	{
		return;
	}
	ChangeState(State::Move);
}

void PlayerBase::Run(void)
{
}

void PlayerBase::Dash(void)
{
}

void PlayerBase::Attack_Left(void)
{
}

void PlayerBase::Attack_Right(void)
{
}

void PlayerBase::UltAttack(void)
{
}

void PlayerBase::Shield(void)
{
}

void PlayerBase::LockSwitch(void)
{
	if (camera_.IsLockOnCamera())
	{
		//カメラを通常状態に以降
		camera_.ChangeMode(camera_.GetPreCameraMode());
		return;
	}

	const weak_ptr<CharactorBase> chara = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);
	if (chara.expired())
	{
		//敵が存在しない
		return;
	}

	camera_.ChangeMode(Camera::CAMERA_MODE::LOCKON);
	camera_.SetTarget(&chara.lock()->GetTransform());
}

void PlayerBase::Idle(void)
{
}

void PlayerBase::IsHit_Enemy(void)
{
	const float backVecLen = 1.5f;
	const int COL_TRYCNT_MAX = 10;

	//衝突判定用のモデルの制御情報
	Transform trans = transform_;

	//敵を取得
	weak_ptr<CharactorBase> enemy = 
	charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);
	const auto& enemyCol = enemy.lock()->GetCollider2D();
	
	const VECTOR& pCenterPos = collider2D_->GetColliderData().colCenterPos;
	Vector2f pXZPos = { pCenterPos.x,pCenterPos.z };
	
	const VECTOR& eCenterPos = enemyCol.GetColliderData().colCenterPos;
	Vector2f eXZPos = { eCenterPos.x,eCenterPos.z };

	//衝突相手から自機へのベクトル
	Vector2f vec = (
		pXZPos - eXZPos).Normalized();

	//10回まで衝突判定を行う
	for (int tryCnt = 0; tryCnt < COL_TRYCNT_MAX; tryCnt++)
	{
		if (!col2d_->CollisionCircle(*collider2D_, enemyCol))
		{
			//衝突していないため処理終了
			break;
		}

		//衝突相手から自機へのベクトルを元に座標をずらす
		vec *= backVecLen;
		movedPos_.x += vec.x;
		movedPos_.z += vec.y;

		//コライダと衝突判定用のモデル制御情報に反映
		trans.pos = movedPos_;
		collider2D_->UpdateTransform(trans);
	}

	//最終結果をモデルの制御情報の座標に反映
	transform_ = trans;
	transform_.Update();

	//移動用の座標を初期化
	movedPos_ = transform_.pos;
}

void PlayerBase::IsHit_EnemyShot1(void)
{
}

void PlayerBase::IsHit_EnemyMissile(void)
{
}

void PlayerBase::IsHit_EnemyBox(void)
{
}

const bool PlayerBase::IsAbleShield(void) const
{
	return state_ == State::Shield || state_ == State::ShieldCounterAttack;
}

const bool PlayerBase::IsShieldCounter(void) const
{
	return state_ == State::ShieldCounterAttack;
}

const bool PlayerBase::IsUltAttackAble(void) const
{
	return state_ == State::UltAttack;
}

const bool PlayerBase::IsUltChargeAble(void) const
{
	return state_ == State::UltCharge;
}

const bool PlayerBase::IsUltAble(void) const
{
	return IsUltAttackAble() || IsUltChargeAble();
}

const bool PlayerBase::IsStateError(void) const
{
	return state_ == State::None || state_ == State::MAX;
}

const bool PlayerBase::IsStateError(const State state) const
{
	return state == State::None || state == State::MAX;
}

const bool PlayerBase::IsAttackStateError(const AttackState state) const
{
	return state == AttackState::MAX;
}

const bool PlayerBase::IsNotNecessaryAttackStateUpdate(void) const
{
	return attackState_ == AttackState::None;
}

const bool PlayerBase::IsInvisible(void) const
{
	return invFrameCnt_ > 0.0f;
}

void PlayerBase::PlayerRotate(const Quaternion& qua)
{
	const float SlerpTime = 0.5f;

	transform_.quaRot = Quaternion::Slerp(preQuaRot_, qua, SlerpTime);
}

void PlayerBase::MoveDir(const VECTOR& deg, const VECTOR& dirAxis)
{
}

Vector2 PlayerBase::GetMpData(void) const
{
	int mp = GetUIDataToVariant<int>(SRC_UIData::PLAYER_MP);
	int mpMax = GetUIDataToVariant<int>(SRC_UIData::PLAYER_MPMAX);

	return Vector2(mp, mpMax);
}

Vector2f PlayerBase::GetUCData(void) const
{
	float uc = GetUIDataToVariant<float>(SRC_UIData::PLAYER_UC);
	float ucMax = GetUIDataToVariant<float>(SRC_UIData::PLAYER_UCMAX);

	return Vector2f(uc, ucMax);
}

Vector2 PlayerBase::GetHPData(void) const
{
	int hp = GetUIDataToVariant<int>(SRC_UIData::PLAYER_HP);
	int hpMax = GetUIDataToVariant<int>(SRC_UIData::PLAYER_HPMAX);

	return Vector2(hp, hpMax);
}

void PlayerBase::MpHeal(const int healVal)
{
	//x:現在のMP
	//y:最大MP
	Vector2 mpData = GetMpData();

	if (mpData.x >= mpData.y)
	{
		//MPが最大値
		return;
	}

	mpData.x += healVal;
	if (mpData.x >= mpData.y)
	{
		//MPが最大値以上
		mpData.x = mpData.y;
	}

	//データに反映
	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_MP, mpData.x);
}

void PlayerBase::MpHealMax(void)
{
	//x:現在のMP
	//y:最大MP
	Vector2 mpData = GetMpData();

	mpData.x = mpData.y;

	//データに反映
	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_MP, mpData.x);
}

void PlayerBase::UCHeal(const float healVal)
{
	//x:現在のUC
	//y:最大UC
	Vector2f ucData = GetUCData();
	
	if (ucData.x >= ucData.y)
	{
		//UCが最大値
		return;
	}

	ucData.x += healVal;
	if (ucData.x >= ucData.y)
	{
		//UCが最大値以上
		ucData.x = ucData.y;
	}

	//データに反映
	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_UC, ucData.x);
}

void PlayerBase::UCActive(void)
{
	//x:現在のUC
	//y:最大UC
	Vector2f ucData = GetUCData();

	ucData.x -= ucData.y;
	

	//データに反映
	uiDataMng_.SetUIData<float>(SRC_UIData::PLAYER_UC, ucData.x);
}

void PlayerBase::HpHeal(const int healVal)
{
	//x:現在のHP
	//y:最大HP
	Vector2 hpData = GetHPData();

	if (hpData.x >= hpData.y)
	{
		//UCが最大値
		return;
	}

	hpData.x += healVal;
	if (hpData.x >= hpData.y)
	{
		//UCが最大値以上
		hpData.x = hpData.y;
	}

	//データに反映
	uiDataMng_.SetUIData<int>(SRC_UIData::PLAYER_HP, hpData.x);
}

void PlayerBase::UpdateObserver_IsGroundPlayer(void)
{
	IsHitGround();
}

void PlayerBase::UpdateObserver_IsNotGroundPlayer(void)
{
	isGround_ = false;
}






