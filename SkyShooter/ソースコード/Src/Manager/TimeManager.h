#pragma once

#include <chrono>
#include <string>
#include "../Utility/TimeUnit.h"
#include "../Singleton.h"

/// <summary>
/// デルタタイム等時間の管理
/// </summary>
class TimeManager 
	: public Singleton<TimeManager>
{
public:

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// デルタタイムのリセット
	/// </summary>
	void ResetDeltaTime(void);

	/// <summary>
	/// 時間の停止
	/// </summary>
	void SetStop(bool isStop);

	/// <summary>
	/// 時間停止中か
	/// </summary>
	const bool GetIsStop(void);

	/// <summary>
	/// 時間の取得
	/// </summary>
	const float GetTime(void);

	/// <summary>
	/// デルタタイムの取得
	/// </summary>
	const float GetDeltaTime(void);

	/// <summary>
	/// 現在の計測時間を分秒ミリ秒で取得
	/// </summary>
	/// <returns>Time_Unit</returns>
	const Time_Unit GetTime_Unit(void);

	/// <summary>
	/// 指定した数値を分秒ミリ秒に変換
	/// </summary>
	/// <returns>Time_Unit</returns>
	const Time_Unit GetTime_Unit(const float time);

private:

	friend class Singleton<TimeManager>;

	//1f前の時間
	std::chrono::system_clock::time_point preTime_;

	//時間
	float time_;

	//デルタタイム
	float deltaTime_;

	//時間停止中か
	bool isStop_;

	TimeManager(void);
	virtual ~TimeManager(void) override;

	//デルタタイム計測
	void MeasureDeltaTime(void);

};

