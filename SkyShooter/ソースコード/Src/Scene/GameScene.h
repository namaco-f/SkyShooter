#pragma once

#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include "../Observer/Message.h"
#include "SceneBase.h"

class Actors;
class CharactorBase;
class UIBase;
class Ground;
class SkyDome;
class SoundController;
class UIDataManager;
class Observer;

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene
	: public SceneBase
{

public:

	GameScene(void);
	virtual ~GameScene(void) override;

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

	UIDataManager& uiDataMng_;

	//�f�o�b�O�`��
	void DrawDebug(void);

	//�L�����N�^�[��ǉ�
	void AddActors(void);

	//UI��ǉ�
	void AddUI(void);

	void GameClear(void);
	void GameFailed(void);

	virtual void InitSound(void) override;

	void InitCamera(void);

	void InitMessageUpdate(void);
	void InitASyncLoad(void);

	std::unordered_map<Message, std::function<void(void)>> messageUpdate_;

	virtual void InitChangeState(void) override;

	virtual void ChangeState_Load(void) override;
	virtual void ChangeState_Play(void) override;
	virtual void ChangeState_End(void) override;

	virtual void StateUpdate_Load(void) override;
	virtual void StateUpdate_Play(void) override;
	virtual void StateUpdate_End(void) override;

	virtual void StateDraw_Load(void) override;
	virtual void StateDraw_Play(void) override;
	virtual void StateDraw_End(void) override;
};
