
#include "../Utility/MathUtility.h"
#include "Collider2D_BoxZY.h"

using Math = MathUtility;

Collider2D_BoxZY::Collider2D_BoxZY(const ColliderData_2D& colData, const bool colliderDef)
	: 
	Collider2D_Box(colData,colliderDef)
{
	colType_ = COLLISION2D_TYPE::BOX_ZY;
	CalculateBoxPos();
}

Collider2D_BoxZY::~Collider2D_BoxZY(void)
{
}

const VECTOR Collider2D_BoxZY::Vector2fToVECTOR(const Vector2f& pos)
{
	return { colData_.colCenterPos.x,pos.y,pos.x };
}

void Collider2D_BoxZY::CalculateBoxPos(void)
{
	Vector2f centerPos = { colData_.colCenterPos.z,colData_.colCenterPos.y };

	boxData_.posLeftTop = centerPos - colData_.size;
	boxData_.posLeftBottom = { centerPos.x - colData_.size.x ,centerPos.y + colData_.size.y };

	boxData_.posRightTop = { centerPos.x + colData_.size.x,centerPos.y - colData_.size.y };
	boxData_.posRightBottom = centerPos + colData_.size;
}

const VECTOR Collider2D_BoxZY::GetDirection(void) const
{
	return Math::AXIS_X;
}
