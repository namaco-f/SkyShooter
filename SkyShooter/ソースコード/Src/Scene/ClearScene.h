#pragma once

#include <functional>
#include "../Utility/TimeUnit.h"
#include "SceneBase.h"

class TimeManager;

class ClearScene :
    public SceneBase
{
public:

	ClearScene(void);
	virtual ~ClearScene(void) override;

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

	//�V�[�����
	enum class SceneState
	{
		Result,
		Menu,
	};

	//���ԊǗ��N���X
	TimeManager& timeMng_;

	//�N���A�^�C��(�b)
	const float clearTime_;

	//�N���A�^�C��(���b�~���b)
	Time_Unit clearTimeUnit_;

	SceneState sceneState_;

	int s60FontHandle_;
	int s40FontHandle_;
	int cursorHandle_;
	int clearTextHandle_;

	//���̏�����
	virtual void InitSound(void) override;

	//�f�o�b�O�p�`��
	void DrawDebugUI(void);
	void DrawDebug3D(void);

	//�N���A�^�C���`��
	void DrawClearTime(void);

	//��ԑJ��
	virtual void ChangeSceneState(const SceneState sceneState);

	//��ԑJ�ڎ��̏�����
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Result(void);	//���U���g
	virtual void ChangeSceneState_Menu(void);	//�N���A���j���[


	//��Ԃ��Ƃ̍X�V����
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Result(void);	//���U���g
	virtual void SceneStateUpdate_Menu(void);	//�N���A���j���[

	//��Ԃ��Ƃ̉�ʕ`��
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Result(void);	//���U���g
	virtual void SceneStateDraw_Menu(void);		//�N���A���j���[

	virtual void DrawBGBox(const Vector2& boxStartPos, const Vector2& boxSize);

};

