#include <DxLib.h>
#include "../Application.h"
#include "Fader.h"

using namespace std;

const int SPEED_ALPHA = 5;		// �t�F�[�h���i�ޑ���

Fader::Fader(void)
	:
	state_(State::NONE),
	alpha_(0),
	isPreEnd_(true),
	isEnd_(true),
	changeStates_{},
	stateUpdate_{},
	stateDraw_{}
{
	InitChangeState();
}

Fader::~Fader(void)
{
	changeStates_.clear();
}

Fader::State Fader::GetState(void) const
{
	return state_;
}

bool Fader::IsEnd(void) const
{
	return isEnd_;
}

void Fader::Update(void)
{
	if (isEnd_)
	{
		//�t�F�[�h���I�����Ă���
		return;
	}

	if (!stateUpdate_)
	{
		//�֐��I�u�W�F�N�g�������N�؂�
		return;
	}

	stateUpdate_();
}

void Fader::Draw(void)
{
	if (!stateDraw_)
	{
		//�֐��I�u�W�F�N�g�������N�؂�
		return;
	}

	stateDraw_();
}

void Fader::InitChangeState(void)
{
	changeStates_.emplace(State::NONE, bind(&Fader::ChangeState_None, this));
	changeStates_.emplace(State::FADE_OUT, bind(&Fader::ChangeState_FadeOut, this));
	changeStates_.emplace(State::FADE_IN, bind(&Fader::ChangeState_FadeIn, this));
}

void Fader::ChangeState(const State state)
{
	if (!changeStates_.contains(state))
	{
		//��ԑJ�ڂ��o�^����Ă��Ȃ�
		return;
	}

	state_ = state;

	const auto& changeState = changeStates_.at(state_);

	if (!changeState)
	{
		//�֐��I�u�W�F�N�g�������N�؂�
		return;
	}

	changeState();
}

void Fader::ChangeState_None(void)
{
	stateUpdate_ = bind(&Fader::StateUpdate_None, this);
	stateDraw_ = bind(&Fader::StateDraw_None, this);
}

void Fader::ChangeState_FadeOut(void)
{
	isPreEnd_ = false;
	isEnd_ = false;

	stateUpdate_ = bind(&Fader::StateUpdate_FadeOut, this);
	stateDraw_ = bind(&Fader::StateDraw_FadeOut, this);
}

void Fader::ChangeState_FadeIn(void)
{
	isPreEnd_ = false;
	isEnd_ = false;

	stateUpdate_ = bind(&Fader::StateUpdate_FadeIn, this);
	stateDraw_ = bind(&Fader::StateDraw_FadeIn, this);
}

void Fader::StateUpdate_None(void)
{
}

void Fader::StateUpdate_FadeOut(void)
{
	alpha_ += SPEED_ALPHA;
	if (alpha_ > 255)
	{
		// �t�F�[�h�I��
		alpha_ = 255;
		if (isPreEnd_)
		{
			// 1�t���[����(Draw��)�ɏI���Ƃ���
			isEnd_ = true;
		}
		isPreEnd_ = true;
	}
}

void Fader::StateUpdate_FadeIn(void)
{
	alpha_ -= SPEED_ALPHA;
	if (alpha_ < 0)
	{
		// �t�F�[�h�I��
		alpha_ = 0;
		if (isPreEnd_)
		{
			// 1�t���[����(Draw��)�ɏI���Ƃ���
			isEnd_ = true;
		}
		isPreEnd_ = true;
	}
}

void Fader::StateDraw_None(void)
{
	//�����Ȃ�
}

void Fader::StateDraw_FadeOut(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x000000, true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fader::StateDraw_FadeIn(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x000000, true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
