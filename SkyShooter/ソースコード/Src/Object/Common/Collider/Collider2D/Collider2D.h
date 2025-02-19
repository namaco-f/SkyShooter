#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "Collider2DTag.h"
#include "../../Transform.h"
#include "../Common/Vector2/Vector2f.h"

/// <summary>
/// 2D�R���C�_�̐�����
/// </summary>
struct ColliderData_2D
{
	VECTOR colCenterPos;		//���S���W
	Vector2f size;				//���c�T�C�Y
	float radius;				//���a
};

/// <summary>
/// 2D�R���C�_�̊��N���X
/// </summary>
class Collider2D
{
public:

	Collider2D(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="colData">�R���C�_���</param>
	explicit Collider2D(const ColliderData_2D& colData,const bool colliderDef = true);
	virtual ~Collider2D(void);

	/// <summary>
	/// �R���C�_�̕`��
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// ������̍X�V
	/// </summary>
	/// <param name="trans">������</param>
	virtual void UpdateTransform(const Transform& trans);

	/// <summary>
	/// �R���C�_�̎�ʂ��擾
	/// </summary>
	/// <returns>colType_</returns>
	virtual inline const COLLISION2D_TYPE& GetColliderType(void) const
	{
		return colType_;
	}

	/// <summary>
	/// �R���C�_�̏����擾
	/// </summary>
	/// <returns>colData_</returns>
	virtual inline const ColliderData_2D& GetColliderData(void) const
	{
		return colData_;
	}

	/// <summary>
	/// �R���C�_�̏�Ԃ𔽓]
	/// </summary>
	virtual inline void SwitchAbleCollider(void)
	{
		isAbleCollider_ = !isAbleCollider_;
	}

	/// <summary>
	/// �R���C�_��Ԃ��擾
	/// </summary>
	/// <returns>isAbleCollider_</returns>
	virtual inline const bool IsAbleCollider(void) const
	{
		return isAbleCollider_;
	}

protected:

	//�R���C�_�̎��
	COLLISION2D_TYPE colType_;

	//�R���C�_�̏��
	ColliderData_2D colData_;

	//�R���C�_���L��������Ă��邩
	bool isAbleCollider_;

};

