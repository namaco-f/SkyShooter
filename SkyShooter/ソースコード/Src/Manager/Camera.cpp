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

const float SPEED = 2.5f;				//カメラスピード(度)
const float CAMERA_NEAR = 10.0f;		//カメラクリップ：NEAR
const float CAMERA_FAR = 30000.0f;		//カメラクリップ：NEAR
const VECTOR DEFAULT_CAMERA_POS = { 0.0f, 300.0f, -500.0f };		//カメラの初期座標

const VECTOR DEFAULT_FIXEDCAMERA_TARGETPOS = { 0.0f,300.0f,0.0f };	//定点カメラの注視座標

const VECTOR LOCAL_F2C_POS = { 0.0f, 60.0f, -600.0f };				//追従位置からカメラ位置までの相対座標
const VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 500.0f };				//追従位置から注視点までの相対座標

const VECTOR LOCAL_POS_MOUSE_FOLLOW = { 0.0f, 100.0f, 500.0f };
const VECTOR LOCAL_POS_CAMERA_PLAYER_MOUSE_FOLLOW = { 0.0f, 100.0f,200.0f };

const float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);			//カメラのX回転角上限
const float LIMIT_X_DW_RAD = 1.0f * (DX_PI_F / 180.0f);			//カメラのX回転角下限

const Vector2f DEF_CAMERA_ROT_POW = { 10.0f,10.0f };				//デフォルトのカメラの回転速度

const float TIME_MAX_SUCCESS_CAMERA = 18.0f;						//成功時のカメラ演出時間
const float TIME_CHANGE_SUCCESS_CAMERAMOVE_Y = 12.0f;				//Y軸回転に切り替わる時間
const float TIME_CHANGE_SUCCESS_CAMERAMOVE_X = 6.0f;				//X軸回転に切り替わる時間

const Vector2f DEF_CAMERA_DEG = { 0.0f,20.0f };

const int PLAYER_HEAD_FRAMENO = 6;

const float CAMERA_HITDISTANCE = 800.0f;
const float CAMERA_NOTHITDISTANCE = 100.0f;

const Vector2f CAMERA_ROTATE_LIMIT_Y_DEG = { 70.0f,-20.0f };

const VECTOR LOCKON_TARGET_TO_CAMERA_LOCALPOS = { 0.0f,300.0f,-600.0f };	//ターゲットからカメラへの相対座標

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
		//modeUpdate_が使用不可
		return;
	}

	modeUpdate_();

	CameraShake();
}

void Camera::SetBeforeDraw(void)
{

	if (!setBeforeDrawMode_)
	{
		//setBeforeDrawMode_が使用不可
		return;
	}

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	setBeforeDrawMode_();

	// DXライブラリのカメラとEffekseerのカメラを同期する。
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
	//注視点とカメラの座標から逆算
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(const CAMERA_MODE mode)
{
	if (IsCameraModeError(mode))
	{
		return;
	}

	//カメラの初期設定
	SetDefault();

	//1つ前のカメラモードを格納
	preMode_ = mode_;

	//カメラモードの変更
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
	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = MathUtility::VECTOR_ZERO;

	// カメラの上方向
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
		//カメラ移動がロック状態、または追従対象がエラー
		return;
	}

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, MathUtility::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, MathUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = MathUtility::DIR_U;
}

void Camera::SyncFollowFix(void)
{
	if (cameraMoveLock_ || IsFollowTransformError())
	{
		//カメラ移動がロック状態、または追従対象がエラー
		return;
	}

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	//角度変化
	Quaternion gRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);


	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, MathUtility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, MathUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);


	localPos = rot_.PosAxis(LOCAL_F2C_POS);

	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();
}

void Camera::SyncMouse(void)
{
	if (cameraMoveLock_ || IsFollowTransformError())
	{
		//カメラ移動がロック状態、または追従対象がエラー
		return;
	}

	const Vector2 defPos = { Application::SCREEN_SIZE_HALF_X,Application::SCREEN_SIZE_HALF_Y };

	auto dT = TimeManager::GetInstance().GetDeltaTime();

	//角度変化
	Quaternion gRot = defSetRot_;

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	preMousePos_ = nowMousePos_;
	GetMousePoint(&nowMousePos_.x, &nowMousePos_.y);
	
	//x軸角度変化
	radXY_.x += MathUtility::Deg2RadF(
		float(nowMousePos_.x - defPos.x) * cameraRotPow_.x * dT
	);

	if (
		MathUtility::Rad2DegF(radXY_.y) <= CAMERA_ROTATE_LIMIT_Y_DEG.x
		&& MathUtility::Rad2DegF(radXY_.y) >= CAMERA_ROTATE_LIMIT_Y_DEG.y
		)
	{
		//Y軸角度変化
		radXY_.y -= MathUtility::Deg2RadF(
			float(nowMousePos_.y - defPos.y) * cameraRotPow_.y * dT
		);
	}
	else if (MathUtility::Rad2DegF(radXY_.y) < CAMERA_ROTATE_LIMIT_Y_DEG.y)
	{
		//下限以下
		radXY_.y = MathUtility::Deg2RadF(CAMERA_ROTATE_LIMIT_Y_DEG.y);
	}
	else if (MathUtility::Rad2DegF(radXY_.y) > CAMERA_ROTATE_LIMIT_Y_DEG.x)
	{
		//上限以上
		radXY_.y = MathUtility::Deg2RadF(CAMERA_ROTATE_LIMIT_Y_DEG.x);
	}

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(radXY_.x, MathUtility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(radXY_.y, MathUtility::AXIS_X));

	angles_ = Quaternion::ToEuler(rot_);

	VECTOR localPos;
	localPos = rotOutX_.PosAxis(LOCAL_POS_CAMERA_PLAYER_MOUSE_FOLLOW);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_POS_MOUSE_FOLLOW);
	prePos_ = pos_;
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

	SetMousePoint(Application::SCREEN_SIZE_HALF_X, Application::SCREEN_SIZE_HALF_Y);

	ProcessRot();
}

void Camera::SyncTarget(void)
{
	if (IsFollowTransformError())
	{
		//追従対象がエラー
		return;
	}

	//カメラをバグらせないより
	//一定範囲内に近づけさせないほうが早い
	using Math = MathUtility;
	const VECTOR localPos = LOCKON_TARGET_TO_CAMERA_LOCALPOS;

	//追従対象の座標
	VECTOR pos = followTransform_->pos;

	//注視点
	targetPos_ = targetTransform_->pos;

	float len = Math::MagnitudeF(VSub(targetPos_, pos));

	//注視点からの追従対象へのベクトル
	VECTOR targetVec = VNorm(VSub(targetPos_, pos));
	VECTOR targetPVec = VNorm(VSub(pos, targetPos_));

	//カメラ角度反映用
	debugAtan2.y = atan2f(targetVec.x, targetVec.z);
	debugAtan2.z = atan2f(targetPVec.y, targetPVec.x);
	debugAtan2.x = atan2f(targetPVec.y, targetPVec.z);


	//角度変化 
	Quaternion gRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(debugAtan2.y, MathUtility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(debugAtan2.x, MathUtility::AXIS_X));

	//カメラ座標を反映
	pos_ = VAdd(pos, VScale(targetVec, localPos.z));
	pos_.y += localPos.y;

	// カメラの上方向
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

	//右回転
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

#pragma region 前後左右上下

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

#pragma region 角度

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
	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow();

	CollisionStage();
}

void Camera::ModeUpdate_FollowFix(void)
{
	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollowFix();

	//ステージとの判定でカメラ位置を調整
	CollisionStage();
}

void Camera::ModeUpdate_FollowMouse(void)
{
	//カメラ操作
	ProcessRotMouse();

	//追従対象との相対位置を同期
	SyncMouse();

	//カメラ操作
	ProcessRot();

	//追従対象との相対位置を同期
	SyncFollowFix();

	//ステージとの判定でカメラ位置を調整
	CollisionStage();
}

void Camera::ModeUpdate_LockOn(void)
{
	//追従対象との相対位置を同期
	SyncTarget();

	CollisionStage();
}

void Camera::SetBeforeDrawMode_FixedPoint(void)
{
	//何もしない
	//カメラの設定(位置と注視点による制御)
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
	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::SetBeforeDrawMode_FollowFix(void)
{
	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}


void Camera::SetBeforeDrawMode_FollowMouse(void)
{
	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::SetBeforeDrawMode_LockOn(void)
{
	//カメラの設定(位置と注視点による制御)
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
		//追従対象がエラー
		return;
	}

	auto& actorMng = ActorManager::GetInstance();
	const weak_ptr<ActorBase> ground = actorMng.GetActor<ActorBase>(OBJECT_TYPE::STAGE);
	
	if (ground.expired())
	{
		//地面が存在しない
		return;
	}

	VECTOR cameraVec = GetCameraVec();

	//頭の座標を取得
	VECTOR fPos = followTransform_->GetPosFrame(PLAYER_HEAD_FRAMENO);

	Collision col = Collision();
	const auto& colg = col.GetCollResultPoly_Line(ground.lock()->GetCollider(), pos_, fPos);


	if (colg.HitFlag)
	{
		// コリジョン用ポリゴンが存在しない座標までカメラ座標を注視点に近づける

		// 注視点座標からカメラ座標へのベクトルを算出
		VECTOR Direction = VNorm(VSub(pos_, fPos));

		// ポリゴンに当たらない距離をセット
		float NotHitDistance = CAMERA_NOTHITDISTANCE;

		// ポリゴンに当たる距離をセット
		VECTOR TestPosition = pos_;

		float collitionLineStageCamera_ = 0.0f;

		float HitDistance = CAMERA_HITDISTANCE;
		while (HitDistance - NotHitDistance > CAMERA_NOTHITDISTANCE)
		{
			// 当たるかどうかテストする距離を算出( 当たらない距離と当たる距離の中間 )
			collitionLineStageCamera_ = NotHitDistance + (HitDistance - NotHitDistance) * 0.5f;

			// テスト用のカメラ座標を算出
			TestPosition =
				VAdd(fPos, VScale(Direction, collitionLineStageCamera_));

			auto& colgb = col.GetCollResultPoly_Line(ground.lock()->GetCollider(), pos_, fPos);

			// 新しい座標でコリジョン用ポリゴンに当たるかテスト
			if (colgb.HitFlag)
			{
				// 当たる距離を collitionLineStageCamera_ に変更する
				HitDistance = collitionLineStageCamera_;
			}
			else
			{
				// 当たらなかったら当たらない距離を collitionLineStageCamera_ に変更する
				NotHitDistance = collitionLineStageCamera_;
			}

			// HitDistance と NoHitDistance が十分に近づいていなかったらループ
		}

		// カメラの座標をセット
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
	//定点
	modeChanges_.emplace(CAMERA_MODE::FIXED_POINT, bind(&Camera::ModeChange_FixedPoint, this));

	//自由
	modeChanges_.emplace(CAMERA_MODE::FREE, bind(&Camera::ModeChange_Free, this));

	//追従自由
	modeChanges_.emplace(CAMERA_MODE::FOLLOW, bind(&Camera::ModeChange_Follow, this));

	//追従定点
	modeChanges_.emplace(CAMERA_MODE::FOLLOW_FIX, bind(&Camera::ModeChange_FollowFix, this));

	//追従自由(マウス)
	modeChanges_.emplace(CAMERA_MODE::FOLLOW_MOUSE, bind(&Camera::ModeChange_FollowMouse, this));

	//ロックオン
	modeChanges_.emplace(CAMERA_MODE::LOCKON, bind(&Camera::ModeChange_LockOn, this));
}

