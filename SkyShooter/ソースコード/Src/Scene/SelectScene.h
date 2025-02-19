#pragma once

#include <unordered_map>
#include <functional>
#include "../Common/Geometry2D.h"
#include "SceneBase.h"

struct SelectControllerImgData
{
	int handle;				//�摜
	double scl;				//�T�C�Y�X�P�[��
	double defScl;			//�f�t�H���g�̃T�C�Y�X�P�[��
	unsigned int color;		//�F
	Vector2 imgSize;		//�摜�̃T�C�Y

	explicit SelectControllerImgData(const double defScale = 1.0)
		:
		handle(-1),
		scl(defScale),
		defScl(defScale),
		color(0xffffff),
		imgSize(Vector2())
	{};

	//�X�P�[����ύX
	void ChangeScale(const double scale)
	{
		scl = scale;
	}

	//�X�P�[�������Z�b�g
	void ResetScale(void)
	{
		scl = defScl;
	}
};

/// <summary>
/// �I�����
/// </summary>
class SelectScene :
    public SceneBase
{
public:

    SelectScene(void);
    virtual ~SelectScene(void) override;

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

	//���
	enum class SceneState
	{
		Open,
		Menu,
		Close,
	};

	//UI�f�[�^�Ǘ�
	UIDataManager& uiDataMng_;

	//�J�[�\���ʒu
	int idx_;

	//�J�[�\���ړ��ő�
	int max_;

	//�t�H���g
	int fontHandle_;

	//�R���g���[���[�摜
	SelectControllerImgData controller_;

	//�L�[�{�[�h
	SelectControllerImgData keyboard_;

	//�J�[�\���摜
	int cursorHandle_;

	//���
	SceneState sceneState_;

	//���o����
	float stagingTime_;

	//���̏�����
	virtual void InitSound(void) override;

	//�J�[�\���ړ�
	virtual void MoveCursor(void);

	//�f�o�b�O�\�L
	virtual void DrawDebug(void);

	//�R���g���[���[�����I��������
	virtual bool PlayWithController(void) const;

	//��ԑJ�ڂ̓o�^
	virtual void InitChangeSceneState(void);

	//��ԑJ��
	virtual void ChangeSceneState(const SceneState sceneState);

	//��ԑJ�ڎ��Ɏ��s�����֐�
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Open(void);		//��ʊJ
	virtual void ChangeSceneState_Menu(void);		//�I�����
	virtual void ChangeSceneState_Close(void);		//��ʕ�

	//��Ԃ��Ƃ̍X�V����
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Open(void);		//��ʊJ
	virtual void SceneStateUpdate_Menu(void);		//�I�����
	virtual void SceneStateUpdate_Close(void);		//��ʕ�

	//��Ԃ��Ƃ̕`�揈��
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Open(void);			//��ʊJ
	virtual void SceneStateDraw_Menu(void);			//�I�����
	virtual void SceneStateDraw_Close(void);		//��ʕ�

	//�w�i�`��
	virtual void DrawBGBox(const Vector2& boxStartPos,const Vector2& boxSize);
};

