#pragma once
#include <unordered_map>
#include <vector>
#include "../Common/Geometry2D.h"
#include "../UI/UIType.h"
#include "SceneBase.h"

class UIDataManager;

class SettingScene :
    public SceneBase
{
public:

    SettingScene(void);
    virtual ~SettingScene(void) override;

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

	//���
	enum class SceneState
	{
		Open,
		Menu,
		Close,
	};

	//UI�f�[�^�Ǘ�
	UIDataManager& uiDataMng_;

	//�I�����Ă���UI�f�[�^
	std::vector<SRC_UIData> selectUIData_;

	//���ʃf�[�^
	std::unordered_map<SRC_UIData, float> volumeData_;

	int idx_;
	int max_;

	//���ʕω����m
	bool isChangeSoundVol_;

	//���
	SceneState sceneState_;

	//���o����
	float stagingTime_;

	int bgmTextImgHandle_;
	int seTextImgHandle_;
	int cursorImgHandle_;

	int s60PgothicFontHandle_;

	//���̏�����
	virtual void InitSound(void) override;

	//���ʃf�[�^�̓Ǎ��A�ϐ�������
	void InitSettingVolumeData(const SRC_UIData srcUIData);

	//���ʍX�V
	void UpdateSoundVolume(void);

	//�J�[�\���ړ�
	void MoveCursor(void);

	//���o���Ԃ��I��������
	const bool IsEndStaging(void) const;

	//��ԑJ�ڂ̓o�^
	virtual void InitChangeSceneState(void);

	//��ԑJ��
	virtual void ChangeSceneState(const SceneState state);

	//��ԑJ�ڎ��Ɏ��s�����֐�
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Open(void);	//��ʊJ
	virtual void ChangeSceneState_Menu(void);	//�I�����
	virtual void ChangeSceneState_Close(void);	//��ʕ�

	//��Ԃ��Ƃ̍X�V����
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Open(void);			//��ʊJ
	virtual void SceneStateUpdate_Menu(void);			//�I�����
	virtual void SceneStateUpdate_Close(void);		//��ʕ�

	//��Ԃ��Ƃ̕`�揈��
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Open(void);			//��ʊJ
	virtual void SceneStateDraw_Menu(void);			//�I�����
	virtual void SceneStateDraw_Close(void);			//��ʕ�

	//�w�i�`��
	virtual void DrawBGBox(const Vector2& boxStartPos, const Vector2& boxSize);
};

