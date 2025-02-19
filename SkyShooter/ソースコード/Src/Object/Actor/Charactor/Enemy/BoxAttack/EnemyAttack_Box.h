#pragma once

#include <memory>
#include "../Common/Vector2/Vector2f.h"
#include "../../../../Common/Transform.h"
#include "../../../ActorBase.h"

class EffectController;

/// <summary>
/// �U�����
/// </summary>
struct BoxAttackData
{
	float attackDelay;     //�U�����肪�L���ɂȂ�܂ł̎���
	float totalTime;       //�U�����肪�L���Ȏ���
	float step;            //�J�E���^�[�ϐ�
	bool attackDelayEnd;

	BoxAttackData(void)
		:
		attackDelay(0.0f),
		totalTime(0.0f),
		step(0.0f),
		attackDelayEnd(false)
	{};

	BoxAttackData(const float delay, const float total)
		:
		attackDelay(delay),
		totalTime(total),
		step(0.0f),
		attackDelayEnd(false)
	{};

	/// <summary>
	/// �U���J�n�܂ł̃f�B���C���I�����Ă��邩
	/// </summary>
	const bool IsEndAttackDelayTime(void) const 
	{
		return attackDelay < step;
	}

	/// <summary>
	/// �U�����I�����Ă��邩
	/// </summary>
	const bool IsEndAttackTotalTime(void) const
	{
		return totalTime < step && attackDelayEnd;
	}
		
	const float attackDelayEndPar(void) const
	{
		if (attackDelay == 0.0f)
		{
			//0���Z�����Ȃ�
			return 1.0f;
		}

		return step / attackDelay;
	}
};

/// <summary>
/// ��`�̎��
/// </summary>
enum class BoxType
{
	XY,		//XY���ʏ�
	ZY,		//ZY���ʏ�
	XZ,		//XZ���ʏ�
	MAX		//�J�E���g�p
};

/// <summary>
/// ��`�̏��
/// </summary>
struct BoxInfo
{
	VECTOR boxCenterPos;	//���S���W
	Vector2f boxSize;		//���c�T�C�Y

	Vector2f boxPosTopLeft;			//����
	Vector2f boxPosTopRight;		//�E��
	Vector2f boxPosBottomLeft;		//����
	Vector2f boxPosBottomRight;		//�E��

	BoxInfo(void) : 
		boxCenterPos{}, 
		boxSize{}
	{};

	BoxInfo(const VECTOR& pos, const Vector2f& size)
		:
		boxCenterPos(pos),
		boxSize(size)
	{};
};

/// <summary>
/// �G�̔��^�͈͍̔U���̊��N���X
/// </summary>
class EnemyAttack_Box
	: 
	public ActorBase
{

public:

	EnemyAttack_Box() = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="transform">���f���̐�����</param>
	/// <param name="boxSize">�c���T�C�Y</param>
	/// <param name="boxAttackData">�U���̏��</param>
	/// <param name="boxType">��`�̎��</param>
	EnemyAttack_Box(
		const Transform& transform,
		const Vector2f& boxSize,
		const BoxAttackData& boxAttackData
	);
	virtual ~EnemyAttack_Box(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void) override;

	/// <summary>
	/// �R���C�_�̏�Ԃ𔽓]
	/// </summary>
	virtual void SwitchCollider(void);

	/// <summary>
	/// �U�������擾
	/// </summary>
	/// <returns>boxAttackData_</returns>
	virtual const BoxAttackData& GetBoxAttackData(void) const
	{
		return boxAttackData_;
	}

	/// <summary>
	/// �U���̃f�B���C���I����Ă��邩
	/// </summary>
	/// <returns>
	/// true : �I��
	/// false : �f�B���C��
	/// </returns>
	virtual inline const bool IsAttackDelayEnd(void) const
	{
		return boxAttackData_.attackDelayEnd;
	}

	/// <summary>
	/// �U�����I����Ă��邩
	/// </summary>
	/// <returns>
	/// true : �I��
	/// false : �U���p����
	/// </returns>
	virtual inline const bool IsAttackEnd(void) const
	{
		return boxAttackData_.IsEndAttackTotalTime();
	}

protected:

	//�U���̏��
	BoxAttackData boxAttackData_;

	//��`�̏��
	BoxInfo boxInfo_;

	//��`�̎��
	BoxType boxType_;
	
	//2D�R���C�_�̐���
	virtual void MakeBoxCollider(void) = 0;

	//�f�B���C���̍X�V����
	virtual void UpdateAttackDelayTime(void);

	//�U�����̍X�V����
	virtual void UpdateAttackTotalTime(void);

	//���������̓Ǎ�����
	virtual void InitLoad(void) override;

	//���f��������̏�����
	virtual void InitTransform(void) override;

	//�����_���[�̏�����
	virtual void InitRender(void) override;

	//isHitUpdate_�̓o�^
	virtual void InitIsHitUpdate(void) override;

	//���̍��W���擾
	virtual void CalculateBoxPos(void);

	virtual void AttackEffectDraw(void);

	//���f���̃X�P�[�����擾
	virtual const VECTOR GetBoxScale(void) = 0 ;

	virtual const VECTOR GetBoxChangeScale(void) = 0;

	//���f���̃T�C�Y�̒���
	virtual const VECTOR RegulateBoxScale(const VECTOR& val);

	virtual const float LerpBoxScale(const float& max) const;

};

