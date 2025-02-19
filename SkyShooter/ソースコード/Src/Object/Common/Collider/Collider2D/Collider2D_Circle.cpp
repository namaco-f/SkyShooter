#include "Collider2D_Circle.h"

const int DEBUG_SPHERE_DIVNUM = 6;

Collider2D_Circle::Collider2D_Circle(const ColliderData_2D& colData,const bool colliderDef)
	: Collider2D(colData, colliderDef)
{
	colType_ = COLLISION2D_TYPE::CIRCLE;
}

Collider2D_Circle::~Collider2D_Circle(void)
{
}

void Collider2D_Circle::Draw(void)
{
	DrawSphere3D(colData_.colCenterPos, colData_.radius, DEBUG_SPHERE_DIVNUM, 0x000000, 0x000000, false);
}
