#pragma once

#include <list>
#include <unordered_map>
#include <functional>
#include "../../../Common/Transform.h"
#include "../../Shot/Shot_CreateData.h"

class TimeManager;
class ShotManager;
class SoundResourcesManager;
class SoundController;

enum class PlayerShotPattern
{
	None,
	Standard,
	Missile,
	Ult,
	CounterShot,
	MAX
};

class PlayerShotTrigger
{
public:

	PlayerShotTrigger(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="trans">
	/// プレイヤーのモデル制御情報
	/// </param>
	explicit PlayerShotTrigger(const Transform& trans);
	virtual ~PlayerShotTrigger(void);

	/// <summary>
	/// 弾の種別を登録
	/// </summary>
	/// <param name="shotPattern">弾の種別</param>
	/// <param name="shotData">弾トリガーの情報</param>
	void AddShotPattern(const PlayerShotPattern& shotPattern,const ShotData& shotData);
	
	/// <summary>
	/// 指定した弾のトリガー情報を更新
	/// </summary>
	/// <param name="shotPattern">弾の種別</param>
	/// <param name="shotData">弾トリガーの情報</param>
	void UpdateShotPattern(const PlayerShotPattern& shotPattern, const ShotData& shotData);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(int& mp);

	/// <summary>
	/// プレイヤーのモデル制御情報を更新
	/// </summary>
	void UpdateTransform(const Transform& transform);

	/// <summary>
	/// 指定した種類の弾を生成開始
	/// </summary>
	void CreateShot(const PlayerShotPattern& shotPattern);

	/// <summary>
	/// 弾を発射可能か
	/// </summary>
	const bool AbleTriggerShot(void) const;

	/// <summary>
	/// トリガーがクールタイム中
	/// </summary>
	const bool IsTriggerCoolTime(void) const;

	const ShotData& GetShotData(const PlayerShotPattern& shotPattern) const
	{
		if (!shotDataMap_.contains(shotPattern))
		{
			return shotDataNull_;
		}

		return shotDataMap_.at(shotPattern);
	}

private:

	TimeManager& timeMng_;
	ShotManager& shotMng_;
	SoundResourcesManager& resMngSound_;

	Transform playerTransform_;
	ShotData createShotData_;

	const ShotData shotDataNull_ = ShotData();

	std::unordered_map<PlayerShotPattern, ShotData> shotDataMap_;
	std::unique_ptr<SoundController> soundController_;

	//クールタイムカウント用
	float coolTimeStep_;

	void DrawDebug(void);

	//弾生成時に行われる初期化
	std::unordered_map<PlayerShotPattern, std::function<void(void)>> createShotInit_;
	void CreateShotInit_None(void);
	void CreateShotInit_Standard(void);
	void CreateShotInit_Missile(void);
	void CreateShotInit_CounterShot(void);
	
	//弾生成時の関数オブジェクト
	std::function<void(void)> createShot_;
	void CreateShot_None(void);
	void CreateShot_Standard(void);
	void CreateShot_Missile(void);
	void CreateShot_CounterShot(void);
	//void CreateShot_Ult(void);

	////弾生成時に行われる初期化を登録
	void AddCreateShotInit(void);

	//発射音追加
	void AddShotSound(const std::string& soundName);

	//発射終了
	void CreateShotEnd(void);

};

