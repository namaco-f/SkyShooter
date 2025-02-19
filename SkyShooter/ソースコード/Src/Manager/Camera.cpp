#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Input/InputManager.h"
#include "ActorsManager/Actor/ActorManager.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Common/Collider/Collision.h"
#include "Input/InputController.h"
#include "../Utility/MathUtility.h"
#include "../Utility/DebugUtility.h"
//#include "../Manager/GravityManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Common/Collider/Collider.h"
#include "Camera.h"

using namespace std;

const float ROTPOW_DEG = 2.0f;
const float MOVESPEED = 2.0f;
const VECTOR INIT_ROTATE_DEG = { 30.0f,0.0f,0.0f };

const float CAMERASHAKE_STEP_MIN = 0.0f;

const float SPEED = 2.5f;				//�J�����X�s�[�h(�x)
const float CAMERA_NEAR = 10.0f;		//�J�����N���b�v�FNEAR
const float CAMERA_FAR = 30000.0f;		//�J�����N���b�v�FNEAR
const VECTOR DEFAULT_CAMERA_POS = { 0.0f, 300.0f, -500.0f };		//�J�����̏������W

const VECTOR DEFAULT_FIXEDCAMERA_TARGETPOS = { 0.0f,300.0f,0.0f };	//��_�J�����̒������W

const VECTOR LOCAL_F2C_POS = { 0.0f, 60.0f, -600.0f };				//�Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W
const VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 500.0f };				//�Ǐ]�ʒu���璍���_�܂ł̑��΍��W

const VECTOR LOCAL_POS_MOUSE_FOLLOW = { 0.0f, 100.0f, 500.0f };
const VECTOR LOCAL_POS_CAMERA_PLAYER_MOUSE_FOLLOW = { 0.0f, 100.0f,200.0f };

const float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);			//�J������X��]�p���
const float LIMIT_X_DW_RAD = 1.0f * (DX_PI_F / 180.0f);			//�J������X��]�p����

const Vector2f DEF_CAMERA_ROT_POW = { 10.0f,10.0f };				//�f�t�H���g�̃J�����̉�]���x

const float TIME_MAX_SUCCESS_CAMERA = 18.0f;						//�������̃J�������o����
const float TIME_CHANGE_SUCCESS_CAMERAMOVE_Y = 12.0f;				//Y����]�ɐ؂�ւ�鎞��
const float TIME_CHANGE_SUCCESS_CAMERAMOVE_X = 6.0f;				//X����]�ɐ؂�ւ�鎞��

const Vector2f DEF_CAMERA_DEG = { 0.0f,20.0f };

const int PLAYER_HEAD_FRAMENO = 6;

const float CAMERA_HITDISTANCE = 800.0f;
const float CAMERA_NOTHITDISTANCE = 100.0f;

const Vector2f CAMERA_ROTATE_LIMIT_Y_DEG = { 70.0f,-20.0f };

const VECTOR LOCKON_TARGET_TO_CAMERA_LOCALPOS = { 0.0f,300.0f,-600.0f };	//�^�[�Q�b�g����J�����ւ̑��΍��W

const string DEBUGDRAW_ATAN2 = "x:%.2f,y:%.2f,z:%.2f";

Camera::Camera(void)
	:
	inputController_(make_unique<InputController>()),
	lineCollider_(nullptr),
	isShake_(false),
	shakeStep_(0.0f),
	shakePow_(0.0f),
	timeScale_(0.0f),
	followTransform_(nullptr),
	targetTransform_(nullptr),
	mode_(CAMERA_MODE::NONE),
	preMode_(CAMERA_MODE::NONE),
	pos_{},
	prePos_{},
	angles_{},
	rotOutX_(Quaternion()),
	rot_(Quaternion()),
	targetPos_{},
	cameraUp_{},
	preMousePos_{},
	nowMousePos_{},
	radXY_{},
	cameraRotPow_(DEF_CAMERA_ROT_POW),
	defSetRot_(Quaternion()),
	debugAtan2{},
	cameraMoveLock_(false),
	modeChanges_{},
	modeUpdate_{},
	setBeforeDrawMode_{}
{
	InitModeChange();

	ChangeMode(CAMERA_MODE::FIXED_POINT);
}

Camera::~Camera(void)
{
	modeChanges_.clear();
}

void Camera::Update(void)
{
	if (!modeUpdate_)
	{
		//modeUpdate_���g�p�s��
		return;
	}

	modeUpdate_();

	CameraShake();
}

void Camera::SetBeforeDraw(void)
{

	if (!setBeforeDrawMode_)
	{
		//setBeforeDrawMode_���g�p�s��
		return;
	}

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	setBeforeDrawMode_();

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
	//defSetRot_ = followTransform_->quaRot;
}

void Camera::SetTarget(const Transform* target)
{
	targetTransform_ = target;
}

void Camera::SetCameraRotPow(const Vector2f& pow)
{
	cameraRotPow_ = pow;
}

void Camera::SetCameraShake(float time, float shakePow,float timeScale)
{
	if (isShake_) return;

	isShake_ = true;
	shakeStep_ = time;
	shakePow_ = shakePow;
	timeScale_ = timeScale;
}

void Camera::CameraShake(void)
{
	if (!isShake_) return;

	Vector2f shakePos = { pos_.x,pos_.y };
	
	Vector2 pow = Vector2(
		(int)(shakeStep_ * timeScale_) % 3 - 1,
		(int)(shakeStep_ * timeScale_) % 3 - 1
	);

	Vector2f shakeMove = ToVector2f(pow) * shakePow_;
	shakePos += shakeMove;
	
	pos_.x = shakePos.x;
	pos_.y = shakePos.y;

	shakeStep_ -= TimeManager::GetInstance().GetDeltaTime();
	if (shakeStep_ < CAMERASHAKE_STEP_MIN)
	{
		isShake_ = false;
	}
}

void Camera::CameraMoveLock(const bool flag)
{
	cameraMoveLock_ = flag;
	SetMousePoint(Application::SCREEN_SIZE_HALF_X, Application::SCREEN_SIZE_HALF_Y);
}

//void Camera::SetTargetPos(const VECTOR& tPos)
//{
//	targetPos_ = tPos;
//}

void Camera::SetDefQuaRot(const Quaternion& qua)
{
	defSetRot_ = qua;
	radXY_ = Vector2f();
	radXY_.y = MathUtility::Deg2RadF(DEF_CAMERA_DEG.y);
	//angles_.x = radXY_.y;
	//angles_.y = radXY_.x;
}

const VECTOR Camera::GetForward(void) const
{
	//�����_�ƃJ�����̍��W����t�Z
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(const CAMERA_MODE mode)
{
	if (IsCameraModeError(mode))
	{
		return;
	}

	//�J�����̏����ݒ�
	SetDefault();

	//1�O�̃J�������[�h���i�[
	preMode_ = mode_;

	//�J�������[�h�̕ύX
	mode_ = mode;


	modeChanges_[mode_]();

}

void Camera::InitCollider(void)
{
	Collider::ColliderData colData;


	lineCollider_ = make_unique<Collider>();
}

void Camera::SetDefault(void)
{
	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = MathUtility::VECTOR_ZERO;

	// �J�����̏����
	cameraUp_ = MathUtility::DIR_U;

	angles_.x = MathUtility::Deg2RadF(INIT_ROTATE_DEG.x);
	angles_.y = MathUtility::Deg2RadF(INIT_ROTATE_DEG.y);
	angles_.z = MathUtility::Deg2RadF(INIT_ROTATE_DEG.z);

	rot_ = Quaternion();
}

void Camera::SyncFollow(void)
{
	if (cameraMoveLock_ || IsFollowTransformError())
	{
		//�J�����ړ������b�N��ԁA�܂��͒Ǐ]�Ώۂ��G���[
		return;
	}

	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = Quaternion::AngleAxis(angles_.y, MathUtility::AXIS_Y);

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, MathUtility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = MathUtility::DIR_U;
}

void Camera::SyncFollowFix(void)
{
	if (cameraMoveLock_ || IsFollowTransformError())
	{
		//�J�����ړ������b�N��ԁA�܂��͒Ǐ]�Ώۂ��G���[
		return;
	}

	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	//�p�x�ω�
	Quaternion gRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);


	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, MathUtility::AXIS_Y));

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, MathUtility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);


	localPos = rot_.PosAxis(LOCAL_F2C_POS);

	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = gRot.GetUp();
}

void Camera::SyncMouse(void)
{
	if (cameraMoveLock_ || IsFollowTransformError())
	{
		//�J�����ړ������b�N��ԁA�܂��͒Ǐ]�Ώۂ��G���[
		return;
	}

	const Vector2 defPos = { Application::SCREEN_SIZE_HALF_X,Application::SCREEN_SIZE_HALF_Y };

	auto dT = TimeManager::GetInstance().GetDeltaTime();

	//�p�x�ω�
	Quaternion gRot = defSetRot_;

	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	preMousePos_ = nowMousePos_;
	GetMousePoint(&nowMousePos_.x, &nowMousePos_.y);
	
	//x���p�x�ω�
	radXY_.x += MathUtility::Deg2RadF(
		float(nowMousePos_.x - defPos.x) * cameraRotPow_.x * dT
	);

	if (
		MathUtility::Rad2DegF(radXY_.y) <= CAMERA_ROTATE_LIMIT_Y_DEG.x
		&& MathUtility::Rad2DegF(radXY_.y) >= CAMERA_ROTATE_LIMIT_Y_DEG.y
		)
	{
		//Y���p�x�ω�
		radXY_.y -= MathUtility::Deg2RadF(
			float(nowMousePos_.y - defPos.y) * cameraRotPow_.y * dT
		);
	}
	else if (MathUtility::Rad2DegF(radXY_.y) < CAMERA_ROTATE_LIMIT_Y_DEG.y)
	{
		//�����ȉ�
		radXY_.y = MathUtility::Deg2RadF(CAMERA_ROTATE_LIMIT_Y_DEG.y);
	}
	else if (MathUtility::Rad2DegF(radXY_.y) > CAMERA_ROTATE_LIMIT_Y_DEG.x)
	{
		//����ȏ�
		radXY_.y = MathUtility::Deg2RadF(CAMERA_ROTATE_LIMIT_Y_DEG.x);
	}

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(radXY_.x, MathUtility::AXIS_Y));

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(radXY_.y, MathUtility::AXIS_X));

	angles_ = Quaternion::ToEuler(rot_);

	VECTOR localPos;
	localPos = rotOutX_.PosAxis(LOCAL_POS_CAMERA_PLAYER_MOUSE_FOLLOW);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = rot_.PosAxis(LOCAL_POS_MOUSE_FOLLOW);
	prePos_ = pos_;
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = gRot.GetUp();

	SetMousePoint(Application::SCREEN_SIZE_HALF_X, Application::SCREEN_SIZE_HALF_Y);

	ProcessRot();
}

void Camera::SyncTarget(void)
{
	if (IsFollowTransformError())
	{
		//�Ǐ]�Ώۂ��G���[
		return;
	}

	//�J�������o�O�点�Ȃ����
	//���͈͓��ɋ߂Â������Ȃ��ق�������
	using Math = MathUtility;
	const VECTOR localPos = LOCKON_TARGET_TO_CAMERA_LOCALPOS;

	//�Ǐ]�Ώۂ̍��W
	VECTOR pos = followTransform_->pos;

	//�����_
	targetPos_ = targetTransform_->pos;

	float len = Math::MagnitudeF(VSub(targetPos_, pos));

	//�����_����̒Ǐ]�Ώۂւ̃x�N�g��
	VECTOR targetVec = VNorm(VSub(targetPos_, pos));
	VECTOR targetPVec = VNorm(VSub(pos, targetPos_));

	//�J�����p�x���f�p
	debugAtan2.y = atan2f(targetVec.x, targetVec.z);
	debugAtan2.z = atan2f(targetPVec.y, targetPVec.x);
	debugAtan2.x = atan2f(targetPVec.y, targetPVec.z);


	//�p�x�ω� 
	Quaternion gRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(debugAtan2.y, MathUtility::AXIS_Y));

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(debugAtan2.x, MathUtility::AXIS_X));

	//�J�������W�𔽉f
	pos_ = VAdd(pos, VScale(targetVec, localPos.z));
	pos_.y += localPos.y;

	// �J�����̏����
	cameraUp_ = gRot.GetUp();
}

void Camera::ProcessRot(void)
{
	if (cameraMoveLock_)
	{
		return;
	}

	auto& ins = InputManager::GetInstance();

	float rotPow = SPEED;

	//�E��]
	if (inputController_->IsPressed(ACTION::CAMERA_RIGHT))
	{
		angles_.y += MathUtility::Deg2RadF(rotPow);
	}
	if (inputController_->IsPressed(ACTION::CAMERA_LEFT))
	{
		angles_.y += MathUtility::Deg2RadF(-rotPow);
	}


	if (angles_.x <= LIMIT_X_UP_RAD)
	{
		if (inputController_->IsPressed(ACTION::CAMERA_UP))
		{
			angles_.x += MathUtility::Deg2RadF(rotPow);
		}
	}

	if (angles_.x >= LIMIT_X_DW_RAD)
	{
		if (inputController_->IsPressed(ACTION::CAMERA_DOWN))
		{
			angles_.x += MathUtility::Deg2RadF(-rotPow);
		}
	}
}

void Camera::ProcessRotMouse(void)
{
}


void Camera::ModeChange_FixedPoint(void)
{
	targetPos_ = DEFAULT_FIXEDCAMERA_TARGETPOS;
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawMode_FixedPoint, this);
	modeUpdate_ = bind(&Camera::ModeUpdate_FixedPoint, this);
}

void Camera::ModeChange_Free(void)
{
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawMode_Free, this);
	modeUpdate_ = bind(&Camera::ModeUpdate_Free, this);
}

void Camera::ModeChange_Follow(void)
{
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawMode_Follow, this);
	modeUpdate_ = bind(&Camera::ModeUpdate_Follow , this);
}

void Camera::ModeChange_FollowFix(void)
{
	modeUpdate_ = bind(&Camera::ModeUpdate_FollowFix, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawMode_FollowFix, this);
}

void Camera::ModeChange_FollowMouse(void)
{
	modeUpdate_ = bind(&Camera::ModeUpdate_FollowMouse, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawMode_FollowMouse, this);
	radXY_.y = MathUtility::Deg2RadF(DEF_CAMERA_DEG.y);
	
	SetMousePoint(Application::SCREEN_SIZE_HALF_X, Application::SCREEN_SIZE_HALF_Y);
}

void Camera::ModeChange_LockOn(void)
{
	modeUpdate_ = bind(&Camera::ModeUpdate_LockOn, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawMode_LockOn, this);
}

void Camera::ModeUpdate_FixedPoint(void)
{	
}

void Camera::ModeUpdate_Free(void)
{
	VECTOR dir = MathUtility::VECTOR_ZERO;

#pragma region �O�㍶�E�㉺

	if (inputController_->IsPressed(ACTION::MOVE_FORWARD))
	{
		dir = MathUtility::DIR_F;
	}
	if (inputController_->IsPressed(ACTION::MOVE_BACK))
	{
		dir = MathUtility::DIR_B;
	}
	if (inputController_->IsPressed(ACTION::MOVE_LEFT))
	{
		dir = MathUtility::DIR_L;
	}
	if (inputController_->IsPressed(ACTION::MOVE_RIGHT))
	{
		dir = MathUtility::DIR_R;
	}

#pragma endregion

#pragma region �p�x

	float rotPow = MathUtility::Deg2RadF(ROTPOW_DEG);
	if (inputController_->IsPressed(ACTION::CAMERA_UP))
	{
		angles_.x -= rotPow;
	}
	if (inputController_->IsPressed(ACTION::CAMERA_DOWN))
	{
		angles_.x += rotPow;
	}
	if (inputController_->IsPressed(ACTION::CAMERA_LEFT))
	{
		angles_.y -= rotPow;
	}
	if (inputController_->IsPressed(ACTION::CAMERA_RIGHT))
	{
		angles_.y += rotPow;
	}

#pragma endregion

	Quaternion qua = Quaternion::Euler(angles_);

	VECTOR moveDir = qua.PosAxis(dir);

	pos_ = VAdd(pos_, VScale(moveDir, MOVESPEED));
}

void Camera::ModeUpdate_Follow(void)
{
	// �J��������
	ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollow();

	CollisionStage();
}

void Camera::ModeUpdate_FollowFix(void)
{
	// �J��������
	ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollowFix();

	//�X�e�[�W�Ƃ̔���ŃJ�����ʒu�𒲐�
	CollisionStage();
}

void Camera::ModeUpdate_FollowMouse(void)
{
	//�J��������
	ProcessRotMouse();

	//�Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncMouse();

	//�J��������
	ProcessRot();

	//�Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollowFix();

	//�X�e�[�W�Ƃ̔���ŃJ�����ʒu�𒲐�
	CollisionStage();
}

void Camera::ModeUpdate_LockOn(void)
{
	//�Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncTarget();

	CollisionStage();
}

void Camera::SetBeforeDrawMode_FixedPoint(void)
{
	//�������Ȃ�
	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::SetBeforeDrawMode_Free(void)
{
	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);
}

void Camera::SetBeforeDrawMode_Follow(void)
{
	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::SetBeforeDrawMode_FollowFix(void)
{
	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}


void Camera::SetBeforeDrawMode_FollowMouse(void)
{
	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::SetBeforeDrawMode_LockOn(void)
{
	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::DrawDebug(void)
{
	auto& debug = SceneManager::GetInstance().GetDebugUtility();

	VECTOR deg = {
		MathUtility::Rad2DegF(debugAtan2.x),
		MathUtility::Rad2DegF(debugAtan2.y),
		MathUtility::Rad2DegF(debugAtan2.z) 
	};
	debug.DebugStringFormat(0, DEBUGDRAW_ATAN2.c_str(), 0xffffff, deg);
}

const bool Camera::IsCameraModeError(const CAMERA_MODE mode) const
{
	return mode == CAMERA_MODE::NONE || mode == CAMERA_MODE::MAX;
}

void Camera::CollisionStage(void)
{
	if (IsFollowTransformError())
	{
		//�Ǐ]�Ώۂ��G���[
		return;
	}

	auto& actorMng = ActorManager::GetInstance();
	const weak_ptr<ActorBase> ground = actorMng.GetActor<ActorBase>(OBJECT_TYPE::STAGE);
	
	if (ground.expired())
	{
		//�n�ʂ����݂��Ȃ�
		return;
	}

	VECTOR cameraVec = GetCameraVec();

	//���̍��W���擾
	VECTOR fPos = followTransform_->GetPosFrame(PLAYER_HEAD_FRAMENO);

	Collision col = Collision();
	const auto& colg = col.GetCollResultPoly_Line(ground.lock()->GetCollider(), pos_, fPos);


	if (colg.HitFlag)
	{
		// �R���W�����p�|���S�������݂��Ȃ����W�܂ŃJ�������W�𒍎��_�ɋ߂Â���

		// �����_���W����J�������W�ւ̃x�N�g�����Z�o
		VECTOR Direction = VNorm(VSub(pos_, fPos));

		// �|���S���ɓ�����Ȃ��������Z�b�g
		float NotHitDistance = CAMERA_NOTHITDISTANCE;

		// �|���S���ɓ����鋗�����Z�b�g
		VECTOR TestPosition = pos_;

		float collitionLineStageCamera_ = 0.0f;

		float HitDistance = CAMERA_HITDISTANCE;
		while (HitDistance - NotHitDistance > CAMERA_NOTHITDISTANCE)
		{
			// �����邩�ǂ����e�X�g���鋗�����Z�o( ������Ȃ������Ɠ����鋗���̒��� )
			collitionLineStageCamera_ = NotHitDistance + (HitDistance - NotHitDistance) * 0.5f;

			// �e�X�g�p�̃J�������W���Z�o
			TestPosition =
				VAdd(fPos, VScale(Direction, collitionLineStageCamera_));

			auto& colgb = col.GetCollResultPoly_Line(ground.lock()->GetCollider(), pos_, fPos);

			// �V�������W�ŃR���W�����p�|���S���ɓ����邩�e�X�g
			if (colgb.HitFlag)
			{
				// �����鋗���� collitionLineStageCamera_ �ɕύX����
				HitDistance = collitionLineStageCamera_;
			}
			else
			{
				// ������Ȃ������瓖����Ȃ������� collitionLineStageCamera_ �ɕύX����
				NotHitDistance = collitionLineStageCamera_;
			}

			// HitDistance �� NoHitDistance ���\���ɋ߂Â��Ă��Ȃ������烋�[�v
		}

		// �J�����̍��W���Z�b�g
		pos_ = TestPosition;
	}
}

const VECTOR Camera::GetCameraVec(void)
{
	if (IsFollowTransformError())
	{
		return pos_;
	}

	return VSub(pos_,followTransform_->pos);
}

const bool Camera::IsFollowTransformError(void) const
{
	return followTransform_ == nullptr;
}

void Camera::InitModeChange(void)
{
	//��_
	modeChanges_.emplace(CAMERA_MODE::FIXED_POINT, bind(&Camera::ModeChange_FixedPoint, this));

	//���R
	modeChanges_.emplace(CAMERA_MODE::FREE, bind(&Camera::ModeChange_Free, this));

	//�Ǐ]���R
	modeChanges_.emplace(CAMERA_MODE::FOLLOW, bind(&Camera::ModeChange_Follow, this));

	//�Ǐ]��_
	modeChanges_.emplace(CAMERA_MODE::FOLLOW_FIX, bind(&Camera::ModeChange_FollowFix, this));

	//�Ǐ]���R(�}�E�X)
	modeChanges_.emplace(CAMERA_MODE::FOLLOW_MOUSE, bind(&Camera::ModeChange_FollowMouse, this));

	//���b�N�I��
	modeChanges_.emplace(CAMERA_MODE::LOCKON, bind(&Camera::ModeChange_LockOn, this));
}

