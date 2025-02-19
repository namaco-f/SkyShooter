#pragma once
#include <memory>
#include "../ActorBase.h"

class TimeManager;
class EffectController;

/// <summary>
/// �e�̊��N���X
/// </summary>
class ShotBase
	: public ActorBase
{

public:

	ShotBase(void) = default;
	virtual ~ShotBase(void) override = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="trans">���f��������</param>
	explicit ShotBase(const Transform& trans);

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void) override final;

	/// <summary>
	/// �^�[�Q�b�g�̎w��
	/// </summary>
	/// <param name="targetPos">�ڕW</param>
	void SetTargetPos(const VECTOR& targetPos);
	
	/// <summary>
	/// �e���ˈʒu�̐ݒ�
	/// </summary>
	/// <param name="shotPos">�e���ˊJ�n�ʒu</param>
	void SetShotPos(const VECTOR& shotPos);

	///// <summary>
	///// �e�̐�������
	///// </summary>
	///// <returns>
	///// true : ����
	///// false : �j��
	///// </returns>
	//virtual const bool IsAlive(void) const override;
	
	//return aliveCnt_ > 0.0f;

	/// <summary>
	/// �j�󏈗�
	/// </summary>
	virtual void Destroy(void) = 0;

protected:

	//����
	TimeManager& timeMng_;

	//�e�̐�������
	float aliveCnt_;

	//�f���^�^�C��
	float deltaTime_;

	//�^�[�Q�b�g���W
	VECTOR targetPos_;

	//�e�j�󎞂ɍs����֐�
	std::function<void(void)> destroyFunc_;

	/// <summary>
	/// �e�̔��ˈʒu�̐ݒ�
	/// </summary>
	virtual void InitShotStartPos(void) = 0;


	virtual void UpdateState(void) = 0;

	/// <summary>
	/// ���f������̍X�V
	/// </summary>
	virtual void UpdateTransform(void);

	/// <summary>
	/// �S�Ă̍X�V�����I����Ɏ��s��������
	/// </summary>
	virtual void UpdateEnd(void);

	/// <summary>
	/// �ړ�
	/// </summary>
	virtual void Move(void) = 0;

	//�Փˎ��ɍs����֐�
	virtual void IsHit_Enemy(void);		//�G�ƏՓ�
	virtual void IsHit_Player(void);	//�v���C���[�ƏՓ�

	virtual void InitIsHitUpdate(void) override;		

	//�G�ƏՓ˂����ۂ̏���
	virtual void HitEnemy(void);

	//���@�ƏՓ˂����ۂ̏���
	virtual void HitPlayer(void);

	//�j�󂳂�Ă��邩
	virtual const bool IsDestroy(void) const;

private:

};

