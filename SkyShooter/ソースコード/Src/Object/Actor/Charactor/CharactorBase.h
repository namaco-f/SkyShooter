#pragma once
#include <memory>
#include <unordered_map>
#include "../ActorBase.h"

class Collider;
class Collision;
class AnimationController;
class ShotBase;
class EnemyAttack_Box;
class EnemyAttack_Boxes;

/// <summary>
/// �L�����N�^�[���N���X
/// </summary>
class CharactorBase
	: public ActorBase
{

public:
	CharactorBase(void);
    virtual ~CharactorBase(void) override = default;

    /// <summary>
    /// �_���[�W
    /// </summary>
    /// <param name="damage">�_���[�W��</param>
    virtual void Damage(const int damage);

    /// <summary>
    /// ��������
    /// </summary>
    /// <returns>
    /// true : ����
    /// false : �������Ă��Ȃ�
    /// </returns>
    virtual const bool IsAlive(void) const override;

    /// <summary>
    /// �d�͂̍X�V
    /// </summary>
    void UpdateGravity(void);

    /// <summary>
    /// �G�̍U���̎擾
    /// </summary>
    const std::list<std::shared_ptr<EnemyAttack_Box>>& GetAttackBox(void) const;

protected:

    //�A�j���[�V�����R���g���[���[
    std::unique_ptr<AnimationController> animationController_;

    std::unique_ptr<Collision> collision_;

    //hp
    int hp_;

    //hp�ő�l
    int hpMax_;   

    std::unique_ptr<EnemyAttack_Boxes> enemyAttackboxes_;

    //�d��
    VECTOR gravityPow_;

    //hp�����ȉ�
    virtual void Destroy(void) = 0;

    //�A�j���[�V�����̐ݒ�
    virtual void InitAnimation(void) = 0;

    virtual void IsHitGround(void);

private:
};

