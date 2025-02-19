#include <DxLib.h>
#include "../Application.h"
#include "../Manager/TimeManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Manager/ResourceManager/Font/FontResourcesManager.h"
#include "../Manager/Input/InputController.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "../Observer/Observer.h"
#include "../Sound/SoundController.h"
#include "../Utility/DebugUtility.h"
#include "../Utility/MathUtility.h"
#include "SettingScene.h"

using namespace std;

const int RESERVE_DATA_NUM_MAX = static_cast<int>(SRC_UIData::MAX);		//vector配列メモリ確保用

const int OFFSET = 20;					//背景描画オフセット値s

const float MENU_OPENCLOSE_TIME = 0.5f;	//メニューの開閉時間

const Vector2 BACKGROND_STARTPOS = Vector2(0 + OFFSET, 0 + OFFSET);
const Vector2 BACKGROND_ENDPOS = Vector2(
	Application::SCREEN_SIZE_X - OFFSET,
	Application::SCREEN_SIZE_Y - OFFSET
);

const float ALPHA = 255.0f * 0.5f;

const float VOLUME_CHANGE_AMOUNT = 1.0f / 100.0f;

const unsigned int BG_COLOR = 0xaaaaaa;
const unsigned int MENU_FONT_COLOR = 0xffffff;

SettingScene::SettingScene(void)
	:
	SceneBase(),
	uiDataMng_(UIDataManager::GetInstance()),
	selectUIData_{},
	volumeData_{},
	idx_(0),
	max_(0),
	isChangeSoundVol_(false),
	sceneState_(SceneState::Open),
	stagingTime_(0.0f),
	bgmTextImgHandle_(-1),
	seTextImgHandle_(-1),
	cursorImgHandle_(-1),
	s60PgothicFontHandle_(-1)
{
	selectUIData_.reserve((size_t)RESERVE_DATA_NUM_MAX);

	InitSettingVolumeData(SRC_UIData::BGM_VOLUME);
	InitSettingVolumeData(SRC_UIData::SE_VOLUME);

	selectUIData_.resize(selectUIData_.size());

	max_ = (int)selectUIData_.size();

	bgmTextImgHandle_ = resMngImage_.Load(SRC_IMAGE::BGM_TEXT).GetHandleId();
	seTextImgHandle_ = resMngImage_.Load(SRC_IMAGE::SE_TEXT).GetHandleId();
	cursorImgHandle_ = resMngImage_.Load(SRC_IMAGE::Cursor).GetHandleId();

	s60PgothicFontHandle_ = resMngFont_.Load(SRC_FONT::Pgothic_s60).GetHandleId();

	InitSound();

	InitChangeSceneState();


	ChangeSceneState(SceneState::Open);
}

SettingScene::~SettingScene(void)
{
}

void SettingScene::Update(void)
{
	if (!sceneStateUpdate_)
	{
		return;
	}

	sceneStateUpdate_();
}

void SettingScene::Draw(void)
{
	if (!sceneStateDraw_)
	{
		return;
	}

	sceneStateDraw_();
}

void SettingScene::ASyncLoad(void)
{

}

void SettingScene::InitSound(void)
{
	//カーソル移動音
	SoundInfo soundInfo = SoundInfo();
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_MENU_CURSORMOVE);
	soundInfo.playType = DX_PLAYTYPE_BACK;

	soundController_->Add(SE_CURSORMOVE, soundInfo);

	//決定音
	soundInfo = SoundInfo();
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::SE_MENU_ENTER);
	soundInfo.playType = DX_PLAYTYPE_BACK;

	soundController_->Add(SE_ENTER, soundInfo);
}

void SettingScene::InitSettingVolumeData(const SRC_UIData srcUIData)
{
	auto volumeParBGM = GetUIDataToVariant<float>(srcUIData);
	selectUIData_.emplace_back(srcUIData);
	volumeData_.emplace(srcUIData, volumeParBGM);
}

void SettingScene::UpdateSoundVolume(void)
{
	MoveCursor();

	//選択されている種別
	const auto& src = selectUIData_[idx_];

	if (!volumeData_.contains(src))
	{
		//音量データが存在しない
		return;
	}

	float& volumePar = volumeData_.at(src);

	if (inputController_->IsPressed(ACTION::MENU_RIGHT))
	{
		soundController_->Play(SE_CURSORMOVE);

		//音量増加
		volumePar += VOLUME_CHANGE_AMOUNT;
		if (volumePar > 1.0f)
		{
			volumePar = 1.0f;
		}

		//UIDataに反映
		uiDataMng_.SetUIData<float>(src, volumePar);
		isChangeSoundVol_ = true;
	}
	else if (inputController_->IsPressed(ACTION::MENU_LEFT))
	{
		soundController_->Play(SE_CURSORMOVE);

		//音量低下
		volumePar -= VOLUME_CHANGE_AMOUNT;
		if (volumePar < 0.0f)
		{
			volumePar = 0.0f;
		}

		//UIDataに反映
		uiDataMng_.SetUIData<float>(src, volumePar);
		isChangeSoundVol_ = true;
	}

	if (isChangeSoundVol_)
	{
		//音量変化を通知
		isChangeSoundVol_ = false;
		observer_.SendMessageList(Message::SoundVolumeChange);
	}
}

void SettingScene::MoveCursor(void)
{
	//SEとBGM切り替え
	if (inputController_->IsTriggered(ACTION::MENU_UP))
	{
		soundController_->Play(SE_CURSORMOVE);
		--idx_;
		if (idx_ < 0)
		{
			idx_ = max_ - 1;
		}
	}
	else if (inputController_->IsTriggered(ACTION::MENU_DOWN))
	{
		soundController_->Play(SE_CURSORMOVE);
		++idx_;
		if (idx_ > max_ - 1)
		{
			idx_ = 0;
		}
	}
}

const bool SettingScene::IsEndStaging(void) const
{
	return stagingTime_ < 0.0f;
}

void SettingScene::InitChangeSceneState(void)
{
	sceneStateChange_.emplace(SceneState::Open, bind(&SettingScene::ChangeSceneState_Open, this));
	sceneStateChange_.emplace(SceneState::Menu, bind(&SettingScene::ChangeSceneState_Menu, this));
	sceneStateChange_.emplace(SceneState::Close, bind(&SettingScene::ChangeSceneState_Close, this));
}

void SettingScene::ChangeSceneState(const SceneState state)
{
	if (!sceneStateChange_.contains(state))
	{
		//状態遷移が登録されていない
		return;
	}

	sceneState_ = state;

	const auto& stateChange = sceneStateChange_.at(sceneState_);

	if (!stateChange)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	stateChange();
}

void SettingScene::ChangeSceneState_Open(void)
{
	stagingTime_ = MENU_OPENCLOSE_TIME;
	sceneStateUpdate_ = bind(&SettingScene::SceneStateUpdate_Open, this);
	sceneStateDraw_ = bind(&SettingScene::SceneStateDraw_Open, this);
}

void SettingScene::ChangeSceneState_Menu(void)
{
	sceneStateUpdate_ = bind(&SettingScene::SceneStateUpdate_Menu, this);
	sceneStateDraw_ = bind(&SettingScene::SceneStateDraw_Menu, this);
}

void SettingScene::ChangeSceneState_Close(void)
{
	stagingTime_ = MENU_OPENCLOSE_TIME;
	sceneStateUpdate_ = bind(&SettingScene::SceneStateUpdate_Close, this);
	sceneStateDraw_ = bind(&SettingScene::SceneStateDraw_Close, this);
}

void SettingScene::SceneStateUpdate_Open(void)
{
	auto deltaTime = TimeManager::GetInstance().GetDeltaTime();

	stagingTime_ -= deltaTime;

	if (IsEndStaging())
	{
		//メニュー画面描画開始
		ChangeSceneState(SceneState::Menu);
	}
}

void SettingScene::SceneStateUpdate_Menu(void)
{
	soundController_->Update();

	UpdateSoundVolume();

	if (inputController_->IsTriggered(ACTION::MENU_ENTER))
	{
		soundController_->Play(SE_ENTER);

		//設定画面を閉じる
		ChangeSceneState(SceneState::Close);
	}
}

void SettingScene::SceneStateUpdate_Close(void)
{
	auto deltaTime = TimeManager::GetInstance().GetDeltaTime();

	stagingTime_ -= deltaTime;

	if (IsEndStaging())
	{
		//設定画面を閉じる
		scnMng_.CloseSettingScene();
	}
}

void SettingScene::SceneStateDraw_Open(void)
{
	Vector2 boxSize = MathUtility::Lerp(
		BACKGROND_STARTPOS, BACKGROND_ENDPOS,
		(MENU_OPENCLOSE_TIME - stagingTime_) / MENU_OPENCLOSE_TIME
	);

	DrawBGBox(BACKGROND_STARTPOS, boxSize);
}

void SettingScene::SceneStateDraw_Menu(void)
{
	Vector2 bgmTextImgSize = {};
	Vector2 seTextImgSize = {};
	Vector2 cursorImgSize = {};

	GetGraphSize(bgmTextImgHandle_, &bgmTextImgSize.x, &bgmTextImgSize.y);
	GetGraphSize(seTextImgHandle_, &seTextImgSize.x, &seTextImgSize.y);
	GetGraphSize(cursorImgHandle_, &cursorImgSize.x, &cursorImgSize.y);

	DrawBGBox(BACKGROND_STARTPOS, BACKGROND_ENDPOS);

	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X / 2,
		Application::SCREEN_SIZE_HALF_Y / 2,
		1.0, 0.0, bgmTextImgHandle_, true
	);


	float bgmVol = volumeData_.at(SRC_UIData::BGM_VOLUME);
	bgmVol *= 100.0f;
	int fontSize = GetFontSizeToHandle(s60PgothicFontHandle_);
	int fontEdgeSize = GetFontEdgeSizeToHandle(s60PgothicFontHandle_);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_HALF_X /*+ bgmTextImgSize.x*/,
		(Application::SCREEN_SIZE_HALF_Y - fontSize + fontEdgeSize) / 2,
		MENU_FONT_COLOR, s60PgothicFontHandle_,
		"%.f", bgmVol
	);


	float seVol = volumeData_.at(SRC_UIData::SE_VOLUME);
	seVol *= 100.0f;

	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X / 2,
		Application::SCREEN_SIZE_HALF_Y,
		1.0, 0.0, seTextImgHandle_, true
	);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y - (fontSize + fontEdgeSize) / 2,
		MENU_FONT_COLOR, s60PgothicFontHandle_,
		"%.f", seVol
	);


	const int idxMax = (int)volumeData_.size();
	Vector2f sizeDivRate = {
		(float)bgmTextImgSize.x / (float)cursorImgSize.x,
		(float)bgmTextImgSize.y / (float)cursorImgSize.y
	};
	
	DrawRotaGraph3(
		Application::SCREEN_SIZE_HALF_X / 2,
		Application::SCREEN_SIZE_HALF_Y / (idxMax - idx_),
		cursorImgSize.x / 2, cursorImgSize.y / 2,
		(double)sizeDivRate.x, (double)sizeDivRate.y, 
		0.0, cursorImgHandle_, true
	);
}

void SettingScene::SceneStateDraw_Close(void)
{
	Vector2 boxSize = MathUtility::Lerp(
		BACKGROND_STARTPOS, BACKGROND_ENDPOS,
		stagingTime_ / MENU_OPENCLOSE_TIME
	);

	DrawBGBox(BACKGROND_STARTPOS, boxSize);
}

void SettingScene::DrawBGBox(const Vector2& boxStartPos, const Vector2& boxSize)
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)ALPHA);
	DrawBox(
		boxStartPos.x, boxStartPos.y,
		boxSize.x, boxSize.y,
		BG_COLOR, true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
