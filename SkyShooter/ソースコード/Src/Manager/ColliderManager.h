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
/// �Փ˂�ʒm����N���X
/// </summary>
class ColliderManager
	: public Singleton<ColliderManager>
{

public:

	ColliderManager(void);
	virtual ~ColliderManager(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void UpdateCollider(void);

private:

	friend class Singleton<ColliderManager>;

	ActorManager& actorMng_;
	CharactorManager& charaMng_;
	ShotManager& shotMng_;
	Observer& observer_;

	//3D�̏Փ˔���
	std::unique_ptr<Collision> collision_;

	//2D�̏Փ˔���
	std::unique_ptr<Collision2D> collision2D_;

	std::mutex col2DMutex_;
	std::mutex col3Dmutex_;

	//�n�ʂƂ̔���
	void UpdateGround(void);

	//�L�����ƒe
	void UpdateShotCharactor(void);

	//2D�p�̍X�V����
	void UpdateCollider2D(void);

	//2D�̃L�����N�^�[���m�̔���
	void Update2DCharactor(
		const std::weak_ptr<CharactorBase> charaA,
		const std::weak_ptr<CharactorBase> charaB
	);

	//�G�̔��U���Ƃ̔���
	void Update2DEnemyAttackBox(
		const std::weak_ptr<CharactorBase> chara,
		const std::weak_ptr<CharactorBase> enemy
	);

	//�~���m�̏Փ˔���
	const bool IsHitCollision2D(
		const std::weak_ptr<CharactorBase> colA,
		const std::weak_ptr<CharactorBase> colB
	) const;

	//�n�ʂƂ̏Փ˔���
	const bool IsHitCollisionGround(
		const std::weak_ptr<CharactorBase> charactor,
		const std::weak_ptr<ActorBase> ground) const;

	//��`�Ƃ̏Փ˔���
	const bool IsHitCollisionBox(
		const std::weak_ptr<CharactorBase> charactor,
		const std::weak_ptr<ActorBase> box) const;

};

