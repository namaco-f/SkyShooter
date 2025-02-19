#include "Collider2D.h"

Collider2D::Collider2D(const ColliderData_2D& colData, const bool colliderDef)
	:
	colType_(COLLISION2D_TYPE::CIRCLE),
	colData_(colData),
	isAbleCollider_(colliderDef)
{
}

Collider2D::~Collider2D(void)
{
}

void Collider2D::Draw(void)
{
}

void Collider2D::UpdateTransform(const Transform& trans)
{
	colData_.colCenterPos = { trans.pos.x,trans.pos.y,trans.pos.z };
}
