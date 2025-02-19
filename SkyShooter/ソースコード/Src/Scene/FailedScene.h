#pragma once
#include "SceneBase.h"
class FailedScene :
    public SceneBase
{
public:

	FailedScene(void);
	virtual ~FailedScene(void) override;

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

	//音の初期化
	virtual void InitSound(void) override;

	//デバッグ用描画
	void DrawDebugUI(void);
	void DrawDebug3D(void);
};

