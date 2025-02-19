
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

const int MIN_HP = 0;						//hp����
const int INIT_HP = 100;					//hp�����l
const int INIT_MAX_HP = 100;				//�ő�hp�����l

const float GRAVITY_POW_LIMIT = 20.0f;		//�d�͂̏��
const float GRAVITY_POW = 10.0f;			//�d�͂̑傫��

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
		//�ڒn���Ă��Ȃ����d�͗L��
		if (gravityPow_.y >= -GRAVITY_POW_LIMIT)
		{
			//�d�͂̑傫�����
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
		//�ڒn���Ă���A�܂��͏d�͖������
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

	//�Փ˔���p�̃��f���̐�����
	Transform trans = transform_;

	//�n�ʂ��擾
	weak_ptr<ActorBase> ground =
		actMng.GetActor<ActorBase>(OBJECT_TYPE::STAGE);

	if (ground.expired())
	{
		//�n�ʂ����݂��Ă��Ȃ�
		return;
	}

	const auto& groundCol = ground.lock()->GetCollider();

	//10��܂ŏՓ˔�����s��
	for (int tryCnt = 0; tryCnt < COL_TRYCNT_MAX; tryCnt++)
	{
		auto colResult = collision_->GetCollResultPoly_Line(groundCol, *colliderFoot_);
		if (colResult.HitPosition.y - trans.pos.y < OFFSET_SHADOW_Y)
		{
			//�e���n�ʂɂ߂荞�܂Ȃ�Y���W
			break;
		}

		//�Փˑ��肩�玩�@�ւ̃x�N�g�������ɍ��W�����炷
		VECTOR vec = VScale(MathUtility::DIR_U, COL_BACKVECTOR_LENGTH);
		movedPos_ = VAdd(movedPos_, vec);

		//�R���C�_�ƏՓ˔���p�̃��f��������ɔ��f
		trans.pos = movedPos_;
		colliderFoot_->UpdateTransform(trans);
	}

	//�ŏI���ʂ����f���̐�����̍��W�ɔ��f
	transform_ = trans;
	transform_.Update();

	//�ړ��p�̍��W��������
	movedPos_ = transform_.pos;
}
