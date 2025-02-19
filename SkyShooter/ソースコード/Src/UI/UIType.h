#pragma once

/// <summary>
/// UI種別
/// </summary>
enum class UIType
{
	None,			//初期化用
	
	PlayerParameter,		//自機HP
	EnemyHp,		//敵HP
	MiniMap,		//ミニマップ

	MAX,			//最大数カウント用
};

/// <summary>
/// UI用データ名
/// </summary>
enum class SRC_UIData
{
	PLAYER_HP,				//自機現HP
	PLAYER_HPMAX,			//自機最大HP
	PLAYER_MP,				//自機現MP
	PLAYER_MPMAX,			//自機最大MP
	PLAYER_UC,				//自機最大MP
	PLAYER_UCMAX,			//自機最大MP
	PLAYER_SHILD_TIME,		//シールド可能時間
	PLAYER_SHILD_LIMIT,		//シールド持続限界時間
	PLAYER_SHILD_COOLPAR,	//シールド時カウンターカウント時間
	PLAYER_POS,				//自機座標

	ENEMY_HP,				//敵現HP
	ENEMY_HPMAX,			//敵最大HP
	ENEMY_POS,				//敵座標

	BGM_VOLUME,				//BGM音量
	SE_VOLUME,				//SE音量

	CLEAR_TIME,				//クリアタイム(秒)

	MAX,					//最大数カウント用、エラー値
};
