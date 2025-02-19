
#include "../Manager/TimeManager.h"
#include "../Manager/ActorsManager/Actor/ActorManager.h"
#include "../Utility/MathUtility.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Collider2D.h"
#include "../Object/Common/Collider/Collision.h"
#include "../Object/Common/AnimationController.h"
#include "Enemy/BoxAttack/EnemyAttack_Boxes.h"
#include "../Observer/Observer.h"
#include "CharactorBase.h"

using namespace std;

const int MIN_HP = 0;						//hp下限
const int INIT_HP = 100;					//hp初期値
const int INIT_MAX_HP = 100;				//最大hp初期値

const float GRAVITY_POW_LIMIT = 20.0f;		//重力の上限
const float GRAVITY_POW = 10.0f;			//重力の大きさ

const float COL_BACKVECTOR_LENGTH = 1.0f;
const int COL_TRYCNT_MAX = 10;
const float OFFSET_SHADOW_Y = 5.0f;

CharactorBase::CharactorBase(void)
	:
	ActorBase(),
	animationController_(nullptr),
	collision_(make_unique<Collision>()),
	hp_(INIT_HP),
	hpMax_(INIT_MAX_HP),
	enemyAttackboxes_(make_unique<EnemyAttack_Boxes>()),
	gravityPow_{}
{
}

void CharactorBase::Damage(const int damage)
{
	hp_ -= damage;

	if (!IsAlive())
	{
		Destroy();
	}
}

const bool CharactorBase::IsAlive(void) const
{
	return hp_ > MIN_HP;
}

void CharactorBase::UpdateGravity(void)
{
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();
	
	if (!isGround_ && !InvalidGravity())
	{
		//接地していないかつ重力有効
		if (gravityPow_.y >= -GRAVITY_POW_LIMIT)
		{
			//重力の大きさ上限
			gravityPow_.y = -GRAVITY_POW_LIMIT;
			return;
		}

		gravityPow_ = VAdd(
			gravityPow_,
			VScale(
				VScale(MathUtility::DIR_D, GRAVITY_POW),
				deltaTime
			)
		);
	}
	else
	{
		//接地している、または重力無効状態
		gravityPow_ = VECTOR();
	}
}

const std::list<std::shared_ptr<EnemyAttack_Box>>& CharactorBase::GetAttackBox(void) const
{
	return enemyAttackboxes_->GetEnemyAttackBoxes();
}

void CharactorBase::IsHitGround(void)
{
	isGround_ = true;

	auto& actMng = ActorManager::GetInstance();

	//衝突判定用のモデルの制御情報
	Transform trans = transform_;

	//地面を取得
	weak_ptr<ActorBase> ground =
		actMng.GetActor<ActorBase>(OBJECT_TYPE::STAGE);

	if (ground.expired())
	{
		//地面が存在していない
		return;
	}

	const auto& groundCol = ground.lock()->GetCollider();

	//10回まで衝突判定を行う
	for (int tryCnt = 0; tryCnt < COL_TRYCNT_MAX; tryCnt++)
	{
		auto colResult = collision_->GetCollResultPoly_Line(groundCol, *colliderFoot_);
		if (colResult.HitPosition.y - trans.pos.y < OFFSET_SHADOW_Y)
		{
			//影が地面にめり込まないY座標
			break;
		}

		//衝突相手から自機へのベクトルを元に座標をずらす
		VECTOR vec = VScale(MathUtility::DIR_U, COL_BACKVECTOR_LENGTH);
		movedPos_ = VAdd(movedPos_, vec);

		//コライダと衝突判定用のモデル制御情報に反映
		trans.pos = movedPos_;
		colliderFoot_->UpdateTransform(trans);
	}

	//最終結果をモデルの制御情報の座標に反映
	transform_ = trans;
	transform_.Update();

	//移動用の座標を初期化
	movedPos_ = transform_.pos;
}
