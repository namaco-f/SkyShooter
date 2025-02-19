#pragma once

enum class Message
{
	Clear,				//ゲームクリア
	GameOver,			//ゲームオーバー
	SoundVolumeChange,	//音量変化
	IsGroundPlayer,		//プレイヤーが接地
	IsNotGroundPlayer,		//プレイヤーが接地していない
};
