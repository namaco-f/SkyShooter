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
/// シーン管理を行うクラス
/// </summary>
class SceneManager
	: public Singleton<SceneManager>
{
public:

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// シーン遷移
	/// </summary>
	/// <param name="nextId">遷移するシーンID</param>
	void ChangeScene(const SCENE_ID nextId);

	/// <summary>
	/// 設定シーンを開く
	/// </summary>
	void OpenSettingScene(void);

	/// <summary>
	/// 設定シーンを閉じる
	/// </summary>
	void CloseSettingScene(void);

	/// <summary>
	/// セレクトシーンを開く
	/// </summary>
	void OpenSelectScene(void);

	/// <summary>
	/// セレクトシーンを閉じる
	/// </summary>
	void CloseSelectScene(void);

	/// <summary>
	/// ゲームをリトライする
	/// </summary>
	void RetryGame(void);

	/// <summary>
	/// タイトルシーンに戻る
	/// </summary>
	void ReturnTitleScene(void);

	/// <summary>
	/// シーンIDの取得
	/// </summary>
	/// <returns>シーンID</returns>
	inline const SCENE_ID GetSceneID(void)
	{
		return sceneId_;
	}

	/// <summary>
	/// メインスクリーンの取得
	/// </summary>
	/// <returns>メインスクリーンのハンドルID</returns>
	inline int GetMainScreen(void) const
	{
		return mainScreen_;
	}

	/// <summary>
	/// シーン遷移中か
	/// </summary>
	/// <returns>
	/// true : シーン遷移中
	/// false : シーン遷移していない
	/// </returns>
	inline const bool IsSceneChanging(void) const
	{
		return isSceneChanging_;
	}

	/// <summary>
	/// デバッグ用クラスの取得
	/// </summary>
	/// <returns>デバッグ用クラスの参照</returns>
	inline DebugUtility& GetDebugUtility(void) const
	{
		return *debugUtility_;
	}

	/// <summary>
	/// カメラクラスの取得
	/// </summary>
	/// <returns>カメラクラスの参照</returns>
	inline Camera& GetCamera(void) const
	{
		return *camera_;
	}

private:

	friend class Singleton<SceneManager>;

	//デバッグ用
	std::unique_ptr<DebugUtility> debugUtility_;

	//フェード
	std::unique_ptr<Fader> fader_;

	//描画するシーン
	std::list<std::unique_ptr<SceneBase>> scenes_;

	//カメラ
	std::unique_ptr<Camera> camera_;

	//シーンID
	SCENE_ID sceneId_;

	//待機中のシーンID
	SCENE_ID waitSceneId_;

	//シーン遷移中判定
	bool isSceneChanging_;

	//メインスクリーン
	int mainScreen_;

	SceneManager(void);
	virtual ~SceneManager(void) override;

	//初期化
	void Init(void) override;

	//シーン遷移
	void DoChangeScene(const SCENE_ID sceneId);

	//シーン遷移
	std::unordered_map<SCENE_ID, std::function<void(void)>> sceneChanges_;
	void SceneChange_None(void);		//None
	void SceneChange_Title(void);		//Title
	void SceneChange_Game(void);		//Game
	void SceneChange_Clear(void);		//Clear
	void SceneChange_Failed(void);		//Failed

	//フェード
	void Fade(void);

	//3D用の初期設定
	void Init3D(void);

	//シーン遷移の登録
	void InitSceneChange(void);

	//シーンがエラー
	const bool IsSceneError(void) const;

	//指定されたシーンがエラー
	const bool IsSceneError(const SCENE_ID scnId) const;

	//オブジェクトやUIを削除
	void ClearActors(void);
};