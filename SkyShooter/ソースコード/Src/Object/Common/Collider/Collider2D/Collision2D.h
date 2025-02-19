#pragma once

#include <unordered_map>
#include <functional>
#include "Collider2DTag.h"

class Collider2D;

/// <summary>
/// 2D�̏Փ˔���
/// </summary>
class Collision2D
{
public:

	Collision2D(void);
	~Collision2D(void);
	
	/// <summary>
	/// 2D�̏Փ˔���
	/// </summary>
	/// <param name="colA">Collider2D</param>
	/// <param name="colB">Collider2D</param>
	/// <returns>
	/// true : �Փ˂��Ă���
	/// false : �Փ˂��Ă��Ȃ�
	/// </returns>
	const bool IsHitCollider(const Collider2D& colA, const Collider2D& colB);

	/// <summary>
	/// �~���m�̔���
	/// </summary>
	const bool CollisionCircle(const Collider2D& colA, const Collider2D& colB);

	/// <summary>
	/// ��`�Ƃ̔���
	/// </summary>
	const bool IsCollisionBox(const Collider2D& colBox, const Collider2D& col);

private:

	//�R���C�_�̎�ނ��Ƃ̏Փ˔���
	std::unordered_map<COLLISION2D_TYPE,
		std::function<const bool(const Collider2D&, const Collider2D&)>> colTypeCollision_;

	//�w��̃R���C�_���~�R���C�_��
	const bool IsColliderType_Circle(const COLLISION2D_TYPE colType);

	//�w��̃R���C�_����`�R���C�_��
	const bool IsColliderType_Box(const COLLISION2D_TYPE colType);

	//XY���ʏ�̋�`�Ƃ̏Փ˔���
	const bool IsCollisionBoxXY(const Collider2D& colBox, const Collider2D& col);

	//XZ���ʏ�̋�`�Ƃ̏Փ˔���
	const bool IsCollisionBoxXZ(const Collider2D& colBox, const Collider2D& col);

	//ZY���ʏ�̋�`�Ƃ̏Փ˔���
	const bool IsCollisionBoxZY(const Collider2D& colBox, const Collider2D& col);

	//��`�Ɖ~�̏Փ˔���
	const bool CollisionBox(
		const Vector2f& boxCenterPos, const Vector2f& circleCenterPos,
		const ColliderData_2D& boxData, const ColliderData_2D& circleData);


};

