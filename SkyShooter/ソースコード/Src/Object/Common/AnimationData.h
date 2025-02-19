#pragma once

const float defaultAnimataionSpeed = 30.0f;

/// <summary>
/// ブレンドレート
/// </summary>
struct BlendRate
{
	float maxRate;		//最大レート?
	float rate;			//レート

	BlendRate(void)
		:
		maxRate(1.0f),
		rate(maxRate)
	{};

	BlendRate(const float rateVal,const float maxRate = 1.0f)
		:
		maxRate(maxRate),
		rate(rateVal)
	{};


	const bool IsEndBlend(void) const
	{
		return rate > 0.0f;
	}

};

/// <summary>
/// アニメーション時間関連
/// </summary>
struct AnimTime
{
	float animStartTime;
	float animEndTime;
	float animStep;
	float animTotalTime;
	float animSpeed;

	AnimTime(void)
		:
		animStartTime(0.0f),
		animEndTime(0.0f),
		animStep(animStartTime),
		animTotalTime(animEndTime),
		animSpeed(defaultAnimataionSpeed)
	{};

	AnimTime(const float speed)
		:
		animStartTime(0.0f),
		animEndTime(0.0f),
		animStep(animStartTime),
		animTotalTime(animEndTime),
		animSpeed(speed)
	{};

	AnimTime(const float start, const float end, const float speed)
		:
		animStartTime(start),
		animEndTime(end),
		animStep(animStartTime),
		animTotalTime(CalculateTotalTime()),
		animSpeed(speed)
	{};

	void StepReset(void)
	{
		animStep = animStartTime;
	}

	/// <summary>
	/// 逆再生時のアニメーションカウンターのリセット
	/// </summary>
	void StepResetRev(void)
	{
		animStep = animEndTime;
	}

	/// <summary>
	/// 終了時間と再生時間から総再生時間を計算
	/// </summary>
	const float CalculateTotalTime(void)
	{
		return animEndTime - animStartTime;
	}

	/// <summary>
	/// アニメーションの再生カウントが終了時間か
	/// </summary>
	const bool IsAnimationTimeEnd(void) const
	{
		return animStep >= animEndTime;
	}

};

/// <summary>
/// アニメーション再生の設定
/// </summary>
struct AnimPlayOption
{
	bool isStop;				//アニメーションの停止
	bool isLoop;				//アニメーションのループ
	bool isReverse;				//アニメーションの逆再生
	bool isPlayAnimationPart;	//アニメーションの一部を再生

	AnimPlayOption(void)
		:
		isStop(false),
		isLoop(false),
		isReverse(false),
		isPlayAnimationPart(false)
	{};

	AnimPlayOption(const bool loop, const bool reverse = false, const bool animationPart = false)
		:
		isStop(false),
		isLoop(loop),
		isReverse(reverse),
		isPlayAnimationPart(animationPart)
	{};

	const bool IsLoop(void) const
	{
		return isLoop;
	}

	const bool IsReverse(void) const
	{
		return isReverse;
	}

	const bool IsPlayAnimationPart(void) const
	{
		return isPlayAnimationPart;
	}

};

/// <summary>
/// アニメーションデータ
/// </summary>
struct AnimData
{
	int animHandle;		//アニメーションのハンドル
	int attachNo;		//アニメーションのアタッチ番号
	int animNo;			//アニメーション番号

	bool isPriority;	//アニメーションの優先

	AnimTime animTime;				//アニメーション時間関連
	AnimPlayOption animPlayOption;	//アニメーション再生の設定
	BlendRate blendRate;			//ブレンドレート

	AnimData(void)
		:
		animHandle(-1),
		attachNo(-1),
		animNo(-1),
		isPriority(false),
		animTime(AnimTime()),
		animPlayOption(AnimPlayOption()),
		blendRate(BlendRate())
	{};

	AnimData(const int animhandle, const int animNo, const AnimTime& animTime, const AnimPlayOption& option)
		:
		animHandle(animhandle),
		attachNo(-1),
		animNo(animNo),
		isPriority(false),
		animTime(animTime),
		animPlayOption(option),
		blendRate(BlendRate())
	{};

	const bool IsLoop(void) const
	{
		return animPlayOption.IsLoop();
	}

	/// <summary>
	/// アニメーションを逆再生するか
	/// </summary>
	/// <returns>
	/// true : 逆再生する
	/// false : 逆再生しない
	/// </returns>
	const bool IsReverse(void) const
	{
		return animPlayOption.IsReverse();
	}

	/// <summary>
	/// アニメーションを一部再生するか
	/// </summary>
	/// <returns>
	/// true : 一部再生する
	/// false : 全て再生する
	/// </returns>
	const bool IsPlayAnimationPart(void) const
	{
		return animPlayOption.IsPlayAnimationPart();
	}

	/// <summary>
	/// アニメーションの再生カウントが終了時間か
	/// </summary>
	/// <returns>
	/// true : 終了時間
	/// false : 再生中
	/// </returns>
	const bool IsAnimationEnd(void) const
	{
		return animTime.IsAnimationTimeEnd();
	}

	void AnimStepReset(void)
	{
		animTime.StepReset();
	}

	/// <summary>
	/// アニメーションがアタッチされているか
	/// </summary>
	/// <returns>
	/// true : アタッチされている
	/// false : アタッチされていない
	/// </returns>
	const bool IsAttachAnim(void) const
	{
		return attachNo != -1;
	}

};
