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
/// ゲームシーン
/// </summary>
class GameScene
	: public SceneBase
{

public:

	GameScene(void);
	virtual ~GameScene(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void) override;

	/// <summary>
	/// 非同期ロード
	/// </summary>
	virtual void ASyncLoad(void) override;

private:

	UIDataManager& uiDataMng_;

	//デバッグ描画
	void DrawDebug(void);

	//キャラクターを追加
	void AddActors(void);

	//UIを追加
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
