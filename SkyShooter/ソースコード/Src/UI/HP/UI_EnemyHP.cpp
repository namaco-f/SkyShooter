
#include <memory>
#include "../Utility/DebugUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ResourceManager/Font/FontResourcesManager.h"
#include "../Application.h"
#include "../Object/Actor/Charactor/Player/PlayerBase.h"
#include"../GetUIDataToVariantTemplate.h"
#include "UI_EnemyHP.h"

using namespace std;

const Vector2f OFFSET_HP_BAR_POS = { 50.0f, 50.0f };
const Vector2f OFFSET_HP_PROGRESSBAR_POS = { 3.0f,3.0f };

const Vector2f HP_BAR_SIZE = { (float)Application::SCREEN_SIZE_HALF_X,50.0f };
const Vector2f HP_PROGRESSBAR_SIZE = HP_BAR_SIZE - OFFSET_HP_PROGRESSBAR_POS * 2.0f;

const float HP_BAR_STARTPOS_X = (float)Application::SCREEN_SIZE_HALF_X / 2.0f;

const Vector2f HP_BAR_STARTPOS = {
	HP_BAR_STARTPOS_X,
	OFFSET_HP_BAR_POS.y
};

const Vector2f HP_BAR_ENDPOS = {
	HP_BAR_STARTPOS.x + HP_BAR_SIZE.x,
	HP_BAR_STARTPOS.y + HP_BAR_SIZE.y
};

const Vector2f HP_PROGRESSBAR_STARTPOS = {
	HP_BAR_STARTPOS_X + OFFSET_HP_PROGRESSBAR_POS.x,
	HP_BAR_STARTPOS.y + OFFSET_HP_PROGRESSBAR_POS.y
};

const Vector2f HP_PROGRESSBAR_ENDPOS_DEF = {
	HP_BAR_STARTPOS_X + HP_PROGRESSBAR_SIZE.x,
	HP_PROGRESSBAR_STARTPOS.y + HP_PROGRESSBAR_SIZE.y
};

const string UISTRING_ENEMY = "BOSS ENEMY";
const TCHAR* UISTRING_ENEMY_TCHAR = UISTRING_ENEMY.c_str();

const Vector2 UISTRING_ENEMY_POS = {
	(int)(HP_BAR_STARTPOS.x + HP_BAR_SIZE.x / 2.0f),
	(int)HP_BAR_STARTPOS.y,
};


UI_EnemyHP::UI_EnemyHP(void)
	:
	UIBase()
{
	auto& resMngFont = ResourceManager::GetInstance().GetFontResourcesInstance();

	s20FontHandle_ = resMngFont.Load(SRC_FONT::Pgothic_s20).GetHandleId();
	SetUIType(UIType::EnemyHp);

	Update();
}

UI_EnemyHP::~UI_EnemyHP(void)
{
}

void UI_EnemyHP::Update(void)
{
	hp_ = UIParam(
		GetUIDataToVariant<int>(SRC_UIData::ENEMY_HP),
		GetUIDataToVariant<int>(SRC_UIData::ENEMY_HPMAX)
	);
}

void UI_EnemyHP::Draw(void)
{
	//HPÉoÅ[ï`âÊ
	DrawHPBar();

	//ìGÇÃñºëOï`âÊ
	DrawEnemyName();

	//DrawDebug();
}

void UI_EnemyHP::DrawHPBar(void)
{
	const float hpPar = (float)hp_.val / (float)hp_.valMax;

	DrawBoxAA(
		HP_BAR_STARTPOS.x, HP_BAR_STARTPOS.y,
		HP_BAR_ENDPOS.x, HP_BAR_ENDPOS.y, 0xffffff, true
	);

	const float HP_PROGRESSBAR_ENDPOS_X =
		HP_PROGRESSBAR_STARTPOS.x + HP_PROGRESSBAR_SIZE.x * hpPar;

	DrawBoxAA(
		HP_PROGRESSBAR_STARTPOS.x, HP_PROGRESSBAR_STARTPOS.y,
		HP_PROGRESSBAR_ENDPOS_X, HP_PROGRESSBAR_ENDPOS_DEF.y, 0xff5555, true
	);
}

void UI_EnemyHP::DrawEnemyName(void)
{
	int strWidth = GetDrawFormatStringWidthToHandle(s20FontHandle_, UISTRING_ENEMY_TCHAR);
	int strFontSize = GetFontSizeToHandle(s20FontHandle_) + GetFontEdgeSizeToHandle(s20FontHandle_);

	DrawFormatStringToHandle(
		UISTRING_ENEMY_POS.x - (strWidth / 2),
		UISTRING_ENEMY_POS.y - strFontSize,
		0xffffff, s20FontHandle_, UISTRING_ENEMY_TCHAR);
}

void UI_EnemyHP::DrawDebug(void)
{
	auto& scnMng = SceneManager::GetInstance();
	auto& debug = scnMng.GetDebugUtility();

	const Vector2 hps = { hp_.val,hp_.valMax };
	debug.DebugStringFormat(0, "%d,%d", 0xff0000, hps);
}
