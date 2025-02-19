#pragma once
#include "../ActorBase.h"

/// <summary>
/// スカイドーム
/// </summary>
class SkyDome :
    public ActorBase
{

public:

	SkyDome(void);
	virtual ~SkyDome(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void) override;

protected:

private:

	//初期化時の読込処理
	virtual void InitLoad(void) override;

	//モデル制御情報の初期化
	virtual void InitTransform(void) override;

	//衝突判定の初期化
	virtual void InitCollider(void) override;

	//レンダーの初期化
	virtual void InitRender(void) override;

	//全ての初期化が終了時に行われる初期化処理
	virtual void InitEnd(void) override;

	//isHitUpdate_の登録
	virtual void InitIsHitUpdate(void) override;

};

