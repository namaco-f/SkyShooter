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

	//シーン状態
	enum class SceneState
	{
		Result,
		Menu,
	};

	//時間管理クラス
	TimeManager& timeMng_;

	//クリアタイム(秒)
	const float clearTime_;

	//クリアタイム(分秒ミリ秒)
	Time_Unit clearTimeUnit_;

	SceneState sceneState_;

	int s60FontHandle_;
	int s40FontHandle_;
	int cursorHandle_;
	int clearTextHandle_;

	//音の初期化
	virtual void InitSound(void) override;

	//デバッグ用描画
	void DrawDebugUI(void);
	void DrawDebug3D(void);

	//クリアタイム描画
	void DrawClearTime(void);

	//状態遷移
	virtual void ChangeSceneState(const SceneState sceneState);

	//状態遷移時の初期化
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Result(void);	//リザルト
	virtual void ChangeSceneState_Menu(void);	//クリアメニュー


	//状態ごとの更新処理
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Result(void);	//リザルト
	virtual void SceneStateUpdate_Menu(void);	//クリアメニュー

	//状態ごとの画面描画
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Result(void);	//リザルト
	virtual void SceneStateDraw_Menu(void);		//クリアメニュー

	virtual void DrawBGBox(const Vector2& boxStartPos, const Vector2& boxSize);

};

