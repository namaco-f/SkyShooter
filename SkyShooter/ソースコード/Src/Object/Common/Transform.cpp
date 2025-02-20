#include <DxLib.h>
#include "../../Utility/MathUtility.h"
#include "Transform.h"


//const float DEBUG_DIR_LEN = 300.0f;

Transform::Transform(void)
	:
	modelId_(-1),
	scl(MathUtility::VECTOR_ONE),
	rot(MathUtility::VECTOR_ZERO),
	pos(MathUtility::VECTOR_ZERO),
	localPos(MathUtility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())
{
}

Transform::Transform(const int model)
	:
	modelId_(model),
	scl(MathUtility::VECTOR_ONE),
	rot(MathUtility::VECTOR_ZERO),
	pos(MathUtility::VECTOR_ZERO),
	localPos(MathUtility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())
{
}

Transform::~Transform(void)
{
	//MV1DeleteModel(modelId_);
}

void Transform::Update(void)
{
	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// 位置
	matPos = MGetTranslate(pos);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// 行列をモデルに判定
	if (modelId_ != -1)
	{
		MV1SetMatrix(modelId_, mat);
	}
}

void Transform::SetModel(const int model)
{
	modelId_ = model;
}

VECTOR Transform::GetForward(void) const
{
	return GetDir(MathUtility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(MathUtility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(MathUtility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(MathUtility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(MathUtility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(MathUtility::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& vec) const
{
	return quaRot.PosAxis(vec);
}

const VECTOR Transform::GetPosFrame(const int& frameNo) const
{
	if (modelId_ == -1)
	{
		//モデルが登録されていない
		return VECTOR();
	}

	return MV1GetFramePosition(modelId_, frameNo);
}

