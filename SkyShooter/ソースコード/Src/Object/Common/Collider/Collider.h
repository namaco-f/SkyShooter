#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "ColliderTag.h"
#include "../Transform.h"

/// <summary>
/// �Փ˔���̏���ێ�����N���X
/// </summary>
class Collider
{

public:

	/// <summary>
	/// �R���C�_���̍\����
	/// </summary>
	struct ColliderData
	{
		Transform transform_;			//���f��������
		Transform preTransform_;		//1f�O�̃��f��������
		COLLISION_TYPE colType_;		//�Փ˔���̎��
		COLLISION_SUBTYPE subType_;		//�I�u�W�F�N�g�̃t���[����
		int colModelFrameIdx_;			//�t���[���ԍ�
		bool ableCol_;					//�Փ˔��肪�L����

		//VECTOR localPos;

		float radius_;				//���a
		VECTOR localPosTop_;		//��[���[�J�����W
		VECTOR localPosBottom_;		//���[���[�J�����W
		VECTOR localPosCenter_;		//���S���W
		Quaternion localQuaRot_;

		ColliderData() :
			transform_(Transform()),
			preTransform_(Transform()),
			subType_(COLLISION_SUBTYPE::NONE),
			colType_(COLLISION_TYPE::NONE),
			colModelFrameIdx_(-1),
			ableCol_(false),
			radius_(0.0f),
			localPosTop_{},
			localPosBottom_{},
			localPosCenter_{},
			localQuaRot_(Quaternion())
		{};
	};

	Collider(void) = default;
	~Collider(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="colData">�R���C�_���</param>
	explicit Collider(const ColliderData& colData);

	/// <summary>
	/// �R���C�_�̍X�V
	/// </summary>
	/// <param name="colData">�R���C�_���</param>
	void UpdateCollider(const ColliderData& colData);

	/// <summary>
	/// �R���C�_���̃��f����������X�V
	/// </summary>
	/// <param name="transform">���f��������</param>
	void UpdateTransform(const Transform& transform);

	/// <summary>
	/// �R���C�_���̃��f����������X�V
	/// </summary>
	/// <param name="transform">���f��������</param>
	void UpdateTransformQuaRot(const Quaternion& quaternion);

	/// <summary>
	/// �R���C�_�̃��[�J�����W��ݒ�
	/// </summary>
	void SetLocalPos(const VECTOR& localPos);

	/// <summary>
	/// 3D�R���C�_�`��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// ���f��������̍��W���擾
	/// </summary>
	inline const VECTOR& GetTrasformPos(void) const
	{
		return colData_.transform_.pos;
	}

	/// <summary>
	/// �����蔻��̎�ނ��擾
	/// </summary>
	inline const COLLISION_TYPE GetColType(void) const
	{
		return colData_.colType_;
	}

	/// <summary>
	/// �Փ˔�����s���Ă��郂�f���̃n���h��ID���擾
	/// </summary>
	inline const int GetModelId(void) const
	{
		return colData_.transform_.modelId_;
	}

	/// <summary>
	/// �Ǐ]�t���[���ԍ����擾
	/// </summary>
	inline const int GetFrameIdx(void) const
	{
		return colData_.colModelFrameIdx_;
	}

	/// <summary>
	/// �Փ˔�����s�����̃t���O���擾
	/// </summary>
	inline const bool GetAbleCol(void) const
	{
		return colData_.ableCol_;
	}

	/// <summary>
	/// �R���C�_�����擾
	/// </summary>
	inline const ColliderData& GetColliderData(void) const
	{
		return colData_;
	}

	/// <summary>
	/// �Ǐ]�t���[�������擾
	/// </summary>
	inline const COLLISION_SUBTYPE GetObjSubType(void) const
	{
		return colData_.subType_;
	}

	/// <summary>
	/// ��[�̃��[���h���W���擾
	/// </summary>
	VECTOR GetPosTop(void) const;

	/// <summary>
	/// ���[�̃��[���h���W���擾
	/// </summary>
	VECTOR GetPosDown(void) const;
	
	//���[���h���W���擾
	const VECTOR& GetPos(void) const;

	/// <summary>
	/// ���S�̃��[���h���W���擾
	/// </summary>
	inline const VECTOR& GetCenterPos(void) const
	{
		return colData_.localPosCenter_;
	}

	/// <summary>
	/// ���a���擾
	/// </summary>
	inline const float GetRadius(void) const
	{
		return colData_.radius_;
	}

	/// <summary>
	/// ��[�̃��[�J�����W��ݒ�
	/// </summary>
	/// <param name="val">��[�̃��[�J�����W</param>
	void SetPosTop(const VECTOR& val);
	
	/// <summary>
	/// ���[�̃��[�J�����W��ݒ�
	/// </summary>
	/// <param name="val">���[�̃��[�J�����W</param>
	void SetPosDown(const VECTOR& val);
	
	/// <summary>
	/// ���S���W��ݒ�
	/// </summary>
	/// <param name="val">���S���W</param>
	void SetPosCenter(const VECTOR& val);

	/// <summary>
	/// ���a��ݒ�
	/// </summary>
	/// <param name="val">���a</param>
	void SetRadius(const float& val);

	/// <summary>
	/// �t���[���ԍ���ݒ�
	/// </summary>
	/// <param name="val">�t���[���ԍ�</param>
	void SetFrameIndex(const int& val);

	/// <summary>
	/// �����蔻���L���ɂ��邩�ݒ�
	/// </summary>
	/// <param name="val">
	/// �����蔻��̗L��
	/// </param>
	void SetColAble(const bool val);
	
	
	/// <summary>
	/// ���΍��W����]�����ă��[���h���W�Ŏ擾����
	/// </summary>
	/// <param name="localPos">���΍��W</param>
	/// <returns>��]��̑��΍��W�̃��[���h���W</returns>
	VECTOR GetRotPos(const VECTOR& localPos) const;

	/// <summary>
	/// ���[�J�����W�̎擾
	/// </summary>
	inline const VECTOR& GetLocalPos(void) const
	{
		return localPos_;
	}


private:

	//���[�J�����W
	VECTOR localPos_;

	//�R���C�_���̍\����
	ColliderData colData_;

	//�R���C�_�̎�ނɍ��킹���`��
	void InitColTypeDraw(void);

	//�R���C�_�̎�ނ��Z�b�g
	void SetColTypeDraw(const COLLISION_TYPE colType);

	//�R���C�_���Ƃ̑J�ڏ�Ԃ̏��
	std::unordered_map<COLLISION_TYPE, std::function<void(void)>> colTypeChange_;
	void Set_Sphere(void);			//����
	void Set_Capsule(void);			//�J�v�Z��
	void Set_Model(void);			//���f��
	void Set_Line(void);			//����
	void Set_Cylinder(void);		//

	//�R���C�_���Ƃ̕`��
	std::function<void(void)> colTypeDraw_;
	void Draw_Sphere(void);			//����
	void Draw_Capsule(void);		//�J�v�Z��
	void Draw_Model(void);			//���f��
	void Draw_Line(void);			//��
	void Draw_Cylinder(void);		//

};

