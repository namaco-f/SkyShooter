#pragma once

#include <unordered_map>
#include <functional>
#include "SceneBase.h"
//#include "../Object/Common/Transform.h"

class SceneManager;
class InputManager;
class ActorManager;

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene 
	: public SceneBase
{

public:

	TitleScene(void);
	virtual ~TitleScene(void) override;
	
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

protected:

	//シーン状態
	enum class SceneState
	{
		Start,		//スタート画面
		Menu,		//タイトルメニュー
	};

	//タイトルロゴ画像ハンドル
	int titleLogoHandle_;

	//カーソル画像ハンドル
	int cursorHandle_;

	//サイズ50のフォント
	int s50FontHandle_;

	//シーン状態
	SceneState sceneState_;

	virtual void InitSound(void) override;

	virtual void Draw2D(void);

	virtual void DrawTitleLogo(void);

	//デバッグ用描画
	virtual void DrawDebugUI(void);
	virtual void DrawDebug3D(void);

	//シーン状態遷移
	void ChangeSceneState(const SceneState state);

	//シーン状態遷移時の初期化
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Start(void);	//スタート画面
	virtual void ChangeSceneState_Menu(void);	//タイトルメニュー
	
	//シーン状態ごとの更新処理
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Start(void);	//スタート画面
	virtual void SceneStateUpdate_Menu(void);	//タイトルメニュー
	
	//シーン状態ごとの画面描画
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Start(void);	//スタート画面
	virtual void SceneStateDraw_Menu(void);		//タイトルメニュー

	//画面状態遷移の登録
	virtual void InitChangeState(void) override;

	//画面状態遷移時の初期化
	virtual void ChangeState_Load(void) override;	//読込中
	virtual void ChangeState_Play(void) override;	//実行中
	virtual void ChangeState_End(void) override;	//終了中

	//画面状態ごとの更新
	virtual void StateUpdate_Load(void) override;	//読込中
	virtual void StateUpdate_Play(void) override;	//実行中
	virtual void StateUpdate_End(void) override;	//終了中

	//画面状態ごとの描画
	virtual void StateDraw_Load(void) override;		//読込中
	virtual void StateDraw_Play(void) override;		//実行中
	virtual void StateDraw_End(void) override;		//終了中

private:

};



