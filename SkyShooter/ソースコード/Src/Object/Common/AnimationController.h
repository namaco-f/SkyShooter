#pragma once

#include <string>
#include <unordered_map>
#include "AnimationData.h"

/// <summary>
/// モデルのアニメーション操作
/// </summary>
class AnimationController
{

public:

	AnimationController(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelId">アニメーションの制御をしたいモデルのハンドルID</param>
	explicit AnimationController(const int& modelId);
	~AnimationController(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// アニメーションの追加
	/// </summary>
	/// <param name="animKey">アニメーションのキー</param>
	/// <param name="animData">アニメーションデータ</param>
	void AddAnimationData(const std::string& animKey,const AnimData& animData);

	/// <summary>
	/// 指定したアニメーションの再生開始
	/// </summary>
	/// <param name="animKey">指定するアニメーションのキー</param>
	void Play(const std::string& animKey);

	/// <summary>
	/// 指定したアニメーションの停止
	/// </summary>
	/// <param name="animKey">指定するアニメーションのキー</param>
	void Stop(const std::string& animKey);

	/// <summary>
	/// アタッチされている全てのアニメーションの再生
	/// </summary>
	void AllPlay(void);

	/// <summary>
	/// アタッチされている全てのアニメーションの再生
	/// </summary>
	void AllStop(void);
	


	/// <summary>
	/// 現在のアタッチされているアニメーションの数を取得
	/// </summary>
	/// <returns>attachNum_</returns>
	inline const int GetAnimAttachNum(void) const
	{
		return attachNum_;
	}

	/// <summary>
	/// 登録されているアニメーションデータを全て取得
	/// </summary>
	/// <returns>animationDatasMap_</returns>
	inline const std::unordered_map<std::string, AnimData>& GetAnimationDatas(void) const
	{
		return animationDatasMap_;
	}

	/// <summary>
	/// 指定したアニメーションのアニメーションデータを取得
	/// </summary>
	/// <returns>animationDatasMap_</returns>
	const AnimData& GetAnimationDatas(const std::string& animName) const;

	const bool IsPlayAnim(const std::string& animName) const;

private:

	//モデルのハンドル
	const int& modelHandle_;

	//空のアニメーションデータ
	const AnimData nullAnimData_ = AnimData();

	//アタッチしているアニメーションの数
	int attachNum_;

	//登録されたアニメーションデータ
	std::unordered_map<std::string, AnimData> animationDatasMap_;

	//1つ前のアニメーションの名前
	std::string preAnimName_;

	//現在のアニメーションの名前
	std::string nowAnimName_;

	//アタッチ操作
	void Attach(AnimData& anim);

	//デタッチ操作
	void Detach(AnimData& anim);

	//ブレンドレートの計算処理
	const float CalculateBlendRate(void);

	//アニメーションの再生処理
	void PlayAnimation(const float blendRate);

};

