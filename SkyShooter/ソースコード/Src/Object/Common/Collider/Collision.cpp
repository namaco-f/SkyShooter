
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
		//衝突判定の結果の後始末
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
		//カプセル同士かカプセルと球か
		return colBCap ? IsHitCapsulesResult(colA, colB) : IsHitSphereCapsuleResult(colA, colB);
	}
	else
	{
		//カプセルと球か球同士
		return colBCap ? IsHitSphereCapsuleResult(colA, colB) : IsHitSpheresResult(colA, colB);
	}

	//入力されている値がエラー
	return nullHitColInfo_;
}

const MV1_COLL_RESULT_POLY Collision::GetCollResultPoly_Line(
	const Collider& colA, const Collider& colB)
{
	//片方が線分型でないなら、もう片方が線分型
	return IsLine(colA) ? 
		CollisionLine(colA, colB) : CollisionLine(colB, colA);
}

const Collision::HitColInfo Collision::IsHitSphereCapsuleResult(const Collider& colA, const Collider& colB)
{
	if (!(AbleToGetHitColInfo(colA) && AbleToGetHitColInfo(colB)))
	{
		//どちらかが球またはカプセルではない
		return nullHitColInfo_;
	}

	//片方が球でないなら、もう片方が球
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
	// 検出した地面ポリゴン情報の後始末
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
	//衝突情報格納用
	HitColInfo hitCol;

	bool ret = false;

	VECTOR hitDir = {};

	// 球とカプセルの衝突判定
	if (Math::IsHitSphereCapsule(
		colSphere.GetPos(),
		colSphere.GetRadius(),
		colCapsule.GetPosTop(),
		colCapsule.GetPosDown(),
		colCapsule.GetRadius()
	))
	{
		//衝突判定している

		//カプセルの中心座標から球の中心座標へのベクトル
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





