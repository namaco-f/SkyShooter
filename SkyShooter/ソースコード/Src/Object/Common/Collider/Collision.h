#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include "ColliderTag.h"
#include "../Transform.h"

class Collider;

/// <summary>
/// �Փ˔���̌v�Z
/// </summary>
class Collision
{
public:

	//���f���ȊO�Ƃ̔��莞�ɕԂ����
	struct HitColInfo
	{
		//VECTOR hitPos;	//�Փ˂������W(���S)
		VECTOR hitDir;		//�Փ˂�������(���_�I�_�Ƃ��ɒ��S)
		bool isHit;			//�Փ˂�����

		HitColInfo()
			:
			hitDir{},
			isHit(false)
		{};
	};

	Collision(void);

	virtual ~Collision(void);

	const HitColInfo GetColResultHitColInfo(const Collider& colA, const Collider& colB);

	/// <summary>
	/// �Փ˔��茋�ʂ�Ԃ� : ���f���p
	/// </summary>
	/// <param name="objA">�I�u�W�F�N�g��</param>
	/// <param name="objB">�I�u�W�F�N�g��</param>
	/// <returns></returns>
	const MV1_COLL_RESULT_POLY GetCollResultPoly_Line(
		const Collider& colA, const Collider& colB);


	/// <summary>
	/// ���ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="colSphere">���R���C�_�[</param>
	/// <param name="colCapsule">�J�v�Z���R���C�_�[</param>
	/// <returns>�R���W�������� HitColInfo</returns>
	const HitColInfo IsHitSphereCapsuleResult(
		const Collider& colA, 
		const Collider& colB
	);

	/// <summary>
	///  �����m�̓����蔻��
	/// </summary>
	/// <param name="colA">��</param>
	/// <param name="colB">��</param>
	/// <returns>�R���W�������� HitColInfo</returns>
	const HitColInfo IsHitSpheresResult(
		const Collider& colA,
		const Collider& colB
	);

	/// <summary>
	///  �����m�̓����蔻��
	/// </summary>
	/// <param name="colA">��</param>
	/// <param name="colB">��</param>
	/// <returns>�R���W�������� HitColInfo</returns>
	const HitColInfo IsHitCapsulesResult(
		const Collider& colA,
		const Collider& colB
	);

	//�|���S�����̍폜
	void CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& hitPoly);

	/// <summary>
	/// �Փ˔��茋�ʂ�Ԃ� : ���f���p
	/// </summary>
	/// <param name="objA">�I�u�W�F�N�g��</param>
	/// <param name="objB">�I�u�W�F�N�g��</param>
	const MV1_COLL_RESULT_POLY GetCollResultPoly_Line(
		const Collider& colA, const VECTOR& lineTop,const VECTOR& lineBottom);

	/// <summary>
	/// �ړ������Ƃ̔���
	/// </summary>
	/// <param name="movedPos">�ړ�����W</param>
	void IsCollisionMoveLimit(VECTOR& movedPos);


private:

	//���茋�ʂ��i�[ : �z��̉�����K�v
	MV1_COLL_RESULT_POLY_DIM resultPolyDim_;

	//��̏Փˏ��
	const HitColInfo nullHitColInfo_ = HitColInfo();

	//���茋�ʂ��i�[���Ă��邩
	bool isStoreResult_;

	/// <summary>
	/// ���ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="colModel">��</param>
	/// <param name="colSphere">�J�v�Z��</param>
	/// <returns>�R���W��������</returns>
	const HitColInfo CollisionSphereCapsule(
		const Collider& colSphere,
		const Collider& colCapsule
	);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="colModel">��</param>
	/// <param name="colSphere">��</param>
	/// <returns>�R���W��������</returns>
	const HitColInfo CollisionCapsules(
		const Collider& colCapsuleA,
		const Collider& colCapsuleB
	);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="colModel">��</param>
	/// <param name="colSphere">��</param>
	/// <returns>�R���W��������</returns>
	const HitColInfo CollisionSpheres(
		const Collider& colSphereA,
		const Collider& colSphereB
	);

	///// <summary>
	///// ���f���̏Փ˔���
	///// </summary>
	///// <param name="colModel">���f��</param>
	///// <param name="colA">�ՓˑΏ�</param>
	///// <returns>�R���W�������� : MV1_COLL_RESULT_POLY_DIM</returns>
	//const MV1_COLL_RESULT_POLY_DIM& CollisionModel(
	//	std::weak_ptr<Collider> colModel,
	//	std::weak_ptr<Collider> colA
	//);


	/// <summary>
	/// ���f���̏Փ˔���
	/// </summary>
	/// <param name="colModel">����</param>
	/// <param name="colA">���f��</param>
	/// <returns>�R���W�������� : MV1_COLL_RESULT_POLY</returns>
	const MV1_COLL_RESULT_POLY CollisionLine(
		const Collider& colLine,
		const Collider& colModel
	) const;

	/// <summary>
	/// ���f���̏Փ˗p�̊֐����擾
	/// </summary>
	/// <param name="colModel">���f��</param>
	/// <param name="colA">�ՓˑΏ�</param>
	/// <returns>�Փ˔���p�̊֐�</returns>
	const std::function<MV1_COLL_RESULT_POLY_DIM(void)>GetCollModelFunc(
		Collider& colModel,
		Collider& colA
	) const;

	//�ړ������̔���
	void CollisionMoveLimit(float& pos, const float minPos, const float maxPos);

	//�Փ˔��肪�J�v�Z���^������
	const bool IsCapsule(const Collider& col) const;

	//�Փ˔��肪���f���^������
	const bool IsModel(Collider& col) const;

	//�Փ˔��肪���^������
	const bool IsSphere(const Collider& col) const;

	//�Փ˔��肪�����^������
	const bool IsLine(const Collider& col) const;

	//HitColInfo���K�p�\�ȃR���C�_��ʂ�
	const bool AbleToGetHitColInfo(const Collider& col) const;

};

