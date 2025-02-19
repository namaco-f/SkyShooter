
#include "../Application.h"
#include "../Utility/DebugUtility.h"
#include "../Utility/MathUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include"../GetUIDataToVariantTemplate.h"
#include "../Object/Actor/Stage/StageSize.h"
#include "UI_MiniMap.h"

using namespace std;

using Math = MathUtility;

//ミニマップのサイズ
const Vector2 MINIMAP_SCREEN_SIZE = {
	Application::SCREEN_SIZE_HALF_X / 5,
	Application::SCREEN_SIZE_HALF_X / 5 
};	

//ミニマップに描画する座標範囲
const Vector2f MINIMAP_DRAW_POS = {
	(float)(STAGE_MOVELIMIT_X ) * 2.0f,
	(float)(STAGE_MOVELIMIT_Z ) * 2.0f
};

//ミニマップ座標の縮小レート
const Vector2f MINIMAP_POS_DIVRATE = {
	(float)(MINIMAP_SCREEN_SIZE.x) / (STAGE_MOVELIMIT_X * 2.0f),
	(float)(MINIMAP_SCREEN_SIZE.y) / (STAGE_MOVELIMIT_Z * 2.0f),
};

//ミニマップ描画時の座標オフセット値
const Vector2f MINIMAP_OFFSET = {
	MINIMAP_SCREEN_SIZE.x / 2,
	MINIMAP_SCREEN_SIZE.y / 2
};

//ミニマップグリッド線の分割数
const int MINIMAP_GRID_NUM = 5;

//ミニマップグリッド線のサイズ
const Vector2 MINIMAP_GRID_SIZE ={ 
	MINIMAP_SCREEN_SIZE.x / MINIMAP_GRID_NUM,
	MINIMAP_SCREEN_SIZE.y / MINIMAP_GRID_NUM 
};

//ミニマップポインターの半径
const int MINIMAP_POINTER_RADIUS = 5;

//ミニマップグリッド線のアルファ値(割合)
const float MINIMAP_GRID_ALPHA_PAR = 0.5f;

//ミニマップグリッド線のアルファ値
const int MINIMAP_GRID_ALPHA = (int)(255.0f * MINIMAP_GRID_ALPHA_PAR);

//ミニマップを描画する座標のオフセット
const int MINIMAP_POS_OFFSET = 50;

//ミニマップ背景オフセット
const int MINIMAP_BG_START_POS_OFFSET = 5;

//ミニマップ座標
const Vector2 MINIMAP_POS = {
	Application::SCREEN_SIZE_X - MINIMAP_SCREEN_SIZE.x - MINIMAP_POS_OFFSET,
	0 + MINIMAP_POS_OFFSET,
};

//ミニマップ背景開始座標
const Vector2 MINIMAP_BG_STARTPOS = {
	MINIMAP_POS.x - MINIMAP_BG_START_POS_OFFSET,
	MINIMAP_POS.y - MINIMAP_BG_START_POS_OFFSET,
};

//ミニマップ背景終了座標
const Vector2 MINIMAP_BG_ENDPOS = {
	MINIMAP_POS.x + MINIMAP_SCREEN_SIZE.x + MINIMAP_BG_START_POS_OFFSET,
	MINIMAP_POS.y + MINIMAP_SCREEN_SIZE.y + MINIMAP_BG_START_POS_OFFSET,
};

UI_MiniMap::UI_MiniMap(void)
	:
	UIBase(),
	miniMapScreen_(-1),
	playerMiniMapPos_{},
	ememyMiniMapPos_{}
{
	SetUIType(UIType::MiniMap);
	miniMapScreen_ = MakeScreen(MINIMAP_SCREEN_SIZE.x, MINIMAP_SCREEN_SIZE.y);

	UpdateMiniMapPos();
}

UI_MiniMap::~UI_MiniMap(void)
{
	DeleteGraph(miniMapScreen_);
}

void UI_MiniMap::Update(void)
{
	UpdateMiniMapPos();
}

void UI_MiniMap::Draw(void)
{
	auto& scnMng = SceneManager::GetInstance();
	auto& debug = scnMng.GetDebugUtility();

	DrawBox(
		MINIMAP_BG_STARTPOS.x, MINIMAP_BG_STARTPOS.y,
		MINIMAP_BG_ENDPOS.x, MINIMAP_BG_ENDPOS.y,
		0xffffff, true
	);

	SetDrawScreen(miniMapScreen_);

	SetBackgroundColor(50, 50, 100);

	ClearDrawScreen();

	MiniMapGridDraw();

	MiniMapPlayerDraw();
	
	MiniMapEnemyDraw();

	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());
	
	DrawGraph(
		MINIMAP_POS.x, MINIMAP_POS.y, miniMapScreen_, false
	);
}

void UI_MiniMap::UpdateMiniMapPos(void)
{
	//プレイヤー座標更新
	const auto playerPos = GetUIDataToVariant<VECTOR>(SRC_UIData::PLAYER_POS);
	playerMiniMapPos_ = Math::VECTORToVector2f_XZ(playerPos);
	playerMiniMapPos_ = playerMiniMapPos_ * MINIMAP_POS_DIVRATE;
	//Y座標反転
	playerMiniMapPos_.y *= -1.0f;
	//-座標も描画
	playerMiniMapPos_ += MINIMAP_OFFSET;

	//敵座標更新
	const auto ememyPos = GetUIDataToVariant<VECTOR>(SRC_UIData::ENEMY_POS);
	ememyMiniMapPos_ = Math::VECTORToVector2f_XZ(ememyPos);
	ememyMiniMapPos_ = ememyMiniMapPos_ * MINIMAP_POS_DIVRATE;
	
	//Y座標反転
	ememyMiniMapPos_.y *= -1.0f;

	//-座標も描画
	ememyMiniMapPos_ += MINIMAP_OFFSET;
}

void UI_MiniMap::MiniMapGridDraw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MINIMAP_GRID_ALPHA);
	
	for (int n = 0; n <= MINIMAP_GRID_NUM; n++)
	{
		//縦軸
		DrawLine(
			MINIMAP_GRID_SIZE.x * n, 0,
			MINIMAP_GRID_SIZE.x * n, MINIMAP_SCREEN_SIZE.y,
			0xffffff
		);

		//横軸
		DrawLine(
			0, MINIMAP_GRID_SIZE.y * n,
			MINIMAP_SCREEN_SIZE.x, MINIMAP_GRID_SIZE.y * n,
			0xffffff
		);
	}
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_MiniMap::MiniMapPlayerDraw(void)
{
	DrawCircle(
		(int)playerMiniMapPos_.x, (int)playerMiniMapPos_.y,
		MINIMAP_POINTER_RADIUS, 0x00ff00, true);
}

void UI_MiniMap::MiniMapEnemyDraw(void)
{
	DrawCircle(
		(int)ememyMiniMapPos_.x, (int)ememyMiniMapPos_.y,
		MINIMAP_POINTER_RADIUS, 0xff0000, true);
}
