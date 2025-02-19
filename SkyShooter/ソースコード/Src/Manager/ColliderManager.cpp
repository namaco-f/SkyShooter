#include <algorithm>
#include <thread>
#include <mutex>
#include "ActorsManager/Charactor/CharactorManager.h"
#include "ActorsManager/Actor/ActorManager.h"
#include "ActorsManager/Shot/ShotManager.h"
#include "../Observer/Observer.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Actor/Charactor/CharactorBase.h"
#include "../Object/Actor/Charactor/Enemy/BoxAttack/EnemyAttack_Box.h"
#include "../Object/Actor/Shot/ShotBase.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Collider2D.h"
#include "../Object/Common/Collider/Collision.h"
#include "../Object/Common/Collider/Collider2D/Collision2D.h"
#include "ColliderManager.h"

using namespace std;

ColliderManager* Singleton<ColliderManager>::instance_ = nullptr;

ColliderManager::ColliderManager(void)
	:
	actorMng_(ActorManager::GetInstance()),
	charaMng_(CharactorManager::GetInstance()),
	shotMng_(ShotManager::GetInstance()),
	observer_(Observer::GetInstance()),
	collision_(make_unique<Collision>()),
	collision2D_(make_unique<Collision2D>())
{
}

ColliderManager::~ColliderManager(void)
{
}

void ColliderManager::UpdateCollider(void)
{
	UpdateGround();

	std::thread thread1(bind(&ColliderManager::UpdateCollider2D, this));
	std::thread thread2(bind(&ColliderManager::UpdateShotCharactor,this));

	thread1.join();
	thread2.join();
}

void ColliderManager::UpdateGround(void)
{
	weak_ptr<CharactorBase> player = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::PLAYER);
	if (player.expired())
	{
		//���@�����݂��Ȃ�
		return;
	}

	weak_ptr<ActorBase> ground = actorMng_.GetActor<ActorBase>(OBJECT_TYPE::STAGE);
	if (ground.expired())
	{
		//�n�ʂ����݂��Ȃ�
		return;
	}

	if (IsHitCollisionGround(player,ground))
	{
		//�ڒn���Ă���
		observer_.SendMessageList(Message::IsGroundPlayer);
	}
	else
	{
		//�ڒn���Ă��Ȃ�
		observer_.SendMessageList(Message::IsNotGroundPlayer);
	}
}

void ColliderManager::UpdateCollider2D(void)
{
	weak_ptr<CharactorBase> player = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::PLAYER);
	if (player.expired())
	{
		//���@�����݂��Ȃ�
		return;
	}

	weak_ptr<CharactorBase> enemy = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);
	if (enemy.expired())
	{
		//�G�����݂��Ȃ�
		return;
	}

	Update2DCharactor(player, enemy);
	Update2DEnemyAttackBox(player, enemy);
}

void ColliderManager::UpdateShotCharactor(void)
{
	lock_guard<mutex> lock(col3Dmutex_);

	auto& charactors = charaMng_.GetCharactors();
	if (charactors.empty())
	{
		//�L�����N�^�[�̔z�񂪑��݂��Ȃ�
		return;
	}

	auto& shots = shotMng_.GetShots();
	if (shots.empty())
	{
		//�e�̔z�񂪑��݂��Ȃ�
		return;
	}

	//�L�����ƒe
	for (const weak_ptr<CharactorBase> chara : charactors)
	{
		if (chara.expired()) 
		{
			//�L�����̎��̂����݂��Ȃ�
			continue; 
		}

		const auto& charaCol = chara.lock()->GetCollider();
		if (!charaCol.GetAbleCol()) 
		{
			//�L�����̏Փ˔��肪����
			continue; 
		}

		for (const auto& shot : shots)
		{
			if (shot == nullptr) 
			{
				//�e�̎��̂����݂��Ȃ�
				continue; 
			}

			const auto& shotCol = shot->GetCollider();
			if (!shotCol.GetAbleCol()) 
			{
				//�e�̏Փ˔��肪����
				continue; 
			}

			if (collision_->GetColResultHitColInfo(charaCol, shotCol).isHit)
			{
				//�Փ˂�ʒm
				chara.lock()->IsHitNotice(shot->GetObjectType());
				shot->IsHitNotice(chara.lock()->GetObjectType());
			}
		}
	}
}

void ColliderManager::Update2DCharactor(
	const std::weak_ptr<CharactorBase> charaA, const std::weak_ptr<CharactorBase> charaB)
{
	lock_guard<mutex> lock(col2DMutex_);

	if (IsHitCollision2D(charaA, charaB))
	{
		//�Փ˂̒ʒm
		charaA.lock()->IsHitNotice(charaB.lock()->GetObjectType());
		charaB.lock()->IsHitNotice(charaA.lock()->GetObjectType());
	}
}

void ColliderManager::Update2DEnemyAttackBox(
	const std::weak_ptr<CharactorBase> chara, const std::weak_ptr<CharactorBase> enemy)
{
	lock_guard<mutex> lock(col2DMutex_);

	const auto& enemyAttacks = enemy.lock()->GetAttackBox();
	if (enemyAttacks.empty())
	{
		//�G�̔��U���z�񂪑��݂��Ȃ�
		return;
	}

	for (const weak_ptr<EnemyAttack_Box> enemyAttack : enemyAttacks)
	{
		if (enemyAttack.expired())
		{
			//�G�̔��U�������݂��Ȃ�
			continue;
		}

		if (IsHitCollisionBox(chara, enemyAttack))
		{
			//�Փ˒ʒm
			chara.lock()->IsHitNotice(enemyAttack.lock()->GetObjectType());
		}
	}
}

const bool ColliderManager::IsHitCollision2D(const weak_ptr<CharactorBase> colA, const weak_ptr<CharactorBase> colB) const
{
	return collision2D_->IsHitCollider(
		colA.lock()->GetCollider2D(),
		colB.lock()->GetCollider2D()
	);
}

const bool ColliderManager::IsHitCollisionGround(const std::weak_ptr<CharactorBase> charactor, const std::weak_ptr<ActorBase> ground) const
{
	return collision_->GetCollResultPoly_Line(
		charactor.lock()->GetColliderFoot(),
		ground.lock()->GetCollider()
	).HitFlag;
}

const bool ColliderManager::IsHitCollisionBox(const std::weak_ptr<CharactorBase> charactor,
	const std::weak_ptr<ActorBase> box) const
{
	return collision2D_->IsHitCollider(
		charactor.lock()->GetCollider2D(),
		box.lock()->GetCollider2D()
	);
}
