
#include <DxLib.h>
#include <algorithm>
#include "../Manager/TimeManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(const int& modelId)
	:
	modelHandle_(modelId),
	attachNum_(0),
	animationDatasMap_{},
	preAnimName_{},
	nowAnimName_{}
{
}

AnimationController::~AnimationController(void)
{
	animationDatasMap_.clear();
	preAnimName_.clear();
	nowAnimName_.clear();
}

void AnimationController::Update(void)
{
	//アニメーションの再生
	PlayAnimation(CalculateBlendRate());

}

void AnimationController::AddAnimationData(const std::string& animKey, const AnimData& animData)
{
	if(
		!animationDatasMap_.empty() 
		&& animationDatasMap_.contains(animKey)
		)
	{
		//登録済
		return;
	}

	animationDatasMap_.emplace(animKey, animData);
}

void AnimationController::Play(const std::string& animKey)
{
	////現在のアニメーションを1つ前のアニメーションとして格納

	if (nowAnimName_ == animKey)
	{
		if (animationDatasMap_.contains(nowAnimName_))
		{
			animationDatasMap_.at(nowAnimName_).animPlayOption.isStop = false;
		}
		return;
	}

	if (!animationDatasMap_.contains(animKey))
	{
		//指定したアニメーションが登録されていない
		return;
	}

	preAnimName_ = nowAnimName_;
	nowAnimName_ = animKey;

	if (animationDatasMap_.contains(preAnimName_))
	{
		animationDatasMap_.at(preAnimName_).isPriority = false;
	}

	//アニメーションのアタッチ
	Attach(animationDatasMap_.at(animKey));

}

void AnimationController::Stop(const std::string& animKey)
{
	if (!animationDatasMap_.contains(animKey))
	{
		return;
	}

	animationDatasMap_.at(animKey).animPlayOption.isStop = true;
}

void AnimationController::AllPlay(void)
{
	for (auto& animData : animationDatasMap_)
	{
		animData.second.animPlayOption.isStop = false;
	}
}

void AnimationController::AllStop(void)
{
	for (auto& animData : animationDatasMap_)
	{
		animData.second.animPlayOption.isStop = true;
	}
}

const AnimData& AnimationController::GetAnimationDatas(const std::string& animName) const
{
	if (!animationDatasMap_.contains(animName))
	{
		//アニメーションが登録されていない
		return nullAnimData_;
	}

	return animationDatasMap_.at(animName);
}

const bool AnimationController::IsPlayAnim(const std::string& animName) const
{
	if (!animationDatasMap_.contains(animName))
	{
		//アニメーションが登録されていない
		return false;
	}

	return !animationDatasMap_.at(animName).IsAnimationEnd();
}

void AnimationController::Attach(AnimData& anim)
{
	if (!anim.animPlayOption.isLoop)
	{
		//逆再生のカウント初期化
		anim.animTime.StepReset();
	}

	if (anim.attachNo != -1)
	{
		//既にアタッチされている
		anim.isPriority = true;
		return;
	}

	//アタッチ数増加
	++attachNum_;

	//モデルにアタッチし、アタッチ番号を取得
	anim.attachNo = MV1AttachAnim(modelHandle_, anim.animNo, anim.animHandle);

	//優先させる
	anim.isPriority = true;
	
#pragma region アニメーションの設定ごとに再生時間の設定が変化
	
	if (!anim.IsPlayAnimationPart())
	{
		//一部のアニメーション再生ではない
		if (!anim.IsReverse())
		{
			//逆再生ではない
			//アタッチされたアニメーションの総再生時間を取得
			anim.animTime.animTotalTime = MV1GetAttachAnimTotalTime(modelHandle_, anim.attachNo);
			anim.animTime.animEndTime = anim.animTime.animTotalTime;
		}
		else
		{
			//逆再生
			anim.animTime.animTotalTime = MV1GetAttachAnimTotalTime(modelHandle_, anim.attachNo);
			anim.animTime.animStartTime = anim.animTime.animTotalTime;
		}
	}
	else
	{
		//一部のアニメーション再生
		if (!anim.IsReverse())
		{
			//逆再生ではない
			//再生時間を開始時間と終了時間から逆算
			anim.animTime.animTotalTime = anim.animTime.CalculateTotalTime();
		}
		else
		{
			//逆再生
			anim.animTime.animTotalTime = anim.animTime.CalculateTotalTime();

			//開始時間と終了時間を入替
			float startTime = anim.animTime.animStartTime;
			anim.animTime.animStartTime = anim.animTime.animEndTime;
			anim.animTime.animEndTime = startTime;
		}
	}
#pragma endregion
}

void AnimationController::Detach(AnimData& anim)
{
	//アタッチ数減少
	--attachNum_;

	//アニメーションのデタッチ
	MV1DetachAnim(modelHandle_, anim.attachNo);

	anim.blendRate.rate = 0.0f;
	anim.attachNo = -1;
}

const float AnimationController::CalculateBlendRate(void)
{
	const float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	float rate = 1.0f;
	for (auto& animData : animationDatasMap_)
	{
		if (animData.second.isPriority 
			|| !animData.second.IsAttachAnim() 
			|| animData.second.animPlayOption.isStop)
		{
			//アニメーションが優先状態
			//または、アニメーションがアタッチされていない
			continue;
		}

		animData.second.blendRate.rate -= deltaTime;

		if (!animData.second.blendRate.IsEndBlend())
		{
			//ブレンドレートが0以下
			//アニメーションのデタッチ
			Detach(animData.second);
		}

		//優先状態のアニメーションに適応するブレンドレート
		rate -= animData.second.blendRate.rate;
	};

	return rate;
}

void AnimationController::PlayAnimation(const float blendRate)
{
	const float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	for (auto& animData : animationDatasMap_)
	{
		if (animData.second.animPlayOption.isStop)
		{
			continue;
		}

		if (!animData.second.IsPlayAnimationPart())
		{
			//再生
			if (
				!animData.second.IsAnimationEnd() &&
				!animData.second.IsReverse())
			{
				//再生中で、逆再生ではない
				animData.second.animTime.animStep += deltaTime * animData.second.animTime.animSpeed;
			}

			if (
				animData.second.IsAnimationEnd()
				&& animData.second.IsLoop())
			{
				//ループ処理
				animData.second.AnimStepReset();
			}

			if (
				animData.second.IsAnimationEnd()
				&& !animData.second.IsLoop())
			{
				//ループ処理
				animData.second.animTime.animStep = animData.second.animTime.animEndTime;
			}
		}

		MV1SetAttachAnimTime(modelHandle_, animData.second.attachNo, animData.second.animTime.animStep);

		if (animData.second.isPriority)
		{
			animData.second.blendRate.rate = blendRate;
		}
		MV1SetAttachAnimBlendRate(
			modelHandle_,
			animData.second.attachNo,
			animData.second.blendRate.rate
		);
	}
}
