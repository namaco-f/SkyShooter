#pragma once
#include "../ActorBase.h"

/// <summary>
/// �X�J�C�h�[��
/// </summary>
class SkyDome :
    public ActorBase
{

public:

	SkyDome(void);
	virtual ~SkyDome(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void) override;

protected:

private:

	//���������̓Ǎ�����
	virtual void InitLoad(void) override;

	//���f��������̏�����
	virtual void InitTransform(void) override;

	//�Փ˔���̏�����
	virtual void InitCollider(void) override;

	//�����_�[�̏�����
	virtual void InitRender(void) override;

	//�S�Ă̏��������I�����ɍs���鏉��������
	virtual void InitEnd(void) override;

	//isHitUpdate_�̓o�^
	virtual void InitIsHitUpdate(void) override;

};

