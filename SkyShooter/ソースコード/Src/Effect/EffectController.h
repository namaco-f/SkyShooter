#pragma once
#include <unordered_map>
#include <string>
#include "../Object/Common/Transform.h"

/// <summary>
/// エフェクト座標制御情報
/// </summary>
struct EffectTransform
{
	VECTOR pos;					//座標
	VECTOR localPos;			//ローカル座標
	VECTOR scale;				//大きさ
	Quaternion quaRot;			//回転
	Quaternion localQuaRot;		//ローカル回転

	EffectTransform(void)
		:
		pos{},
		localPos{},
		scale({1.0f,1.0f,1.0f}),
		quaRot{},
		localQuaRot{}
	{};
};

/// <summary>
/// エフェクト再生用情報
/// </summary>
struct EffectPlayData
{

	int startTime;			//再生開始時間			
	int endTime;			//再生終了時間
	float step;				//カウント用
	bool isPartPlay;		//一部再生を使用するか
	bool isLoop;			//ループするか
	bool PlayInMiddle;		//途中から再生し、最後まで再生するか

	EffectPlayData(void)
		:
		startTime(0),
		endTime(0),
		step(0.0f),
		isPartPlay(false),
		isLoop(false),
		PlayInMiddle(false)
	{};

	/// <summary>
	/// 一部再生用のコンストラクタ
	/// </summary>
	/// <param name="start">開始する時間</param>
	/// <param name="end">終了する時間</param>
	/// <param name="isLoop">ループするか</param>
	explicit EffectPlayData(const int start,const int end = -1,const bool isLoop = false)
		:
		startTime(start),
		endTime(end),
		step(0.0f),
		isPartPlay(true),
		isLoop(isLoop),
		PlayInMiddle(IsSetPlayInMiddle())
	{};

	/// <summary>
	/// 再生が終了しているか
	/// </summary>
	/// <returns>
	/// true:終了している
	/// false:終了していない
	/// </returns>
	const bool IsEndTime(void) const
	{
		return endTime < startTime + (int)step;
	}

	/// <summary>
	/// 途中再生し、最後まで再生するか	
	/// </summary>
	const bool IsSetPlayInMiddle(void) const
	{
		//再生終了時間が設定されているか確認
		//true : 途中で再生終了
		//false : 最後まで再生
		return endTime != -1 ? false : true;
	}
};

/// <summary>
/// エフェクト情報
/// </summary>
struct EffectData
{
	int resId;							//エフェクトハンドル
	int playId;							//エフェクトの再生用ハンドル
	EffectPlayData effectPlayData;		//エフェクト再生用の情報
	EffectTransform effectTransform;	//エフェクト座標制御情報

	EffectData(void)
		:
		resId(-1),
		playId(-1),
		effectPlayData(EffectPlayData()),
		effectTransform(EffectTransform())
	{};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="transform">エフェクト座標制御情報</param>
	/// <param name="effectPlayData">エフェクト再生用情報</param>
	/// <param name="PartPlay">一部再生するか</param>
	explicit EffectData(
		const EffectTransform& transform,
		const EffectPlayData& effectPlayData = EffectPlayData(),
		const bool PartPlay = false)
		:
		resId(-1),
		playId(-1),
		effectPlayData(effectPlayData),
		effectTransform(transform)
	{};

	/// <summary>
	/// 座標を取得
	/// </summary>
	const VECTOR& GetPos(void) const
	{
		return effectTransform.pos;
	}

	/// <summary>
	/// 途中から再生し、最後まで再生するか
	/// </summary>
	const bool IsPlayInMiddle(void) const
	{
		return effectPlayData.PlayInMiddle;
	}

	/// <summary>
	/// 一部再生か
	/// </summary>
	const bool IsPartPlayEffect(void) const
	{
		//一部再生、最後まで再生しない
		return effectPlayData.isPartPlay && !IsPlayInMiddle();
	}
};

/// <summary>
/// エフェクトの描画制御
/// </summary>
class EffectController
{

public:

	EffectController(void);
	virtual ~EffectController(void);

	/// <summary>
	/// 一部再生時のみ更新処理
	/// </summary>
	virtual void Update(const std::string& effectKeyName);

	/// <summary>
	/// エフェクト追加
	/// </summary>
	/// <param name="effectKeyName">エフェクトの名前</param>
	/// <param name="handleId">エフェクトのハンドルID</param>
	/// <param name="effectTransform">エフェクトの制御情報</param>
	virtual void Add(
		const std::string& effectKeyName, 
		const int handleId, 
		const EffectTransform& effectTransform,
		const EffectPlayData& effectPlayData = EffectPlayData()
	);

	/// <summary>
	/// 指定したエフェクトの描画
	/// </summary>
	/// <param name="effectKeyName">エフェクトの名前</param>
	virtual void Draw(
		const std::string& effectKeyName);

	/// <summary>
	/// 指定したエフェクトを停止
	/// </summary>
	/// <param name="effectKeyName">エフェクトの名前</param>
	virtual void Stop(const std::string& effectKeyName);

	/// <summary>
	/// 指定したエフェクトを停止
	/// </summary>
	/// <param name="effectKeyName">エフェクトの名前</param>
	virtual const bool IsEffectEnd(const std::string& effectKeyName);

	/// <summary>
	/// エフェクトが追従している座標情報を更新
	/// </summary>
	/// <param name="effectKeyName">エフェクトの名前</param>
	/// <param name="transform">エフェクトが追従している対象の制御情報</param>
	virtual void FollowTransformUpdate(
		const std::string& effectKeyName,
		const Transform& transform);

private:

	//エフェクト情報
	std::unordered_map<std::string, EffectData> effectsData_;

	//エフェクトを停止
	virtual void StopEffect3D(const int playId);

	//座標をエフェクトに反映
	virtual void SetPosPlayEffect3D(const int playId, const VECTOR& pos);
	
	//回転をエフェクトに反映
	virtual void SetRotatePlayEffect3D(const int playId, const Quaternion& quaRot,const Quaternion& quaRotLocal);
	
	//大きさをエフェクトに反映
	virtual void SetScalePlayEffect3D(const int playId, const VECTOR& scale);

	//エフェクトが登録されていない
	virtual const bool IsEffectNameKeyError(const std::string& effectKeyName) const;

	//指定のエフェクトが部分再生ではない場合の再生処理
	virtual void NotPartPlay(EffectData& effectData);

	//指定のエフェクトが部分再生の場合の再生処理
	virtual void PartPlay(EffectData& effectData);

	/// <summary>
	/// エフェクトの座標を更新
	/// </summary>
	/// <param name="effectData">エフェクト情報</param>
	virtual void FollowPos(EffectData& effectData);
};

