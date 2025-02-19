#pragma once

#include <unordered_map>
#include <functional>
#include "../Common/Geometry2D.h"
#include "SceneBase.h"

struct SelectControllerImgData
{
	int handle;				//画像
	double scl;				//サイズスケール
	double defScl;			//デフォルトのサイズスケール
	unsigned int color;		//色
	Vector2 imgSize;		//画像のサイズ

	explicit SelectControllerImgData(const double defScale = 1.0)
		:
		handle(-1),
		scl(defScale),
		defScl(defScale),
		color(0xffffff),
		imgSize(Vector2())
	{};

	//スケールを変更
	void ChangeScale(const double scale)
	{
		scl = scale;
	}

	//スケールをリセット
	void ResetScale(void)
	{
		scl = defScl;
	}
};

/// <summary>
/// 選択画面
/// </summary>
class SelectScene :
    public SceneBase
{
public:

    SelectScene(void);
    virtual ~SelectScene(void) override;

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

	//状態
	enum class SceneState
	{
		Open,
		Menu,
		Close,
	};

	//UIデータ管理
	UIDataManager& uiDataMng_;

	//カーソル位置
	int idx_;

	//カーソル移動最大
	int max_;

	//フォント
	int fontHandle_;

	//コントローラー画像
	SelectControllerImgData controller_;

	//キーボード
	SelectControllerImgData keyboard_;

	//カーソル画像
	int cursorHandle_;

	//状態
	SceneState sceneState_;

	//演出時間
	float stagingTime_;

	//音の初期化
	virtual void InitSound(void) override;

	//カーソル移動
	virtual void MoveCursor(void);

	//デバッグ表記
	virtual void DrawDebug(void);

	//コントローラー操作を選択したか
	virtual bool PlayWithController(void) const;

	//状態遷移の登録
	virtual void InitChangeSceneState(void);

	//状態遷移
	virtual void ChangeSceneState(const SceneState sceneState);

	//状態遷移時に実行される関数
	std::unordered_map<SceneState, std::function<void(void)>> sceneStateChange_;
	virtual void ChangeSceneState_Open(void);		//画面開
	virtual void ChangeSceneState_Menu(void);		//選択画面
	virtual void ChangeSceneState_Close(void);		//画面閉

	//状態ごとの更新処理
	std::function<void(void)> sceneStateUpdate_;
	virtual void SceneStateUpdate_Open(void);		//画面開
	virtual void SceneStateUpdate_Menu(void);		//選択画面
	virtual void SceneStateUpdate_Close(void);		//画面閉

	//状態ごとの描画処理
	std::function<void(void)> sceneStateDraw_;
	virtual void SceneStateDraw_Open(void);			//画面開
	virtual void SceneStateDraw_Menu(void);			//選択画面
	virtual void SceneStateDraw_Close(void);		//画面閉

	//背景描画
	virtual void DrawBGBox(const Vector2& boxStartPos,const Vector2& boxSize);
};

