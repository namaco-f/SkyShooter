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
		//自機が存在しない
		return;
	}

	weak_ptr<ActorBase> ground = actorMng_.GetActor<ActorBase>(OBJECT_TYPE::STAGE);
	if (ground.expired())
	{
		//地面が存在しない
		return;
	}

	if (IsHitCollisionGround(player,ground))
	{
		//接地している
		observer_.SendMessageList(Message::IsGroundPlayer);
	}
	else
	{
		//接地していない
		observer_.SendMessageList(Message::IsNotGroundPlayer);
	}
}

void ColliderManager::UpdateCollider2D(void)
{
	weak_ptr<CharactorBase> player = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::PLAYER);
	if (player.expired())
	{
		//自機が存在しない
		return;
	}

	weak_ptr<CharactorBase> enemy = charaMng_.GetCharactor<CharactorBase>(OBJECT_TYPE::ENEMY);
	if (enemy.expired())
	{
		//敵が存在しない
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
		//キャラクターの配列が存在しない
		return;
	}

	auto& shots = shotMng_.GetShots();
	if (shots.empty())
	{
		//弾の配列が存在しない
		return;
	}

	//キャラと弾
	for (const weak_ptr<CharactorBase> chara : charactors)
	{
		if (chara.expired()) 
		{
			//キャラの実体が存在しない
			continue; 
		}

		const auto& charaCol = chara.lock()->GetCollider();
		if (!charaCol.GetAbleCol()) 
		{
			//キャラの衝突判定が無効
			continue; 
		}

		for (const auto& shot : shots)
		{
			if (shot == nullptr) 
			{
				//弾の実体が存在しない
				continue; 
			}

			const auto& shotCol = shot->GetCollider();
			if (!shotCol.GetAbleCol()) 
			{
				//弾の衝突判定が無効
				continue; 
			}

			if (collision_->GetColResultHitColInfo(charaCol, shotCol).isHit)
			{
				//衝突を通知
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
		//衝突の通知
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
		//敵の箱攻撃配列が存在しない
		return;
	}

	for (const weak_ptr<EnemyAttack_Box> enemyAttack : enemyAttacks)
	{
		if (enemyAttack.expired())
		{
			//敵の箱攻撃が存在しない
			continue;
		}

		if (IsHitCollisionBox(chara, enemyAttack))
		{
			//衝突通知
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
