
#include "../Manager/TimeManager.h"
#include "../Manager/Input/InputController.h"
#include "../Object/Actor/Charactor/Player/PlayerBase.h"
#include "PlayerInputComponent.h"

using namespace std;

PlayerInputComponent::PlayerInputComponent(void)
	: 
	inputController_(make_unique<InputController>()),
	deltaTime_(0.0f)
{
	deltaTime_ = TimeManager::GetInstance().GetDeltaTime();
}

PlayerInputComponent::~PlayerInputComponent(void)
{
}

void PlayerInputComponent::Update(PlayerBase& player)
{
	deltaTime_ = TimeManager::GetInstance().GetDeltaTime();

	if (inputController_->IsTriggered(ACTION::PLAYER_LOCK_SWITCH))
	{
		//���b�N�I���̐ؑ�
		player.NoticeAction(ACTION::PLAYER_LOCK_SWITCH);
	}

	if (inputController_->IsPressed(ACTION::PLAYER_SHIELD)
		&& !player.IsShieldCounter())
	{
		//�V�[���h��Ԃ�ʒm
		player.NoticeAction(ACTION::PLAYER_SHIELD);

		//�V�[���h���͑��A�N�V�����ɐ؂�ւ��Ȃ�
		return;
	}

	if (player.IsAbleShield())
	{
		//�V�[���h���
		return;
	}

	//�ړ��֘A
	InputUpdateMove(player);

	//�U���֘A
	InputUpdateAttack(player);
}

void PlayerInputComponent::InputUpdateMove(PlayerBase& player)
{
	if (inputController_->IsPressed(ACTION::MOVE_FORWARD))
	{
		//�O���ړ�
		player.NoticeAction(ACTION::MOVE_FORWARD);
	}

	if (inputController_->IsPressed(ACTION::MOVE_BACK))
	{
		//����ړ�
		player.NoticeAction(ACTION::MOVE_BACK);
	}

	if (inputController_->IsPressed(ACTION::MOVE_LEFT))
	{
		//���ړ�
		player.NoticeAction(ACTION::MOVE_LEFT);
	}

	if (inputController_->IsPressed(ACTION::MOVE_RIGHT))
	{
		//�E�ړ�
		player.NoticeAction(ACTION::MOVE_RIGHT);
	}

	if (inputController_->IsPressed(ACTION::MOVE_UP))
	{
		//��ړ�
		player.NoticeAction(ACTION::MOVE_UP);
	}

	if (inputController_->IsPressed(ACTION::PLAYER_DASH))
	{
		//�_�b�V��
		player.ReceiveCommonMessage(Common_Message::DASH);
	}

	if (inputController_->IsReleased(ACTION::PLAYER_DASH))
	{
		//�_�b�V���L�[��������A�ʏ�ړ��ɐؑ�
		player.ReceiveCommonMessage(Common_Message::DASH_END);
	}

	if (IsNotMoving())
	{
		//�ړ����Ă��Ȃ�
		player.NoticeAction(ACTION::PLAYER_IDLE);
	}
}

void PlayerInputComponent::InputUpdateAttack(PlayerBase& player)
{
	if (player.IsUltAttackAble())
	{
		//�K�E�Z���͑��̍U�������Ȃ�
		return;
	}

	if (inputController_->IsTriggered(ACTION::PLAYER_ULTATTACK))
	{
		//�K�E�Z
		player.NoticeAction(ACTION::PLAYER_ULTATTACK);
		return;
	}

	if (inputController_->IsPressed(ACTION::PLAYER_ATTACK))
	{
		//�ʏ�e
		player.NoticeAction(ACTION::PLAYER_ATTACK);
	}

	if (inputController_->IsPressed(ACTION::PLAYER_ATTACK2))
	{
		//�ǔ��e
		player.NoticeAction(ACTION::PLAYER_ATTACK2);
	}
}

const bool PlayerInputComponent::IsNotMoving(void)
{
	//�ǂ̈ړ��L�[��������Ă��Ȃ�������
	return
		!inputController_->IsPressed(ACTION::MOVE_FORWARD)
		&& !inputController_->IsPressed(ACTION::MOVE_UP)
		&& !inputController_->IsPressed(ACTION::MOVE_BACK)
		&& !inputController_->IsPressed(ACTION::MOVE_LEFT)
		&& !inputController_->IsPressed(ACTION::MOVE_RIGHT);
}
