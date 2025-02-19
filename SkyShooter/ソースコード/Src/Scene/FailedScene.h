#pragma once
#include "SceneBase.h"
class FailedScene :
    public SceneBase
{
public:

	FailedScene(void);
	virtual ~FailedScene(void) override;

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void) override;

	/// <summary>
	/// �񓯊����[�h
	/// </summary>
	virtual void ASyncLoad(void) override;

private:

	//���̏�����
	virtual void InitSound(void) override;

	//�f�o�b�O�p�`��
	void DrawDebugUI(void);
	void DrawDebug3D(void);
};

