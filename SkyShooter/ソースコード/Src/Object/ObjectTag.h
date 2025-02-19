#pragma once

//オブジェクトの名前
enum class OBJECT_TYPE
{
	NONE,		//初期化用
	STAGE,		//ステージ
	
	PLAYER,				//自機
	PLAYER_ATTACK,		//自機の攻撃(仮)
	PLAYER_ATTACK2,		//自機の攻撃(仮)
	PLAYER_ULTATTACK,	//自機の攻撃(仮)
	
	ENEMY,				//敵
	ENEMY_ATACK,		//敵の攻撃(仮)
	ENEMY_MISSILE,		//敵の攻撃(仮)
	ENEMY_ATACK_BOX,	//敵の範囲攻撃

	MAX,		//最大数カウント用
};
