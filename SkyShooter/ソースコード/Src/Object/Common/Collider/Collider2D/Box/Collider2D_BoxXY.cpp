
#include "../Utility/MathUtility.h"
#include "Collider2D_BoxXY.h"

using Math = MathUtility;

Collider2D_BoxXY::Collider2D_BoxXY(const ColliderData_2D& colData, const bool colliderDef)
	:
	Collider2D_Box(colData,colliderDef)
{
	colType_ = COLLISION2D_TYPE::BOX_XY;
	CalculateBoxPos();
}

Collider2D_BoxXY::~Collider2D_BoxXY(void)
{
}

const VECTOR Collider2D_BoxXY::Vector2fToVECTOR(const Vector2f& pos)
{
	return { pos.x,pos.y,colData_.colCenterPos.z };
}

void Collider2D_BoxXY::CalculateBoxPos(void)
{
	Vector2f centerPos = { colData_.colCenterPos.x,colData_.colCenterPos.y };

	boxData_.posLeftTop = centerPos - colData_.size;
	boxData_.posLeftBottom = { centerPos.x - colData_.size.x ,centerPos.y + colData_.size.y };

	boxData_.posRightTop = { centerPos.x + colData_.size.x,centerPos.y - colData_.size.y };
	boxData_.posRightBottom = centerPos + colData_.size;
}

const VECTOR Collider2D_BoxXY::GetDirection(void) const
{
    return VScale(Math::AXIS_Z,-1.0f);
}
