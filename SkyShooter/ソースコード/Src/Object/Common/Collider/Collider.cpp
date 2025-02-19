
//#include <Dxlib.h>
//#include "Collider.h"
#include "../Transform.h"
#include "Collider.h"

using namespace std;

const unsigned int DEBUG_COLLIDER_COLOR_SPHERE = 0xff0000;
const unsigned int DEBUG_COLLIDER_COLOR_CAPSULE = 0x00ff00;
const unsigned int DEBUG_COLLIDER_COLOR_MODEL = 0x0000ff;
const unsigned int DEBUG_COLLIDER_COLOR_LINE = 0xffffff;

Collider::Collider(const ColliderData& colData)
	:
	localPos_{}
{
	InitColTypeDraw();
	colData_ = colData;
	SetColTypeDraw(colData_.colType_);
}

void Collider::UpdateCollider(const ColliderData& colData)
{
	colData_ = colData;
	colData_.transform_.localPos = localPos_;
}

void Collider::UpdateTransform(const Transform& transform)
{
	colData_.preTransform_ = colData_.transform_;
	colData_.transform_ = transform;
	colData_.transform_.localPos = localPos_;
}

void Collider::UpdateTransformQuaRot(const Quaternion& quaternion)
{
	colData_.preTransform_ = colData_.transform_;
	colData_.transform_.quaRot = quaternion;
}

void Collider::SetLocalPos(const VECTOR& localPos)
{
	localPos_ = localPos;
}

void Collider::Draw(void)
{
	if (!colTypeDraw_ || !colData_.ableCol_)
	{
		return;
	}
	colTypeDraw_();
}

VECTOR Collider::GetPosTop(void) const
{
	VECTOR ret = GetRotPos(colData_.localPosTop_);
	return ret;
}

VECTOR Collider::GetPosDown(void) const
{
	VECTOR ret = GetRotPos(colData_.localPosBottom_);

	return ret;
}

const VECTOR& Collider::GetPos(void) const
{
	if (colData_.colModelFrameIdx_ != -1)
	{
		return colData_.localPosCenter_;
	}

	return colData_.transform_.pos;
}

void Collider::SetPosTop(const VECTOR& val)
{
	colData_.localPosTop_ = val;
}

void Collider::SetPosDown(const VECTOR& val)
{
	colData_.localPosBottom_ = val;
}

void Collider::SetPosCenter(const VECTOR& val)
{
	colData_.localPosCenter_ = val;
}

void Collider::SetRadius(const float& val)
{
	colData_.radius_ = val;
}

void Collider::SetFrameIndex(const int& val)
{
	colData_.colModelFrameIdx_ = val;
}

void Collider::SetColAble(const bool val)
{
	colData_.ableCol_ = val;
}

VECTOR Collider::GetRotPos(const VECTOR& localPos) const
{
	Quaternion q = colData_.transform_.quaRot.Mult(colData_.transform_.quaRotLocal);
	q = q.Mult(colData_.localQuaRot_);


	VECTOR localRotPos = q.PosAxis(localPos);
	return VAdd(colData_.transform_.pos, localRotPos);
}

void Collider::InitColTypeDraw(void)
{
	colTypeChange_.emplace(COLLISION_TYPE::CAPSULE, bind(&Collider::Set_Capsule, this));
	colTypeChange_.emplace(COLLISION_TYPE::SPHERE, bind(&Collider::Set_Sphere, this));
	colTypeChange_.emplace(COLLISION_TYPE::LINE, bind(&Collider::Set_Line, this));
	colTypeChange_.emplace(COLLISION_TYPE::MODEL, bind(&Collider::Set_Model, this));
	colTypeChange_.emplace(COLLISION_TYPE::CYLINDER, bind(&Collider::Set_Cylinder, this));
}

void Collider::SetColTypeDraw(const COLLISION_TYPE colType)
{
	if(!colTypeChange_.contains(colType))
	{
		return;
	}

	colTypeChange_.at(colType)();
}

void Collider::Set_Sphere(void)
{
	colTypeDraw_ = bind(&Collider::Draw_Sphere, this);
}

void Collider::Set_Capsule(void)
{
	colTypeDraw_ = bind(&Collider::Draw_Capsule, this);
}

void Collider::Set_Model(void)
{
	colTypeDraw_ = bind(&Collider::Draw_Model, this);
}

void Collider::Set_Line(void)
{
	colTypeDraw_ = bind(&Collider::Draw_Line, this);
}

void Collider::Set_Cylinder(void)
{
	colTypeDraw_ = bind(&Collider::Draw_Cylinder, this);
}

void Collider::Draw_Sphere(void)
{
	DrawSphere3D(GetRotPos(localPos_), colData_.radius_, 6,
		DEBUG_COLLIDER_COLOR_SPHERE, DEBUG_COLLIDER_COLOR_SPHERE, false);
}

void Collider::Draw_Capsule(void)
{
	DrawCapsule3D(GetPosTop(), GetPosDown(), colData_.radius_, 6,
		DEBUG_COLLIDER_COLOR_CAPSULE, DEBUG_COLLIDER_COLOR_CAPSULE, false);
}

void Collider::Draw_Model(void)
{
	MV1DrawModelDebug(colData_.transform_.modelId_,
		DEBUG_COLLIDER_COLOR_MODEL, false, false, false, true
	);
}

void Collider::Draw_Line(void)
{
	DrawLine3D(GetPosTop(), GetPosDown(), DEBUG_COLLIDER_COLOR_LINE);
}

void Collider::Draw_Cylinder(void)
{
}

