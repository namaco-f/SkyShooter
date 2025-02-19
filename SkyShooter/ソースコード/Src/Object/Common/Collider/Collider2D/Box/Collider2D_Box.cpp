#include "Collider2D_Box.h"

const unsigned int COLLISION_ABLE_COLOR = 0xff0000;
const unsigned int COLLISION_UNABLE_COLOR = 0xffff00;
const float DEBUG_LENGTH = 5000.0f;
const float DEBUG_RADIUS = 50.0f;
const int DEBUG_SPHERE_DIVNUM = 6;

Collider2D_Box::Collider2D_Box(const ColliderData_2D& colData, const bool colliderDef)
	:
	Collider2D(colData, colliderDef)
{
}

Collider2D_Box::~Collider2D_Box(void)
{
}

void Collider2D_Box::Draw(void)
{
	unsigned int color = GetColliderColor();

	DrawSphere3D(Vector2fToVECTOR(boxData_.posLeftTop), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);
	DrawSphere3D(Vector2fToVECTOR(boxData_.posLeftBottom), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);
	DrawSphere3D(Vector2fToVECTOR(boxData_.posRightTop), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);
	DrawSphere3D(Vector2fToVECTOR(boxData_.posRightBottom), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);

	DrawLine3D(
		Vector2fToVECTOR(boxData_.posLeftTop), Vector2fToVECTOR(boxData_.posLeftBottom),
		color);
	DrawLine3D(
		Vector2fToVECTOR(boxData_.posLeftBottom), Vector2fToVECTOR(boxData_.posRightTop),
		color);
	DrawLine3D(
		Vector2fToVECTOR(boxData_.posRightTop), Vector2fToVECTOR(boxData_.posRightBottom),
		color);
	DrawLine3D(
		Vector2fToVECTOR(boxData_.posRightBottom), Vector2fToVECTOR(boxData_.posLeftTop),
		color);

	const VECTOR& length = VScale(GetDirection(), DEBUG_LENGTH);
	
	DrawSphere3D(
		VAdd(Vector2fToVECTOR(boxData_.posLeftTop), length), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);
	DrawSphere3D(
		VAdd(Vector2fToVECTOR(boxData_.posLeftBottom), length), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);
	DrawSphere3D(
		VAdd(Vector2fToVECTOR(boxData_.posRightTop), length), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);
	DrawSphere3D(
		VAdd(Vector2fToVECTOR(boxData_.posRightBottom), length), DEBUG_RADIUS, DEBUG_SPHERE_DIVNUM, color, color, false);

	DrawLine3D(
		Vector2fToVECTOR(boxData_.posLeftTop), VAdd(Vector2fToVECTOR(boxData_.posLeftTop), length),
		color);
	DrawLine3D(
		Vector2fToVECTOR(boxData_.posLeftBottom), VAdd(Vector2fToVECTOR(boxData_.posLeftBottom), length),
		color);
	DrawLine3D(
		Vector2fToVECTOR(boxData_.posRightTop), VAdd(Vector2fToVECTOR(boxData_.posRightTop), length),
		color);
	DrawLine3D(
		Vector2fToVECTOR(boxData_.posRightBottom), VAdd(Vector2fToVECTOR(boxData_.posRightBottom), length),
		color);

}

void Collider2D_Box::UpdateTransform(const Transform& trans)
{
	colData_.colCenterPos = { trans.pos.x,trans.pos.y,trans.pos.z };
	CalculateBoxPos();
}

const unsigned int Collider2D_Box::GetColliderColor(void) const
{
	return isAbleCollider_ ? COLLISION_ABLE_COLOR : COLLISION_UNABLE_COLOR;
}
