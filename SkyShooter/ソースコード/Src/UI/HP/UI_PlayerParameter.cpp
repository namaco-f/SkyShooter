
#include <memory>
#include "../Utility/DebugUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ResourceManager/Font/FontResourcesManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Object/Actor/Charactor/Player/PlayerBase.h"
#include "../Application.h"
#include"../GetUIDataToVariantTemplate.h"
#include "UI_PlayerParameter.h"

using namespace std;

//UI�̌����J�E���g(UI�w�i�`��p)
const int UI_COUNT = 4;	

const string UISTRING_VALUE_INT = " %d / %d ";		//DrawFormatString�p
const string UISTRING_VALUE_UCPERCENT = " %.f";		//DrawFormatString�p

//DrawFormatString�p(TCHAR)
const TCHAR* UISTRING_VALUE_INT_TCHAR = UISTRING_VALUE_INT.c_str();

//DrawFormatString�p(TCHAR)
const TCHAR* UISTRING_VALUE_FLOAT_TCHAR = UISTRING_VALUE_UCPERCENT.c_str();

//UI�J�n���WX
const int UI_START_POS_X = 64;

//UI�J�n���WY
const int UISTART_POS_Y = 
Application::SCREEN_SIZE_Y - (Application::SCREEN_SIZE_HALF_Y * 2 / 5 );

//�X�e�[�^�X�o�[�̒�����
const int BAR_SIZE_X = 450;

//�X�e�[�^�X�o�[�̒����c
const int BAR_SIZE_Y = 20;		

//HP�o�[�̏c���T�C�Y
const Vector2 HP_BAR_SIZE = { BAR_SIZE_X,BAR_SIZE_Y };

//HP�o�[�̍��W�I�t�Z�b�g
const Vector2 HP_BAR_POS_OFFSET = { UI_START_POS_X, 32 };

//HP�����o�[�̍��W�I�t�Z�b�g
const int HP_PROGRESSBAR_POS_OFFSET = 2;

//HP�����o�[�̏c���T�C�Y
const Vector2f HP_PROGRESSBAR_SIZE = {
	(float)(HP_BAR_SIZE.x - HP_PROGRESSBAR_POS_OFFSET * 2),
	(float)(HP_BAR_SIZE.y - HP_PROGRESSBAR_POS_OFFSET * 2),
};

//HP�o�[�̕`��J�n���W
const Vector2 HP_BAR_STARTPOS = {
	HP_BAR_POS_OFFSET.x,
	UISTART_POS_Y,
};

//HP�o�[�̕`��I�����W
const Vector2 HP_BAR_ENDPOS = {
	HP_BAR_POS_OFFSET.x + HP_BAR_SIZE.x,
	UISTART_POS_Y + HP_BAR_SIZE.y,
};

//HP�����o�[�̕`��J�n���W
const Vector2 HP_PROGRESSBAR_STARTPOS = {
	HP_BAR_STARTPOS.x + HP_PROGRESSBAR_POS_OFFSET,
	HP_BAR_STARTPOS.y + HP_PROGRESSBAR_POS_OFFSET,
};

//HP�����o�[�̕`��I�����W
const Vector2 HP_PROGRESSBAR_ENDPOS_DEF = {
	HP_BAR_ENDPOS.x - HP_PROGRESSBAR_POS_OFFSET,
	HP_BAR_ENDPOS.y - HP_PROGRESSBAR_POS_OFFSET,
};

//MP�o�[�̃T�C�Y
const Vector2 MP_BAR_SIZE = { BAR_SIZE_X,10 };

//MP�o�[�̍��W�I�t�Z�b�g
const Vector2 MP_BAR_POS_OFFSET = { UI_START_POS_X,20 };

//MP�����o�[�̍��W�I�t�Z�b�g
const int MP_PROGRESSBAR_POS_OFFSET = 1;

//MP�����o�[�̃T�C�Y
const Vector2f MP_PROGRESSBAR_SIZE = {
	(float)(MP_BAR_SIZE.x - MP_PROGRESSBAR_POS_OFFSET * 2),
	(float)(MP_BAR_SIZE.y - MP_PROGRESSBAR_POS_OFFSET * 2),
};

//MP�o�[�̕`��J�n���W
const Vector2 MP_BAR_STARTPOS = {
	MP_BAR_POS_OFFSET.x,
	HP_BAR_ENDPOS.y + MP_BAR_POS_OFFSET.y,
};

//MP�o�[�̕`��I�����W
const Vector2 MP_BAR_ENDPOS = {
	MP_BAR_POS_OFFSET.x + MP_BAR_SIZE.x,
	MP_BAR_STARTPOS.y + MP_BAR_SIZE.y,
};

//MP�����o�[�̕`��J�n���W
const Vector2 MP_PROGRESSBAR_STARTPOS = {
	MP_BAR_STARTPOS.x + MP_PROGRESSBAR_POS_OFFSET,
	MP_BAR_STARTPOS.y + MP_PROGRESSBAR_POS_OFFSET,
};

//MP�o�[�̕`��I�����W
const Vector2 MP_PROGRESSBAR_ENDPOS_DEF = {
	MP_BAR_ENDPOS.x - MP_PROGRESSBAR_POS_OFFSET,
	MP_BAR_ENDPOS.y - MP_PROGRESSBAR_POS_OFFSET,
};

//UC�o�[�̃T�C�Y
const Vector2f UC_BAR_SIZE = { (float)(BAR_SIZE_X * 3 / 4),10.0f };

//UC�o�[�̍��W�I�t�Z�b�g
const Vector2f UC_BAR_POS_OFFSET = { (float)UI_START_POS_X,20.0f };

//UC�����o�[�̍��W�I�t�Z�b�g
const float UC_PROGRESSBAR_POS_OFFSET = 1.0f;

//UC�����o�[�̃T�C�Y
const Vector2f UC_PROGRESSBAR_SIZE = {
	UC_BAR_SIZE.x - UC_PROGRESSBAR_POS_OFFSET * 2.0f,
	UC_BAR_SIZE.y - UC_PROGRESSBAR_POS_OFFSET * 2.0f,
};

//UC�o�[�̕`��J�n���W
const Vector2 UC_BAR_STARTPOS = {
	(int)UC_BAR_POS_OFFSET.x,
	MP_BAR_ENDPOS.y + (int)UC_BAR_POS_OFFSET.y 
};

//UC�o�[�̕`��I�����W
const Vector2 UC_BAR_ENDPOS = {
	(int)(UC_BAR_POS_OFFSET.x + UC_BAR_SIZE.x),
	UC_BAR_STARTPOS.y + (int)UC_BAR_SIZE.y,
};

//UC�����o�[�̕`��J�n���W
const Vector2 UC_PROGRESSBAR_STARTPOS = {
	UC_BAR_STARTPOS.x + (int)UC_PROGRESSBAR_POS_OFFSET,
	UC_BAR_STARTPOS.y + (int)UC_PROGRESSBAR_POS_OFFSET,
};

//UC�����o�[�̕`��I�����W
const Vector2 UC_PROGRESSBAR_ENDPOS_DEF = {
	UC_BAR_ENDPOS.x - (int)UC_PROGRESSBAR_POS_OFFSET,
	UC_BAR_ENDPOS.y - (int)UC_PROGRESSBAR_POS_OFFSET,
};

//UC�o�[�̃T�C�Y
const Vector2f SHIELD_BAR_SIZE = { (float)(BAR_SIZE_X * 3 / 4),10.0f };

//UC�o�[�̍��W�I�t�Z�b�g
const Vector2f SHIELD_BAR_POS_OFFSET = { (float)UI_START_POS_X,20.0f };

//UC�����o�[�̍��W�I�t�Z�b�g
const float SHIELD_PROGRESSBAR_POS_OFFSET = 1.0f;

//UC�����o�[�̃T�C�Y
const Vector2f SHIELD_PROGRESSBAR_SIZE = {
	SHIELD_BAR_SIZE.x - SHIELD_PROGRESSBAR_POS_OFFSET * 2.0f,
	SHIELD_BAR_SIZE.y - SHIELD_PROGRESSBAR_POS_OFFSET * 2.0f,
};

//UC�o�[�̕`��J�n���W
const Vector2 SHIELD_BAR_STARTPOS = {
	(int)SHIELD_BAR_POS_OFFSET.x,
	UC_BAR_ENDPOS.y + (int)SHIELD_BAR_POS_OFFSET.y
};

//UC�o�[�̕`��I�����W
const Vector2 SHIELD_BAR_ENDPOS = {
	(int)(SHIELD_BAR_POS_OFFSET.x + SHIELD_BAR_SIZE.x),
	SHIELD_BAR_STARTPOS.y + (int)SHIELD_BAR_SIZE.y,
};

//UC�����o�[�̕`��J�n���W
const Vector2 SHIELD_PROGRESSBAR_STARTPOS = {
	SHIELD_BAR_STARTPOS.x + (int)SHIELD_PROGRESSBAR_POS_OFFSET,
	SHIELD_BAR_STARTPOS.y + (int)SHIELD_PROGRESSBAR_POS_OFFSET,
};

//UC�����o�[�̕`��I�����W
const Vector2 SHIELD_PROGRESSBAR_ENDPOS_DEF = {
	SHIELD_BAR_ENDPOS.x - (int)SHIELD_PROGRESSBAR_POS_OFFSET,
	SHIELD_BAR_ENDPOS.y - (int)SHIELD_PROGRESSBAR_POS_OFFSET,
};

const int UISTRING_OFFSET_X = 5;

//HP�̕�����\�L
const string UISTRING_HP = "HP";

//HP��������W
const Vector2 INIT_UI_HP_POS = {
	HP_BAR_STARTPOS.x + UISTRING_OFFSET_X,
	HP_BAR_STARTPOS.y - (HP_BAR_SIZE.y / 2),
};

//HP�\�L
const Vector2 INIT_UI_HPVALUE_POS = {
	HP_BAR_STARTPOS.x + (HP_BAR_SIZE.x / 2),
	HP_BAR_STARTPOS.y,
};

const int UISTRING_MP_OFFSET_X = 5;
//MP�̕�����\�L
const string UISTRING_MP = "MP";

//MP�̕�������W
const Vector2 INIT_UI_MP_POS = {
	MP_BAR_STARTPOS.x + UISTRING_OFFSET_X + UISTRING_MP_OFFSET_X,
	MP_BAR_STARTPOS.y - (MP_BAR_SIZE.y / 2),
};

//MP�\�L
const Vector2 INIT_UI_MPVALUE_POS = {
	MP_BAR_STARTPOS.x + (MP_BAR_SIZE.x / 2),
	MP_BAR_STARTPOS.y,
};

//UC�̕�����\�L
const string UISTRING_UC = "UC";

//UC�̕�������W
const Vector2 INIT_UI_UC_POS = {
	UC_BAR_STARTPOS.x,
	UC_BAR_STARTPOS.y - ((int)UC_BAR_SIZE.y / 2),
};

//UC�\�L
const Vector2 INIT_UI_UCVALUE_POS = {
	UC_BAR_ENDPOS.x,
	UC_BAR_STARTPOS.y - ((int)UC_BAR_SIZE.y / 2),
};

const Vector2 UI_BG_DIFF = { 48,32 };



UI_PlayerParameter::UI_PlayerParameter(void)
	:
	UIBase()
{
	auto& resMngFont = ResourceManager::GetInstance().GetFontResourcesInstance();
	auto& resMngImg = ResourceManager::GetInstance().GetImageResourcesInstance();

	s20FontHandle_ = resMngFont.Load(SRC_FONT::Pgothic_s20).GetHandleId();
	SetUIType(UIType::PlayerParameter);

	hpUITextImgHandle_ = resMngImg.Load(SRC_IMAGE::HP_TEXT).GetHandleId();
	mpUITextImgHandle_ = resMngImg.Load(SRC_IMAGE::MP_TEXT).GetHandleId();

	Update();
}

UI_PlayerParameter::~UI_PlayerParameter(void)
{
}

void UI_PlayerParameter::Update(void)
{
	hp_ = UIParam<int>(
		GetUIDataToVariant<int>(SRC_UIData::PLAYER_HP),
		GetUIDataToVariant<int>(SRC_UIData::PLAYER_HPMAX)
	);

	mp_ = UIParam<int>(
		GetUIDataToVariant<int>(SRC_UIData::PLAYER_MP),
		GetUIDataToVariant<int>(SRC_UIData::PLAYER_MPMAX)
	);	
	
	uc_ = UIParam<float>(
		GetUIDataToVariant<float>(SRC_UIData::PLAYER_UC),
		GetUIDataToVariant<float>(SRC_UIData::PLAYER_UCMAX)
	);
}

void UI_PlayerParameter::Draw(void)
{
	auto& uiData = UIDataManager::GetInstance();
	
	auto& scnMng = SceneManager::GetInstance();
	auto& debug = scnMng.GetDebugUtility();

	DrawBox(
		UI_START_POS_X - UI_BG_DIFF.x, 
		UISTART_POS_Y - UI_BG_DIFF.y,
		UI_START_POS_X + BAR_SIZE_X + UI_BG_DIFF.x, 
		UISTART_POS_Y + UI_BG_DIFF.y * UI_COUNT,
		0x000000,true
	);

	DrawHP();

	DrawMP();

	DrawUC();

	DrawShieldTime();
}

void UI_PlayerParameter::DrawHP(void)
{
	if (hp_.valMax == 0)
	{
		return;
	}

	const float hpPar = (float)hp_.val / (float)hp_.valMax;

	DrawBox(
		HP_BAR_STARTPOS.x, HP_BAR_STARTPOS.y,
		HP_BAR_ENDPOS.x, HP_BAR_ENDPOS.y,
		0xffffff, true
	);

	const int hpProgressBarSize = (int)((float)HP_PROGRESSBAR_SIZE.x * hpPar);
	const int hpProgressBarEndPosX = HP_PROGRESSBAR_STARTPOS.x + hpProgressBarSize;

	DrawBox(
		HP_PROGRESSBAR_STARTPOS.x, HP_PROGRESSBAR_STARTPOS.y,
		hpProgressBarEndPosX
		, HP_PROGRESSBAR_ENDPOS_DEF.y,
		0x00ff00, true
	);

	int strWidth = GetDrawFormatStringWidthToHandle(
		s20FontHandle_, UISTRING_VALUE_INT_TCHAR,
		hp_.val, hp_.valMax);
	int strFontSize = GetFontSizeToHandle(s20FontHandle_) + GetFontEdgeSizeToHandle(s20FontHandle_);


	DrawFormatStringToHandle(
		INIT_UI_HPVALUE_POS.x - (strWidth / 2),
		INIT_UI_HPVALUE_POS.y - strFontSize,
		0xffffff, s20FontHandle_, UISTRING_VALUE_INT_TCHAR, 
		hp_.val, hp_.valMax
	);

	//strWidth = GetDrawFormatStringWidthToHandle(
	//	s20FontHandle_, UISTRING_HP.c_str()
	//);
	//strFontSize = GetFontSizeToHandle(s20FontHandle_) + GetFontEdgeSizeToHandle(s20FontHandle_);

	//DrawFormatStringToHandle(
	//	INIT_UI_HP_POS.x - (strWidth),
	//	INIT_UI_HP_POS.y,
	//	0xffffff, s20FontHandle_,
	//	UISTRING_HP.c_str());

	Vector2 imgSize = {};

	GetGraphSize(hpUITextImgHandle_, &imgSize.x, &imgSize.y);
	DrawRotaGraph(
		INIT_UI_HP_POS.x - (imgSize.x / 2),
		INIT_UI_HP_POS.y + (imgSize.y / 2),
		1.0, 0.0,
		hpUITextImgHandle_, true
	);	
}

void UI_PlayerParameter::DrawMP(void)
{
	if (mp_.valMax == 0)
	{
		return;
	}

	const float mpPar = (float)mp_.val / (float)mp_.valMax;

	DrawBox(
		MP_BAR_STARTPOS.x, MP_BAR_STARTPOS.y,
		MP_BAR_ENDPOS.x, MP_BAR_ENDPOS.y,
		0xffffff, true
	);

	const int mpProgressBarSize = (int)((float)MP_PROGRESSBAR_SIZE.x * mpPar);
	const int mpProgressBarEndPosX = MP_PROGRESSBAR_STARTPOS.x + mpProgressBarSize;

	DrawBox(
		MP_PROGRESSBAR_STARTPOS.x, MP_PROGRESSBAR_STARTPOS.y,
		mpProgressBarEndPosX, MP_PROGRESSBAR_ENDPOS_DEF.y,
		0x00ffff, true
	);

	int strWidth = GetDrawFormatStringWidthToHandle(
		s20FontHandle_, UISTRING_VALUE_INT_TCHAR,
		mp_.val, mp_.valMax);
	int strFontSize = GetFontSizeToHandle(s20FontHandle_) + GetFontEdgeSizeToHandle(s20FontHandle_);

	DrawFormatStringToHandle(
		INIT_UI_MPVALUE_POS.x - (strWidth / 2),
		INIT_UI_MPVALUE_POS.y - strFontSize,
		0xffffff, s20FontHandle_,
		UISTRING_VALUE_INT_TCHAR, mp_.val, mp_.valMax
	);


	//strWidth = GetDrawFormatStringWidthToHandle(
	//	s20FontHandle_, UISTRING_MP.c_str()
	//);
	//strFontSize = GetFontSizeToHandle(s20FontHandle_) + GetFontEdgeSizeToHandle(s20FontHandle_);

	//DrawFormatStringToHandle(
	//	INIT_UI_MP_POS.x - (strWidth),
	//	INIT_UI_MP_POS.y,
	//	0xffffff, s20FontHandle_,
	//	UISTRING_MP.c_str());

	Vector2 imgSize = {};

	GetGraphSize(mpUITextImgHandle_, &imgSize.x, &imgSize.y);
	DrawRotaGraph(
		INIT_UI_MP_POS.x - (imgSize.x / 2),
		INIT_UI_MP_POS.y + (imgSize.y / 4),
		1.0, 0.0,
		mpUITextImgHandle_, true
	);
}

void UI_PlayerParameter::DrawUC(void)
{
	if (uc_.valMax == 0.0f)
	{
		return;
	}

	float ucPar = (float)uc_.val / (float)uc_.valMax;

	if (ucPar > 1.00f)
	{
		ucPar = 1.00f;
	}

	DrawBox(
		UC_BAR_STARTPOS.x, UC_BAR_STARTPOS.y,
		UC_BAR_ENDPOS.x, UC_BAR_ENDPOS.y,
		0xffffff, true
	);

	const int ucProgressBarSize = (int)(UC_PROGRESSBAR_SIZE.x * ucPar);
	const int ucProgressBarEndPosX = UC_PROGRESSBAR_STARTPOS.x + ucProgressBarSize;

	DrawBox(
		UC_PROGRESSBAR_STARTPOS.x, UC_PROGRESSBAR_STARTPOS.y,
		ucProgressBarEndPosX, UC_PROGRESSBAR_ENDPOS_DEF.y,
		0xff00ff, true
	);

	const float uc = ucPar * 100.0f;

	int strWidth = GetDrawFormatStringWidthToHandle(
		s20FontHandle_, UISTRING_VALUE_FLOAT_TCHAR,
		uc);
	int strFontSize = GetFontSizeToHandle(s20FontHandle_) + GetFontEdgeSizeToHandle(s20FontHandle_);

	DrawFormatStringToHandle(
		INIT_UI_UCVALUE_POS.x, 
		INIT_UI_UCVALUE_POS.y,
		0xffffff, s20FontHandle_, UISTRING_VALUE_FLOAT_TCHAR, uc
	);
}

void UI_PlayerParameter::DrawShieldTime(void)
{
	float shieldTime = GetUIDataToVariant<float>(SRC_UIData::PLAYER_SHILD_TIME);
	float shieldLimit = GetUIDataToVariant<float>(SRC_UIData::PLAYER_SHILD_LIMIT);
	float shieldCoolPar = GetUIDataToVariant<float>(SRC_UIData::PLAYER_SHILD_COOLPAR);

	if (shieldCoolPar > 1.0f)
	{
		shieldCoolPar = 1.0f;
	}
	else if (shieldCoolPar < 0.0f)
	{
		shieldCoolPar = 0.0f;
	}

	DrawBox(
		SHIELD_BAR_STARTPOS.x, SHIELD_BAR_STARTPOS.y,
		SHIELD_BAR_ENDPOS.x, SHIELD_BAR_ENDPOS.y,
		0xffffff, true
	);

	const int ucProgressBarSize = (int)(SHIELD_PROGRESSBAR_SIZE.x * shieldCoolPar);
	const int ucProgressBarEndPosX = SHIELD_PROGRESSBAR_STARTPOS.x + ucProgressBarSize;

	DrawBox(
		SHIELD_PROGRESSBAR_STARTPOS.x, SHIELD_PROGRESSBAR_STARTPOS.y,
		ucProgressBarEndPosX, SHIELD_PROGRESSBAR_ENDPOS_DEF.y,
		0xffff00, true
	);

}
