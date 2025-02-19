#pragma once

const float defaultAnimataionSpeed = 30.0f;

/// <summary>
/// �u�����h���[�g
/// </summary>
struct BlendRate
{
	float maxRate;		//�ő僌�[�g?
	float rate;			//���[�g

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
/// �A�j���[�V�������Ԋ֘A
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
	/// �t�Đ����̃A�j���[�V�����J�E���^�[�̃��Z�b�g
	/// </summary>
	void StepResetRev(void)
	{
		animStep = animEndTime;
	}

	/// <summary>
	/// �I�����ԂƍĐ����Ԃ��瑍�Đ����Ԃ��v�Z
	/// </summary>
	const float CalculateTotalTime(void)
	{
		return animEndTime - animStartTime;
	}

	/// <summary>
	/// �A�j���[�V�����̍Đ��J�E���g���I�����Ԃ�
	/// </summary>
	const bool IsAnimationTimeEnd(void) const
	{
		return animStep >= animEndTime;
	}

};

/// <summary>
/// �A�j���[�V�����Đ��̐ݒ�
/// </summary>
struct AnimPlayOption
{
	bool isStop;				//�A�j���[�V�����̒�~
	bool isLoop;				//�A�j���[�V�����̃��[�v
	bool isReverse;				//�A�j���[�V�����̋t�Đ�
	bool isPlayAnimationPart;	//�A�j���[�V�����̈ꕔ���Đ�

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
/// �A�j���[�V�����f�[�^
/// </summary>
struct AnimData
{
	int animHandle;		//�A�j���[�V�����̃n���h��
	int attachNo;		//�A�j���[�V�����̃A�^�b�`�ԍ�
	int animNo;			//�A�j���[�V�����ԍ�

	bool isPriority;	//�A�j���[�V�����̗D��

	AnimTime animTime;				//�A�j���[�V�������Ԋ֘A
	AnimPlayOption animPlayOption;	//�A�j���[�V�����Đ��̐ݒ�
	BlendRate blendRate;			//�u�����h���[�g

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
	/// �A�j���[�V�������t�Đ����邩
	/// </summary>
	/// <returns>
	/// true : �t�Đ�����
	/// false : �t�Đ����Ȃ�
	/// </returns>
	const bool IsReverse(void) const
	{
		return animPlayOption.IsReverse();
	}

	/// <summary>
	/// �A�j���[�V�������ꕔ�Đ����邩
	/// </summary>
	/// <returns>
	/// true : �ꕔ�Đ�����
	/// false : �S�čĐ�����
	/// </returns>
	const bool IsPlayAnimationPart(void) const
	{
		return animPlayOption.IsPlayAnimationPart();
	}

	/// <summary>
	/// �A�j���[�V�����̍Đ��J�E���g���I�����Ԃ�
	/// </summary>
	/// <returns>
	/// true : �I������
	/// false : �Đ���
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
	/// �A�j���[�V�������A�^�b�`����Ă��邩
	/// </summary>
	/// <returns>
	/// true : �A�^�b�`����Ă���
	/// false : �A�^�b�`����Ă��Ȃ�
	/// </returns>
	const bool IsAttachAnim(void) const
	{
		return attachNo != -1;
	}

};
