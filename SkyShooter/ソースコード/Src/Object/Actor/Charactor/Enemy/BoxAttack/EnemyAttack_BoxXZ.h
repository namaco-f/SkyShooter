#pragma once
#include "EnemyAttack_Box.h"

/// <summary>
/// XZ���ʏ�ɋ�`����������锠�U��
/// </summary>
class EnemyAttack_BoxXZ :
    public EnemyAttack_Box
{
public:

    EnemyAttack_BoxXZ(void) = default;
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="transform">���f���̐�����</param>
	/// <param name="boxSize">�c���T�C�Y</param>
	/// <param name="boxAttackData">�U���̏��</param>
	/// <param name="boxType">��`�̎��</param>
	EnemyAttack_BoxXZ(
		const Transform& transform,
		const Vector2f& boxSize,
		const BoxAttackData& boxAttackData
	);

    virtual ~EnemyAttack_BoxXZ(void);

protected:

	virtual void InitEffect(void) override;

	virtual void AttackEffectDraw(void) override;

	//2D�R���C�_�̐���
	virtual void MakeBoxCollider(void) override;

	//���f���̃X�P�[�����擾
	virtual const VECTOR GetBoxScale(void) override;

	virtual const VECTOR GetBoxChangeScale(void) override;

};

