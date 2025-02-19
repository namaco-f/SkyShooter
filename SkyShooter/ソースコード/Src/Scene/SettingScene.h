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

	//状態
	enum class SceneState
	{
		Open,
		Menu,
		Close,
	};

	//UIデータ管理
	UIDataManager& uiDataMng_;

	//選択しているUIデータ
	std::vector<SRC_UIData> selectUIData_;

	//音量データ
	std::unordered_map<SRC_UIData, float> volumeData_;

	int idx_;
	int max_;

	//音量変化検知
	bool isChangeSoundVol_;

	//状態
	SceneState sceneState_;

	//演出時間
	float stagingTime_;

	int bgmTextImgHandle_;
	int seTextImgHandle_;
	int cursorImgHandle_;

	int s60PgothicFontHandle_;

	//音の初期化
	virtual void InitSound(void) override;

	//音量データの読込、変数初期化
	void InitSettingVolumeData(const SRC_UIData srcUIData);

	//音量更新
	void UpdateSoundVolume(void);

	//カーソル移動
	void MoveCursor(void);

	//演出時間が終了したか
	const bool IsEndStaging(void) const;

	//状態遷移の登録
	virtual void InitChangeSceneState(void);

	//状態遷移
	virtual void ChangeSceneState(const SceneState state);

	//状態遷移時に実行される関数
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Open(void);	//画面開
	virtual void ChangeSceneState_Menu(void);	//選択画面
	virtual void ChangeSceneState_Close(void);	//画面閉

	//状態ごとの更新処理
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Open(void);			//画面開
	virtual void SceneStateUpdate_Menu(void);			//選択画面
	virtual void SceneStateUpdate_Close(void);		//画面閉

	//状態ごとの描画処理
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Open(void);			//画面開
	virtual void SceneStateDraw_Menu(void);			//選択画面
	virtual void SceneStateDraw_Close(void);			//画面閉

	//背景描画
	virtual void DrawBGBox(const Vector2& boxStartPos, const Vector2& boxSize);
};

