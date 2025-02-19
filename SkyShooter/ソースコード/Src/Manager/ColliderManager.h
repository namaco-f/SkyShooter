#pragma once

#include <memory>
#include <mutex>
#include "../Object/ObjectTag.h"
#include "../Singleton.h"

class Collider;
class Collision;
class Collision2D;
class ActorManager;
class CharactorManager;
class ShotManager;
class ActorBase;
class CharactorBase;
class Observer;

/// <summary>
/// 衝突を通知するクラス
/// </summary>
class ColliderManager
	: public Singleton<ColliderManager>
{

public:

	ColliderManager(void);
	virtual ~ColliderManager(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void UpdateCollider(void);

private:

	friend class Singleton<ColliderManager>;

	ActorManager& actorMng_;
	CharactorManager& charaMng_;
	ShotManager& shotMng_;
	Observer& observer_;

	//3Dの衝突判定
	std::unique_ptr<Collision> collision_;

	//2Dの衝突判定
	std::unique_ptr<Collision2D> collision2D_;

	std::mutex col2DMutex_;
	std::mutex col3Dmutex_;

	//地面との判定
	void UpdateGround(void);

	//キャラと弾
	void UpdateShotCharactor(void);

	//2D用の更新処理
	void UpdateCollider2D(void);

	//2Dのキャラクター同士の判定
	void Update2DCharactor(
		const std::weak_ptr<CharactorBase> charaA,
		const std::weak_ptr<CharactorBase> charaB
	);

	//敵の箱攻撃との判定
	void Update2DEnemyAttackBox(
		const std::weak_ptr<CharactorBase> chara,
		const std::weak_ptr<CharactorBase> enemy
	);

	//円同士の衝突判定
	const bool IsHitCollision2D(
		const std::weak_ptr<CharactorBase> colA,
		const std::weak_ptr<CharactorBase> colB
	) const;

	//地面との衝突判定
	const bool IsHitCollisionGround(
		const std::weak_ptr<CharactorBase> charactor,
		const std::weak_ptr<ActorBase> ground) const;

	//矩形との衝突判定
	const bool IsHitCollisionBox(
		const std::weak_ptr<CharactorBase> charactor,
		const std::weak_ptr<ActorBase> box) const;

};

