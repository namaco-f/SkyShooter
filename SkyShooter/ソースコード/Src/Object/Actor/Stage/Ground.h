#pragma once
#include "../ActorBase.h"

/// <summary>
/// �n��
/// </summary>
class Ground :
    public ActorBase
{

public:

    Ground(void);
    virtual ~Ground(void);

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

	float uvScrollTime_;

	Transform wall_;

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

	virtual void DrawWall(void);

};

