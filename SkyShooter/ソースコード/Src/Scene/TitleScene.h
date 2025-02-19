#pragma once

#include <unordered_map>
#include <functional>
#include "SceneBase.h"
//#include "../Object/Common/Transform.h"

class SceneManager;
class InputManager;
class ActorManager;

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene 
	: public SceneBase
{

public:

	TitleScene(void);
	virtual ~TitleScene(void) override;
	
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

protected:

	//�V�[�����
	enum class SceneState
	{
		Start,		//�X�^�[�g���
		Menu,		//�^�C�g�����j���[
	};

	//�^�C�g�����S�摜�n���h��
	int titleLogoHandle_;

	//�J�[�\���摜�n���h��
	int cursorHandle_;

	//�T�C�Y50�̃t�H���g
	int s50FontHandle_;

	//�V�[�����
	SceneState sceneState_;

	virtual void InitSound(void) override;

	virtual void Draw2D(void);

	virtual void DrawTitleLogo(void);

	//�f�o�b�O�p�`��
	virtual void DrawDebugUI(void);
	virtual void DrawDebug3D(void);

	//�V�[����ԑJ��
	void ChangeSceneState(const SceneState state);

	//�V�[����ԑJ�ڎ��̏�����
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Start(void);	//�X�^�[�g���
	virtual void ChangeSceneState_Menu(void);	//�^�C�g�����j���[
	
	//�V�[����Ԃ��Ƃ̍X�V����
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Start(void);	//�X�^�[�g���
	virtual void SceneStateUpdate_Menu(void);	//�^�C�g�����j���[
	
	//�V�[����Ԃ��Ƃ̉�ʕ`��
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Start(void);	//�X�^�[�g���
	virtual void SceneStateDraw_Menu(void);		//�^�C�g�����j���[

	//��ʏ�ԑJ�ڂ̓o�^
	virtual void InitChangeState(void) override;

	//��ʏ�ԑJ�ڎ��̏�����
	virtual void ChangeState_Load(void) override;	//�Ǎ���
	virtual void ChangeState_Play(void) override;	//���s��
	virtual void ChangeState_End(void) override;	//�I����

	//��ʏ�Ԃ��Ƃ̍X�V
	virtual void StateUpdate_Load(void) override;	//�Ǎ���
	virtual void StateUpdate_Play(void) override;	//���s��
	virtual void StateUpdate_End(void) override;	//�I����

	//��ʏ�Ԃ��Ƃ̕`��
	virtual void StateDraw_Load(void) override;		//�Ǎ���
	virtual void StateDraw_Play(void) override;		//���s��
	virtual void StateDraw_End(void) override;		//�I����

private:

};



