
#include <cassert>
#include <algorithm>
#include "Collider.h"
#include "../../../Utility/MathUtility.h"
#include "../Object/Actor/Stage/StageSize.h"
#include "Collision.h"

using namespace std;

using Math = MathUtility;

Collision::Collision(void)
	:
	resultPolyDim_{},
	isStoreResult_(false)
{
}

Collision::~Collision(void)
{
	if (isStoreResult_)
	{
		//�Փ˔���̌��ʂ̌�n��
		MV1CollResultPolyDimTerminate(resultPolyDim_);
		isStoreResult_ = false;
	}
}

const Collision::HitColInfo Collision::GetColResultHitColInfo(const Collider& colA, const Collider& colB)
{

	bool colACap = IsCapsule(colA);
	bool colASph = IsSphere(colA);
	bool colBCap = IsCapsule(colB);

	if (colACap)
	{
		//�J�v�Z�����m���J�v�Z���Ƌ���
		return colBCap ? IsHitCapsulesResult(colA, colB) : IsHitSphereCapsuleResult(colA, colB);
	}
	else
	{
		//�J�v�Z���Ƌ��������m
		return colBCap ? IsHitSphereCapsuleResult(colA, colB) : IsHitSpheresResult(colA, colB);
	}

	//���͂���Ă���l���G���[
	return nullHitColInfo_;
}

const MV1_COLL_RESULT_POLY Collision::GetCollResultPoly_Line(
	const Collider& colA, const Collider& colB)
{
	//�Е��������^�łȂ��Ȃ�A�����Е��������^
	return IsLine(colA) ? 
		CollisionLine(colA, colB) : CollisionLine(colB, colA);
}

const Collision::HitColInfo Collision::IsHitSphereCapsuleResult(const Collider& colA, const Collider& colB)
{
	if (!(AbleToGetHitColInfo(colA) && AbleToGetHitColInfo(colB)))
	{
		//�ǂ��炩�����܂��̓J�v�Z���ł͂Ȃ�
		return nullHitColInfo_;
	}

	//�Е������łȂ��Ȃ�A�����Е�����
	return IsSphere(colA) ? 
		CollisionSphereCapsule(colA, colB) : CollisionSphereCapsule(colB, colA);
}

const Collision::HitColInfo Collision::IsHitSpheresResult(const Collider& colA, const Collider& colB)
{
	if (!(IsSphere(colA) && IsSphere(colB)))
	{
		return nullHitColInfo_;
	}

	return CollisionSpheres(colA, colB);
}

const Collision::HitColInfo Collision::IsHitCapsulesResult(const Collider& colA, const Collider& colB)
{
	if (!(IsCapsule(colA) && IsCapsule(colB)))
	{
		return nullHitColInfo_;
	}

	return CollisionCapsules(colA, colB);
}

void Collision::CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	// ���o�����n�ʃ|���S�����̌�n��
	MV1CollResultPolyDimTerminate(hitPoly);
}

const MV1_COLL_RESULT_POLY Collision::GetCollResultPoly_Line(const Collider& colA, const VECTOR& lineTop, const VECTOR& lineBottom)
{
	return MV1CollCheck_Line(
		colA.GetModelId(), colA.GetFrameIdx(),
		lineTop, lineBottom
	);
}

void Collision::IsCollisionMoveLimit(VECTOR& movedPos)
{
	CollisionMoveLimit(movedPos.x, -STAGE_MOVELIMIT_X, STAGE_MOVELIMIT_X);
	CollisionMoveLimit(movedPos.y, -STAGE_MOVELIMIT_Y, STAGE_MOVELIMIT_Y);
	CollisionMoveLimit(movedPos.z, -STAGE_MOVELIMIT_Z, STAGE_MOVELIMIT_Z);
}

const Collision::HitColInfo Collision::CollisionSphereCapsule(
	const Collider& colSphere, const Collider& colCapsule)
{
	//�Փˏ��i�[�p
	HitColInfo hitCol;

	bool ret = false;

	VECTOR hitDir = {};

	// ���ƃJ�v�Z���̏Փ˔���
	if (Math::IsHitSphereCapsule(
		colSphere.GetPos(),
		colSphere.GetRadius(),
		colCapsule.GetPosTop(),
		colCapsule.GetPosDown(),
		colCapsule.GetRadius()
	))
	{
		//�Փ˔��肵�Ă���

		//�J�v�Z���̒��S���W���狅�̒��S���W�ւ̃x�N�g��
		hitDir = VSub(
			colSphere.GetPos(), colCapsule.GetPos());
		hitDir = VNorm(hitDir);
		ret = true;
	}

	hitCol.hitDir = hitDir;
	hitCol.isHit = ret;

	return hitCol;
}

const Collision::HitColInfo Collision::CollisionCapsules(const Collider& colCapsuleA, const Collider& colCapsuleB)
{
	HitColInfo hitCol;
	bool ret = false;

	if (
		Math::IsHitCapsuleCapsule(
			colCapsuleA.GetPosTop(), colCapsuleA.GetPosDown(), colCapsuleA.GetRadius(),
			colCapsuleB.GetPosTop(), colCapsuleB.GetPosDown(), colCapsuleB.GetRadius())
		)
	{
		ret = true;
	}

	hitCol.isHit = ret;

	return hitCol;
}

const Collision::HitColInfo Collision::CollisionSpheres(const Collider& colSphereA, const Collider& colSphereB)
{
	HitColInfo hitCol;

	bool ret = false;

	//VECTOR hitpos;
	VECTOR hitDir = {};

	if (
		Math::IsHitSpheres(
			colSphereA.GetRotPos(colSphereA.GetLocalPos()), 
			colSphereA.GetRadius(),
			colSphereB.GetRotPos(colSphereB.GetLocalPos()), 
			colSphereB.GetRadius()
			)
		)
	{
		hitDir = VSub(
			colSphereA.GetTrasformPos(), colSphereB.GetTrasformPos());
		hitDir = VNorm(hitDir);
		ret = true;
	}
	else
	{
		ret = false;
	}


	hitCol.hitDir = hitDir;
	hitCol.isHit = ret;

	return hitCol;
}

const MV1_COLL_RESULT_POLY Collision::CollisionLine(
	const Collider& colLine, const Collider& colModel
)const
{
	return MV1CollCheck_Line(
		colModel.GetModelId(), colModel.GetFrameIdx(),
		colLine.GetPosTop(), colLine.GetPosDown()
	);
}

void Collision::CollisionMoveLimit(float& pos, const float minPos, const float maxPos)
{
	pos = std::clamp<float>(pos, minPos, maxPos);
}

const bool Collision::IsCapsule(const Collider& col) const
{
	return col.GetColType() == COLLISION_TYPE::CAPSULE;
}

const bool Collision::IsModel(Collider& col) const
{
	return col.GetColType() == COLLISION_TYPE::MODEL;
}

const bool Collision::IsSphere(const Collider& col) const
{
	return col.GetColType() == COLLISION_TYPE::SPHERE;
}

const bool Collision::IsLine(const Collider& col) const
{
	return col.GetColType() == COLLISION_TYPE::LINE;
}

const bool Collision::AbleToGetHitColInfo(const Collider& col) const
{
	return IsSphere(col) || IsCapsule(col);
}





