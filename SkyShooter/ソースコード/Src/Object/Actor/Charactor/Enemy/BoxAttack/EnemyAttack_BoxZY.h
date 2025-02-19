#pragma once
#include "EnemyAttack_Box.h"

/// <summary>
/// ZY���ʏ�ɋ�`����������锠�U��
/// </summary>
class EnemyAttack_BoxZY :
    public EnemyAttack_Box
{

public:

	EnemyAttack_BoxZY(void) = default;
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="transform">���f���̐�����</param>
	/// <param name="boxSize">�c���T�C�Y</param>
	/// <param name="boxAttackData">�U���̏��</param>
	/// <param name="boxType">��`�̎��</param>
	EnemyAttack_BoxZY(
		const Transform& transform,
		const Vector2f& boxSize,
		const BoxAttackData& boxAttackData
	);

	virtual ~EnemyAttack_BoxZY(void);


protected:

	//2D�R���C�_�̐���
	virtual void MakeBoxCollider(void) override;

	//���f���̃X�P�[�����擾
	virtual const VECTOR GetBoxScale(void) override;

	virtual const VECTOR GetBoxChangeScale(void) override;

};

