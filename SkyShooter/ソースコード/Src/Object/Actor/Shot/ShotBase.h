#pragma once
#include <memory>
#include "../ActorBase.h"

class TimeManager;
class EffectController;

/// <summary>
/// 弾の基底クラス
/// </summary>
class ShotBase
	: public ActorBase
{

public:

	ShotBase(void) = default;
	virtual ~ShotBase(void) override = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="trans">モデル制御情報</param>
	explicit ShotBase(const Transform& trans);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void) override final;

	/// <summary>
	/// ターゲットの指定
	/// </summary>
	/// <param name="targetPos">目標</param>
	void SetTargetPos(const VECTOR& targetPos);
	
	/// <summary>
	/// 弾発射位置の設定
	/// </summary>
	/// <param name="shotPos">弾発射開始位置</param>
	void SetShotPos(const VECTOR& shotPos);

	///// <summary>
	///// 弾の生存判定
	///// </summary>
	///// <returns>
	///// true : 生存
	///// false : 破壊
	///// </returns>
	//virtual const bool IsAlive(void) const override;
	
	//return aliveCnt_ > 0.0f;

	/// <summary>
	/// 破壊処理
	/// </summary>
	virtual void Destroy(void) = 0;

protected:

	//時間
	TimeManager& timeMng_;

	//弾の生存時間
	float aliveCnt_;

	//デルタタイム
	float deltaTime_;

	//ターゲット座標
	VECTOR targetPos_;

	//弾破壊時に行われる関数
	std::function<void(void)> destroyFunc_;

	/// <summary>
	/// 弾の発射位置の設定
	/// </summary>
	virtual void InitShotStartPos(void) = 0;


	virtual void UpdateState(void) = 0;

	/// <summary>
	/// モデル制御の更新
	/// </summary>
	virtual void UpdateTransform(void);

	/// <summary>
	/// 全ての更新処理終了後に実行されるもの
	/// </summary>
	virtual void UpdateEnd(void);

	/// <summary>
	/// 移動
	/// </summary>
	virtual void Move(void) = 0;

	//衝突時に行われる関数
	virtual void IsHit_Enemy(void);		//敵と衝突
	virtual void IsHit_Player(void);	//プレイヤーと衝突

	virtual void InitIsHitUpdate(void) override;		

	//敵と衝突した際の処理
	virtual void HitEnemy(void);

	//自機と衝突した際の処理
	virtual void HitPlayer(void);

	//破壊されているか
	virtual const bool IsDestroy(void) const;

private:

};

