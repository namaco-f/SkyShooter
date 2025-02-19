
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
		//ロックオンの切替
		player.NoticeAction(ACTION::PLAYER_LOCK_SWITCH);
	}

	if (inputController_->IsPressed(ACTION::PLAYER_SHIELD)
		&& !player.IsShieldCounter())
	{
		//シールド状態を通知
		player.NoticeAction(ACTION::PLAYER_SHIELD);

		//シールド中は他アクションに切り替わらない
		return;
	}

	if (player.IsAbleShield())
	{
		//シールド状態
		return;
	}

	//移動関連
	InputUpdateMove(player);

	//攻撃関連
	InputUpdateAttack(player);
}

void PlayerInputComponent::InputUpdateMove(PlayerBase& player)
{
	if (inputController_->IsPressed(ACTION::MOVE_FORWARD))
	{
		//前方移動
		player.NoticeAction(ACTION::MOVE_FORWARD);
	}

	if (inputController_->IsPressed(ACTION::MOVE_BACK))
	{
		//後方移動
		player.NoticeAction(ACTION::MOVE_BACK);
	}

	if (inputController_->IsPressed(ACTION::MOVE_LEFT))
	{
		//左移動
		player.NoticeAction(ACTION::MOVE_LEFT);
	}

	if (inputController_->IsPressed(ACTION::MOVE_RIGHT))
	{
		//右移動
		player.NoticeAction(ACTION::MOVE_RIGHT);
	}

	if (inputController_->IsPressed(ACTION::MOVE_UP))
	{
		//上移動
		player.NoticeAction(ACTION::MOVE_UP);
	}

	if (inputController_->IsPressed(ACTION::PLAYER_DASH))
	{
		//ダッシュ
		player.ReceiveCommonMessage(Common_Message::DASH);
	}

	if (inputController_->IsReleased(ACTION::PLAYER_DASH))
	{
		//ダッシュキーが離され、通常移動に切替
		player.ReceiveCommonMessage(Common_Message::DASH_END);
	}

	if (IsNotMoving())
	{
		//移動していない
		player.NoticeAction(ACTION::PLAYER_IDLE);
	}
}

void PlayerInputComponent::InputUpdateAttack(PlayerBase& player)
{
	if (player.IsUltAttackAble())
	{
		//必殺技中は他の攻撃をしない
		return;
	}

	if (inputController_->IsTriggered(ACTION::PLAYER_ULTATTACK))
	{
		//必殺技
		player.NoticeAction(ACTION::PLAYER_ULTATTACK);
		return;
	}

	if (inputController_->IsPressed(ACTION::PLAYER_ATTACK))
	{
		//通常弾
		player.NoticeAction(ACTION::PLAYER_ATTACK);
	}

	if (inputController_->IsPressed(ACTION::PLAYER_ATTACK2))
	{
		//追尾弾
		player.NoticeAction(ACTION::PLAYER_ATTACK2);
	}
}

const bool PlayerInputComponent::IsNotMoving(void)
{
	//どの移動キーも押されていないか判定
	return
		!inputController_->IsPressed(ACTION::MOVE_FORWARD)
		&& !inputController_->IsPressed(ACTION::MOVE_UP)
		&& !inputController_->IsPressed(ACTION::MOVE_BACK)
		&& !inputController_->IsPressed(ACTION::MOVE_LEFT)
		&& !inputController_->IsPressed(ACTION::MOVE_RIGHT);
}
