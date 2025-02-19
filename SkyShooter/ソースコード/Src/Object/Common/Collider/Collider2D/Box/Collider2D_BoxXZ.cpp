
#include "../Utility/MathUtility.h"
#include "Collider2D_BoxXZ.h"

using Math = MathUtility;

Collider2D_BoxXZ::Collider2D_BoxXZ(const ColliderData_2D& colData,const bool colliderDef)
	: 
	Collider2D_Box(colData,colliderDef)
{
	colType_ = COLLISION2D_TYPE::BOX_XZ;
	CalculateBoxPos();
}

Collider2D_BoxXZ::~Collider2D_BoxXZ(void)
{
}

const VECTOR Collider2D_BoxXZ::Vector2fToVECTOR(const Vector2f& pos)
{
	return { pos.x,colData_.colCenterPos.y,pos.y };
}

void Collider2D_BoxXZ::CalculateBoxPos(void)
{
	Vector2f centerPos = { colData_.colCenterPos.x,colData_.colCenterPos.z };

	boxData_.posLeftTop = centerPos - colData_.size;
	boxData_.posLeftBottom = { centerPos.x - colData_.size.x ,centerPos.y + colData_.size.y };

	boxData_.posRightTop = { centerPos.x + colData_.size.x,centerPos.y - colData_.size.y };
	boxData_.posRightBottom = centerPos + colData_.size;
}

const VECTOR Collider2D_BoxXZ::GetDirection(void) const
{
	return Math::AXIS_Y;
}
