
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
	//�A�j���[�V�����̍Đ�
	PlayAnimation(CalculateBlendRate());

}

void AnimationController::AddAnimationData(const std::string& animKey, const AnimData& animData)
{
	if(
		!animationDatasMap_.empty() 
		&& animationDatasMap_.contains(animKey)
		)
	{
		//�o�^��
		return;
	}

	animationDatasMap_.emplace(animKey, animData);
}

void AnimationController::Play(const std::string& animKey)
{
	////���݂̃A�j���[�V������1�O�̃A�j���[�V�����Ƃ��Ċi�[

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
		//�w�肵���A�j���[�V�������o�^����Ă��Ȃ�
		return;
	}

	preAnimName_ = nowAnimName_;
	nowAnimName_ = animKey;

	if (animationDatasMap_.contains(preAnimName_))
	{
		animationDatasMap_.at(preAnimName_).isPriority = false;
	}

	//�A�j���[�V�����̃A�^�b�`
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
		//�A�j���[�V�������o�^����Ă��Ȃ�
		return nullAnimData_;
	}

	return animationDatasMap_.at(animName);
}

const bool AnimationController::IsPlayAnim(const std::string& animName) const
{
	if (!animationDatasMap_.contains(animName))
	{
		//�A�j���[�V�������o�^����Ă��Ȃ�
		return false;
	}

	return !animationDatasMap_.at(animName).IsAnimationEnd();
}

void AnimationController::Attach(AnimData& anim)
{
	if (!anim.animPlayOption.isLoop)
	{
		//�t�Đ��̃J�E���g������
		anim.animTime.StepReset();
	}

	if (anim.attachNo != -1)
	{
		//���ɃA�^�b�`����Ă���
		anim.isPriority = true;
		return;
	}

	//�A�^�b�`������
	++attachNum_;

	//���f���ɃA�^�b�`���A�A�^�b�`�ԍ����擾
	anim.attachNo = MV1AttachAnim(modelHandle_, anim.animNo, anim.animHandle);

	//�D�悳����
	anim.isPriority = true;
	
#pragma region �A�j���[�V�����̐ݒ育�ƂɍĐ����Ԃ̐ݒ肪�ω�
	
	if (!anim.IsPlayAnimationPart())
	{
		//�ꕔ�̃A�j���[�V�����Đ��ł͂Ȃ�
		if (!anim.IsReverse())
		{
			//�t�Đ��ł͂Ȃ�
			//�A�^�b�`���ꂽ�A�j���[�V�����̑��Đ����Ԃ��擾
			anim.animTime.animTotalTime = MV1GetAttachAnimTotalTime(modelHandle_, anim.attachNo);
			anim.animTime.animEndTime = anim.animTime.animTotalTime;
		}
		else
		{
			//�t�Đ�
			anim.animTime.animTotalTime = MV1GetAttachAnimTotalTime(modelHandle_, anim.attachNo);
			anim.animTime.animStartTime = anim.animTime.animTotalTime;
		}
	}
	else
	{
		//�ꕔ�̃A�j���[�V�����Đ�
		if (!anim.IsReverse())
		{
			//�t�Đ��ł͂Ȃ�
			//�Đ����Ԃ��J�n���ԂƏI�����Ԃ���t�Z
			anim.animTime.animTotalTime = anim.animTime.CalculateTotalTime();
		}
		else
		{
			//�t�Đ�
			anim.animTime.animTotalTime = anim.animTime.CalculateTotalTime();

			//�J�n���ԂƏI�����Ԃ����
			float startTime = anim.animTime.animStartTime;
			anim.animTime.animStartTime = anim.animTime.animEndTime;
			anim.animTime.animEndTime = startTime;
		}
	}
#pragma endregion
}

void AnimationController::Detach(AnimData& anim)
{
	//�A�^�b�`������
	--attachNum_;

	//�A�j���[�V�����̃f�^�b�`
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
			//�A�j���[�V�������D����
			//�܂��́A�A�j���[�V�������A�^�b�`����Ă��Ȃ�
			continue;
		}

		animData.second.blendRate.rate -= deltaTime;

		if (!animData.second.blendRate.IsEndBlend())
		{
			//�u�����h���[�g��0�ȉ�
			//�A�j���[�V�����̃f�^�b�`
			Detach(animData.second);
		}

		//�D���Ԃ̃A�j���[�V�����ɓK������u�����h���[�g
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
			//�Đ�
			if (
				!animData.second.IsAnimationEnd() &&
				!animData.second.IsReverse())
			{
				//�Đ����ŁA�t�Đ��ł͂Ȃ�
				animData.second.animTime.animStep += deltaTime * animData.second.animTime.animSpeed;
			}

			if (
				animData.second.IsAnimationEnd()
				&& animData.second.IsLoop())
			{
				//���[�v����
				animData.second.AnimStepReset();
			}

			if (
				animData.second.IsAnimationEnd()
				&& !animData.second.IsLoop())
			{
				//���[�v����
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
