#pragma once
#include "EnemyAttack_Box.h"

/// <summary>
/// XY���ʏ�ɋ�`����������锠�U��
/// </summary>
class EnemyAttack_BoxXY :
    public EnemyAttack_Box
{

public:

	EnemyAttack_BoxXY(void) = default;
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="transform">���f���̐�����</param>
	/// <param name="boxSize">�c���T�C�Y</param>
	/// <param name="boxAttackData">�U���̏��</param>
	/// <param name="boxType">��`�̎��</param>
	explicit EnemyAttack_BoxXY(
		const Transform& transform,
		const Vector2f& boxSize,
		const BoxAttackData& boxAttackData
	);

	virtual ~EnemyAttack_BoxXY(void);

protected:

	virtual void InitEffect(void) override;

	//2D�R���C�_�̐���
	virtual void MakeBoxCollider(void) override;

	//�U���͈͂̃|���S���̐���
	virtual void CalculateBoxPos(void) override;

	//���f���̃X�P�[�����擾
	virtual const VECTOR GetBoxScale(void) override;

	virtual const VECTOR GetBoxChangeScale(void) override;

};

