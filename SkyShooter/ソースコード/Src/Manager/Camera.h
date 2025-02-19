#pragma once

#include <map>
#include <unordered_map>
#include <functional>
#include "../Common/Quaternion.h"
#include "../Common/Geometry2D.h"

class Transform;
class InputController;
class Collider;

/// <summary>
/// カメラ制御
/// </summary>
class Camera
{
public:

	/// <summary>
	/// カメラモード
	/// </summary>
	enum class CAMERA_MODE
	{
		NONE,			//初期化用
		FIXED_POINT,	//定点
		FREE,			//自由視点
		FOLLOW,			//追従
		FOLLOW_FIX,		//追従定点
		FOLLOW_MOUSE,	//追従マウス
		LOCKON,			//ロックオン
		MAX,			//最大数カウント用
	};

	Camera(void);
	virtual ~Camera(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画前のカメラ設定
	/// </summary>
	void SetBeforeDraw(void);

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// カメラ位置の取得
	/// </summary>
	/// <returns>pos</returns>
	inline const VECTOR& GetPos(void) const
	{
		return pos_;
	}

	/// <summary>
	/// カメラの操作角度
	/// </summary>
	/// <returns>angles_</returns>
	inline const VECTOR& GetAngles(void) const
	{
		return angles_;
	}

	/// <summary>
	/// カメラの初期角度を設定
	/// </summary>
	/// <param name="qua">Quaternion</param>
	void SetDefQuaRot(const Quaternion& qua);
	
	/// <summary>
	/// カメラの注視点の取得
	/// </summary>
	/// <returns>targetPos_</returns>
	inline const VECTOR& GetTargetPos(void) const
	{
		return targetPos_;
	}
	
	/// <summary>
	/// カメラ角度の取得
	/// </summary>
	/// <returns>rot</returns>
	inline const Quaternion& GetQuaRot(void) const
	{
		return rot_;
	}

	/// <summary>
	/// X回転を抜いたカメラ角度
	/// </summary>
	/// <returns>rotOutX_</returns>
	inline const Quaternion& GetQuaRotOutX(void) const
	{
		return rotOutX_;
	}

	/// <summary>
	/// カメラの前方方向の取得
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR GetForward(void) const;

	/// <summary>
	/// カメラモードの変更
	/// </summary>
	/// <param name="mode">CAMERA_MODE : 変更するカメラモード</param>
	void ChangeMode(const CAMERA_MODE mode);

	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="follow">追従対象のTransform</param>
	void SetFollow(const Transform* follow);

	/// <summary>
	/// 注視点を設定
	/// </summary>
	/// <param name="target">注視対象のTransform</param>
	void SetTarget(const Transform* target);
	
	
	/// <summary>
	/// カメラの回転力をセット
	/// </summary>
	/// <param name="pow">縦横方向のカメラの回転力</param>
	void SetCameraRotPow(const Vector2f& pow);

	/// <summary>
	/// カメラシェイクの設定
	/// </summary>
	/// <param name="time">時間</param>
	/// <param name="shakePow">移動量</param>
	/// <param name="timeScale">時間の加減速</param>
	void SetCameraShake(float time,float shakePow,float timeScale = 10.0f);

	/// <summary>
	/// カメラシェイクを実行
	/// </summary>
	void CameraShake(void);

	/// <summary>
	/// ロックオンカメラモードか判定
	/// </summary>
	inline const bool IsLockOnCamera(void) const
	{
		return mode_ == CAMERA_MODE::LOCKON;
	}

	inline const CAMERA_MODE GetPreCameraMode(void) const
	{
		return preMode_;
	}

	/// <summary>
	///	カメラ回転をロックするか設定
	/// </summary>
	void CameraMoveLock(const bool flag);

private:

	//入力制御
	std::unique_ptr<InputController> inputController_;

	//注視点からカメラまでのコライダ
	std::unique_ptr<Collider> lineCollider_;

	//カメラシェイク中か
	bool isShake_;
	
	//カメラシェイクカウント用
	float shakeStep_;

	//カメラシェイクの移動量
	float shakePow_;

	//カメラシェイクの時間の加減速
	float timeScale_;

	//カメラが追従対象とするTransform
	const Transform* followTransform_;

	//カメラの注視点となるTransform
	const Transform* targetTransform_;

	//カメラモード
	CAMERA_MODE mode_;

	//一つ前のカメラモード
	CAMERA_MODE preMode_;

	//カメラの位置
	VECTOR pos_;

	//一フレーム前のカメラの位置
	VECTOR prePos_;

	//カメラ角度(rad)
	VECTOR angles_;

	//X軸回転が無い角度
	Quaternion rotOutX_;

	//カメラ角度
	Quaternion rot_;

	//注視点
	VECTOR targetPos_;

	//カメラの上方向
	VECTOR cameraUp_;

	//1f前のマウス座標
	Vector2 preMousePos_;

	//現在のマウス座標
	Vector2 nowMousePos_;

	//マウス時のカメラの角度
	Vector2f radXY_;

	//マウス時の1f前のカメラの角度
	Vector2f preRadXY_;

	//カメラの回転力
	Vector2f cameraRotPow_;

	//カメラの初期角度
	Quaternion defSetRot_;

	//デバッグ表記用
	VECTOR debugAtan2;

	//カメラをロック
	bool cameraMoveLock_;

	void InitCollider(void);

	//カメラを初期位置に戻す
	void SetDefault(void);

	//追従対象との位置同期
	void SyncFollow(void);

	//定点時の追従対象との位置同期
	void SyncFollowFix(void);

	//マウス時の位置同期
	void SyncMouse(void);

	//ロックオンカメラ時の位置同期
	void SyncTarget(void);

	//カメラ操作
	void ProcessRot(void);

	//マウス時のカメラ操作
	void ProcessRotMouse(void);

	//モード変更時に行われる関数
	std::unordered_map<CAMERA_MODE, std::function<void(void)>> modeChanges_;
	void ModeChange_FixedPoint(void);		//定点
	void ModeChange_Free(void);				//自由視点
	void ModeChange_Follow(void);			//追従
	void ModeChange_FollowFix(void);		//定点追従
	void ModeChange_FollowMouse(void);		//マウス追従
	void ModeChange_LockOn(void);			//ロックオン
	
	//モードごとの更新処理
	std::function<void(void)> modeUpdate_;
	void ModeUpdate_FixedPoint(void);		//定点
	void ModeUpdate_Free(void);				//自由視点
	void ModeUpdate_Follow(void);			//追従
	void ModeUpdate_FollowFix(void);		//定点追従
	void ModeUpdate_FollowMouse(void);		//マウス追従
	void ModeUpdate_LockOn(void);			//ロックオン

	//モードごとの描画前処理
	std::function<void(void)> setBeforeDrawMode_;
	void SetBeforeDrawMode_FixedPoint(void);	//定点
	void SetBeforeDrawMode_Free(void);			//自由
	void SetBeforeDrawMode_Follow(void);		//追従
	void SetBeforeDrawMode_FollowFix(void);		//定点追従
	void SetBeforeDrawMode_FollowMouse(void);	//マウス追従
	void SetBeforeDrawMode_LockOn(void);		//ロックオン

	/// <summary>
	/// 入力するカメラモードがエラー
	/// </summary>
	/// <param name="mode">カメラモード</param>
	/// <returns>
	/// true : エラー
	/// false : 正常
	/// </returns>
	const bool IsCameraModeError(const CAMERA_MODE mode) const;

	//ステージとの判定
	void CollisionStage(void);

	//カメラの方向を取得
	const VECTOR GetCameraVec(void);

	//追従対象がエラー
	const bool IsFollowTransformError(void) const;

	//モード遷移の登録
	void InitModeChange(void);

};

