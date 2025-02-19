#pragma once

#include <unordered_map>
#include <functional>
#include "Collider2DTag.h"

class Collider2D;

/// <summary>
/// 2Dの衝突判定
/// </summary>
class Collision2D
{
public:

	Collision2D(void);
	~Collision2D(void);
	
	/// <summary>
	/// 2Dの衝突判定
	/// </summary>
	/// <param name="colA">Collider2D</param>
	/// <param name="colB">Collider2D</param>
	/// <returns>
	/// true : 衝突している
	/// false : 衝突していない
	/// </returns>
	const bool IsHitCollider(const Collider2D& colA, const Collider2D& colB);

	/// <summary>
	/// 円同士の判定
	/// </summary>
	const bool CollisionCircle(const Collider2D& colA, const Collider2D& colB);

	/// <summary>
	/// 矩形との判定
	/// </summary>
	const bool IsCollisionBox(const Collider2D& colBox, const Collider2D& col);

private:

	//コライダの種類ごとの衝突判定
	std::unordered_map<COLLISION2D_TYPE,
		std::function<const bool(const Collider2D&, const Collider2D&)>> colTypeCollision_;

	//指定のコライダが円コライダか
	const bool IsColliderType_Circle(const COLLISION2D_TYPE colType);

	//指定のコライダが矩形コライダか
	const bool IsColliderType_Box(const COLLISION2D_TYPE colType);

	//XY平面上の矩形との衝突判定
	const bool IsCollisionBoxXY(const Collider2D& colBox, const Collider2D& col);

	//XZ平面上の矩形との衝突判定
	const bool IsCollisionBoxXZ(const Collider2D& colBox, const Collider2D& col);

	//ZY平面上の矩形との衝突判定
	const bool IsCollisionBoxZY(const Collider2D& colBox, const Collider2D& col);

	//矩形と円の衝突判定
	const bool CollisionBox(
		const Vector2f& boxCenterPos, const Vector2f& circleCenterPos,
		const ColliderData_2D& boxData, const ColliderData_2D& circleData);


};

