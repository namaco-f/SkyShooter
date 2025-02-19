#pragma once
#include <vector>

/// <summary>
/// シーン管理用
/// </summary>
enum class SCENE_ID
{
	NONE,		//初期化用
	TITLE,		//タイトルシーン
	GAME,		//ゲームシーン
	RESULT,		//リザルトシーン
	CLEAR,		//クリアシーン
	FAILED,		//敗北シーン
	MAX,		//シーン数カウント用
};

const std::vector<SCENE_ID> USESCENE_GAME_ONLY = {
		SCENE_ID::GAME,
};

const std::vector<SCENE_ID> USESCENE_TITLE_ONLY = {
		SCENE_ID::TITLE,
};

const std::vector<SCENE_ID> USESCENE_ALL = {
		SCENE_ID::TITLE,
		SCENE_ID::GAME,
		SCENE_ID::CLEAR,
		SCENE_ID::FAILED,
};
