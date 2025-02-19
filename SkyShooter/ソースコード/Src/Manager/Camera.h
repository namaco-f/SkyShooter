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
/// �J��������
/// </summary>
class Camera
{
public:

	/// <summary>
	/// �J�������[�h
	/// </summary>
	enum class CAMERA_MODE
	{
		NONE,			//�������p
		FIXED_POINT,	//��_
		FREE,			//���R���_
		FOLLOW,			//�Ǐ]
		FOLLOW_FIX,		//�Ǐ]��_
		FOLLOW_MOUSE,	//�Ǐ]�}�E�X
		LOCKON,			//���b�N�I��
		MAX,			//�ő吔�J�E���g�p
	};

	Camera(void);
	virtual ~Camera(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`��O�̃J�����ݒ�
	/// </summary>
	void SetBeforeDraw(void);

	/// <summary>
	/// �f�o�b�O�`��
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// �J�����ʒu�̎擾
	/// </summary>
	/// <returns>pos</returns>
	inline const VECTOR& GetPos(void) const
	{
		return pos_;
	}

	/// <summary>
	/// �J�����̑���p�x
	/// </summary>
	/// <returns>angles_</returns>
	inline const VECTOR& GetAngles(void) const
	{
		return angles_;
	}

	/// <summary>
	/// �J�����̏����p�x��ݒ�
	/// </summary>
	/// <param name="qua">Quaternion</param>
	void SetDefQuaRot(const Quaternion& qua);
	
	/// <summary>
	/// �J�����̒����_�̎擾
	/// </summary>
	/// <returns>targetPos_</returns>
	inline const VECTOR& GetTargetPos(void) const
	{
		return targetPos_;
	}
	
	/// <summary>
	/// �J�����p�x�̎擾
	/// </summary>
	/// <returns>rot</returns>
	inline const Quaternion& GetQuaRot(void) const
	{
		return rot_;
	}

	/// <summary>
	/// X��]�𔲂����J�����p�x
	/// </summary>
	/// <returns>rotOutX_</returns>
	inline const Quaternion& GetQuaRotOutX(void) const
	{
		return rotOutX_;
	}

	/// <summary>
	/// �J�����̑O�������̎擾
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR GetForward(void) const;

	/// <summary>
	/// �J�������[�h�̕ύX
	/// </summary>
	/// <param name="mode">CAMERA_MODE : �ύX����J�������[�h</param>
	void ChangeMode(const CAMERA_MODE mode);

	/// <summary>
	/// �Ǐ]�Ώۂ̐ݒ�
	/// </summary>
	/// <param name="follow">�Ǐ]�Ώۂ�Transform</param>
	void SetFollow(const Transform* follow);

	/// <summary>
	/// �����_��ݒ�
	/// </summary>
	/// <param name="target">�����Ώۂ�Transform</param>
	void SetTarget(const Transform* target);
	
	
	/// <summary>
	/// �J�����̉�]�͂��Z�b�g
	/// </summary>
	/// <param name="pow">�c�������̃J�����̉�]��</param>
	void SetCameraRotPow(const Vector2f& pow);

	/// <summary>
	/// �J�����V�F�C�N�̐ݒ�
	/// </summary>
	/// <param name="time">����</param>
	/// <param name="shakePow">�ړ���</param>
	/// <param name="timeScale">���Ԃ̉�����</param>
	void SetCameraShake(float time,float shakePow,float timeScale = 10.0f);

	/// <summary>
	/// �J�����V�F�C�N�����s
	/// </summary>
	void CameraShake(void);

	/// <summary>
	/// ���b�N�I���J�������[�h������
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
	///	�J������]�����b�N���邩�ݒ�
	/// </summary>
	void CameraMoveLock(const bool flag);

private:

	//���͐���
	std::unique_ptr<InputController> inputController_;

	//�����_����J�����܂ł̃R���C�_
	std::unique_ptr<Collider> lineCollider_;

	//�J�����V�F�C�N����
	bool isShake_;
	
	//�J�����V�F�C�N�J�E���g�p
	float shakeStep_;

	//�J�����V�F�C�N�̈ړ���
	float shakePow_;

	//�J�����V�F�C�N�̎��Ԃ̉�����
	float timeScale_;

	//�J�������Ǐ]�ΏۂƂ���Transform
	const Transform* followTransform_;

	//�J�����̒����_�ƂȂ�Transform
	const Transform* targetTransform_;

	//�J�������[�h
	CAMERA_MODE mode_;

	//��O�̃J�������[�h
	CAMERA_MODE preMode_;

	//�J�����̈ʒu
	VECTOR pos_;

	//��t���[���O�̃J�����̈ʒu
	VECTOR prePos_;

	//�J�����p�x(rad)
	VECTOR angles_;

	//X����]�������p�x
	Quaternion rotOutX_;

	//�J�����p�x
	Quaternion rot_;

	//�����_
	VECTOR targetPos_;

	//�J�����̏����
	VECTOR cameraUp_;

	//1f�O�̃}�E�X���W
	Vector2 preMousePos_;

	//���݂̃}�E�X���W
	Vector2 nowMousePos_;

	//�}�E�X���̃J�����̊p�x
	Vector2f radXY_;

	//�}�E�X����1f�O�̃J�����̊p�x
	Vector2f preRadXY_;

	//�J�����̉�]��
	Vector2f cameraRotPow_;

	//�J�����̏����p�x
	Quaternion defSetRot_;

	//�f�o�b�O�\�L�p
	VECTOR debugAtan2;

	//�J���������b�N
	bool cameraMoveLock_;

	void InitCollider(void);

	//�J�����������ʒu�ɖ߂�
	void SetDefault(void);

	//�Ǐ]�ΏۂƂ̈ʒu����
	void SyncFollow(void);

	//��_���̒Ǐ]�ΏۂƂ̈ʒu����
	void SyncFollowFix(void);

	//�}�E�X���̈ʒu����
	void SyncMouse(void);

	//���b�N�I���J�������̈ʒu����
	void SyncTarget(void);

	//�J��������
	void ProcessRot(void);

	//�}�E�X���̃J��������
	void ProcessRotMouse(void);

	//���[�h�ύX���ɍs����֐�
	std::unordered_map<CAMERA_MODE, std::function<void(void)>> modeChanges_;
	void ModeChange_FixedPoint(void);		//��_
	void ModeChange_Free(void);				//���R���_
	void ModeChange_Follow(void);			//�Ǐ]
	void ModeChange_FollowFix(void);		//��_�Ǐ]
	void ModeChange_FollowMouse(void);		//�}�E�X�Ǐ]
	void ModeChange_LockOn(void);			//���b�N�I��
	
	//���[�h���Ƃ̍X�V����
	std::function<void(void)> modeUpdate_;
	void ModeUpdate_FixedPoint(void);		//��_
	void ModeUpdate_Free(void);				//���R���_
	void ModeUpdate_Follow(void);			//�Ǐ]
	void ModeUpdate_FollowFix(void);		//��_�Ǐ]
	void ModeUpdate_FollowMouse(void);		//�}�E�X�Ǐ]
	void ModeUpdate_LockOn(void);			//���b�N�I��

	//���[�h���Ƃ̕`��O����
	std::function<void(void)> setBeforeDrawMode_;
	void SetBeforeDrawMode_FixedPoint(void);	//��_
	void SetBeforeDrawMode_Free(void);			//���R
	void SetBeforeDrawMode_Follow(void);		//�Ǐ]
	void SetBeforeDrawMode_FollowFix(void);		//��_�Ǐ]
	void SetBeforeDrawMode_FollowMouse(void);	//�}�E�X�Ǐ]
	void SetBeforeDrawMode_LockOn(void);		//���b�N�I��

	/// <summary>
	/// ���͂���J�������[�h���G���[
	/// </summary>
	/// <param name="mode">�J�������[�h</param>
	/// <returns>
	/// true : �G���[
	/// false : ����
	/// </returns>
	const bool IsCameraModeError(const CAMERA_MODE mode) const;

	//�X�e�[�W�Ƃ̔���
	void CollisionStage(void);

	//�J�����̕������擾
	const VECTOR GetCameraVec(void);

	//�Ǐ]�Ώۂ��G���[
	const bool IsFollowTransformError(void) const;

	//���[�h�J�ڂ̓o�^
	void InitModeChange(void);

};

