#pragma once

#include <memory>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <list>
#include "SceneId.h"
#include "../Singleton.h"

class SceneBase;
class Fader;
class Camera;
class DebugUtility;


/// <summary>
/// �V�[���Ǘ����s���N���X
/// </summary>
class SceneManager
	: public Singleton<SceneManager>
{
public:

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// �V�[���J��
	/// </summary>
	/// <param name="nextId">�J�ڂ���V�[��ID</param>
	void ChangeScene(const SCENE_ID nextId);

	/// <summary>
	/// �ݒ�V�[�����J��
	/// </summary>
	void OpenSettingScene(void);

	/// <summary>
	/// �ݒ�V�[�������
	/// </summary>
	void CloseSettingScene(void);

	/// <summary>
	/// �Z���N�g�V�[�����J��
	/// </summary>
	void OpenSelectScene(void);

	/// <summary>
	/// �Z���N�g�V�[�������
	/// </summary>
	void CloseSelectScene(void);

	/// <summary>
	/// �Q�[�������g���C����
	/// </summary>
	void RetryGame(void);

	/// <summary>
	/// �^�C�g���V�[���ɖ߂�
	/// </summary>
	void ReturnTitleScene(void);

	/// <summary>
	/// �V�[��ID�̎擾
	/// </summary>
	/// <returns>�V�[��ID</returns>
	inline const SCENE_ID GetSceneID(void)
	{
		return sceneId_;
	}

	/// <summary>
	/// ���C���X�N���[���̎擾
	/// </summary>
	/// <returns>���C���X�N���[���̃n���h��ID</returns>
	inline int GetMainScreen(void) const
	{
		return mainScreen_;
	}

	/// <summary>
	/// �V�[���J�ڒ���
	/// </summary>
	/// <returns>
	/// true : �V�[���J�ڒ�
	/// false : �V�[���J�ڂ��Ă��Ȃ�
	/// </returns>
	inline const bool IsSceneChanging(void) const
	{
		return isSceneChanging_;
	}

	/// <summary>
	/// �f�o�b�O�p�N���X�̎擾
	/// </summary>
	/// <returns>�f�o�b�O�p�N���X�̎Q��</returns>
	inline DebugUtility& GetDebugUtility(void) const
	{
		return *debugUtility_;
	}

	/// <summary>
	/// �J�����N���X�̎擾
	/// </summary>
	/// <returns>�J�����N���X�̎Q��</returns>
	inline Camera& GetCamera(void) const
	{
		return *camera_;
	}

private:

	friend class Singleton<SceneManager>;

	//�f�o�b�O�p
	std::unique_ptr<DebugUtility> debugUtility_;

	//�t�F�[�h
	std::unique_ptr<Fader> fader_;

	//�`�悷��V�[��
	std::list<std::unique_ptr<SceneBase>> scenes_;

	//�J����
	std::unique_ptr<Camera> camera_;

	//�V�[��ID
	SCENE_ID sceneId_;

	//�ҋ@���̃V�[��ID
	SCENE_ID waitSceneId_;

	//�V�[���J�ڒ�����
	bool isSceneChanging_;

	//���C���X�N���[��
	int mainScreen_;

	SceneManager(void);
	virtual ~SceneManager(void) override;

	//������
	void Init(void) override;

	//�V�[���J��
	void DoChangeScene(const SCENE_ID sceneId);

	//�V�[���J��
	std::unordered_map<SCENE_ID, std::function<void(void)>> sceneChanges_;
	void SceneChange_None(void);		//None
	void SceneChange_Title(void);		//Title
	void SceneChange_Game(void);		//Game
	void SceneChange_Clear(void);		//Clear
	void SceneChange_Failed(void);		//Failed

	//�t�F�[�h
	void Fade(void);

	//3D�p�̏����ݒ�
	void Init3D(void);

	//�V�[���J�ڂ̓o�^
	void InitSceneChange(void);

	//�V�[�����G���[
	const bool IsSceneError(void) const;

	//�w�肳�ꂽ�V�[�����G���[
	const bool IsSceneError(const SCENE_ID scnId) const;

	//�I�u�W�F�N�g��UI���폜
	void ClearActors(void);
};