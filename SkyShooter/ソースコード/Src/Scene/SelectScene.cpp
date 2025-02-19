
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../Manager/ResourceManager/Font/FontResourcesManager.h"
#include "../Manager/Input/InputManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/Input/InputController.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "../Utility/DebugUtility.h"
#include "../Utility/MathUtility.h"
#include "../Sound/SoundController.h"
#include "SelectScene.h"

using namespace std;

const int OFFSET = 20;					//背景描画オフセット値

const double DEF_IMG_SCALE = 0.2;		//操作方法選択画像の初期スケール
const double SELECT_IMG_SCALE = 0.25;	//操作方法選択画像の選択時画像スケール

const float MENU_OPENCLOSE_TIME = 0.5f;	//メニューの開閉時間

const Vector2 BACKGROND_STARTPOS = Vector2(0 + OFFSET, 0 + OFFSET);
const Vector2 BACKGROND_ENDPOS = Vector2(
	Application::SCREEN_SIZE_X - OFFSET,
	Application::SCREEN_SIZE_Y - OFFSET
);

const string PLAY_WITH_CONTROLLER_MESSAGE = "操作方法の選択";

const unsigned int COLOR_MENU_FONT = 0xffffff;
const unsigned int COLOR_MENU_BG = 0xffffff;

const Vector2f IMAGE_SIZE_SCALE_SELECT_CONTROLLER = { 1.5f ,1.25f };

const float ALPHA = 255.0f * 0.5f;

SelectScene::SelectScene(void)
	:
	SceneBase(),
	uiDataMng_(UIDataManager::GetInstance()),
	idx_(0),
	max_(2),
	fontHandle_(-1),
	controller_(SelectControllerImgData(DEF_IMG_SCALE)),
	keyboard_(SelectControllerImgData(DEF_IMG_SCALE)),
	stagingTime_(0.0f)
{
	controller_.handle = resMngImage_.Load(SRC_IMAGE::Controller).GetHandleId();
	keyboard_.handle = resMngImage_.Load(SRC_IMAGE::Keyboard_Mouse).GetHandleId();
	cursorHandle_ = resMngImage_.Load(SRC_IMAGE::Cursor).GetHandleId();

	//音
	resMngSound_.Load(SRC_SOUND::SE_MENU_CURSORMOVE);
	resMngSound_.Load(SRC_SOUND::SE_MENU_ENTER);

	//フォントハンドル
	fontHandle_ = resMngFont_.Load(SRC_FONT::Pgothic_s40).GetHandleId();

	InitSound();

	InitChangeSceneState();
	ChangeSceneState(SceneState::Open);
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Update(void)
{
	if (!sceneStateUpdate_)
	{
		return;
	}

	sceneStateUpdate_();
}

void SelectScene::Draw(void)
{
	if (!sceneStateDraw_)
	{
		return;
	}

	sceneStateDraw_();
}

void SelectScene::ASyncLoad(void)
{
}

void SelectScene::InitSound(void)
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
	soundController_->Update();
}

void SelectScene::MoveCursor(void)
{
	if (inputController_->IsTriggered(ACTION::MENU_LEFT))
	{
		soundController_->Play(SE_CURSORMOVE);
		--idx_;
		if (idx_ < 0)
		{
			//カーソル移動下限
			idx_ = 0;
		}
	}
	else if (inputController_->IsTriggered(ACTION::MENU_RIGHT))
	{
		soundController_->Play(SE_CURSORMOVE);
		++idx_;
		if (idx_ > max_)
		{
			//カーソル移動上限
			idx_ = max_;
		}
	}
}

void SelectScene::DrawDebug(void)
{
	auto& debug = scnMng_.GetDebugUtility();

	debug.DebugStringFormat(0, "SelectScene", 0x00ffff);
	debug.DebugStringFormat(0, "idx = %d", 0x00ffff, idx_);
}

bool SelectScene::PlayWithController(void) const
{
	return idx_ == 0;
}

void SelectScene::InitChangeSceneState(void)
{
	sceneStateChange_.emplace(SceneState::Open, bind(&SelectScene::ChangeSceneState_Open, this));
	sceneStateChange_.emplace(SceneState::Menu, bind(&SelectScene::ChangeSceneState_Menu, this));
	sceneStateChange_.emplace(SceneState::Close, bind(&SelectScene::ChangeSceneState_Close, this));
}

void SelectScene::ChangeSceneState(const SceneState state)
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

void SelectScene::ChangeSceneState_Open(void)
{
	stagingTime_ = MENU_OPENCLOSE_TIME;

	soundController_->Play(SE_ENTER);

	sceneStateUpdate_ = bind(&SelectScene::SceneStateUpdate_Open, this);
	sceneStateDraw_ = bind(&SelectScene::SceneStateDraw_Open, this);
}

void SelectScene::ChangeSceneState_Menu(void)
{
	sceneStateUpdate_ = bind(&SelectScene::SceneStateUpdate_Menu, this);
	sceneStateDraw_ = bind(&SelectScene::SceneStateDraw_Menu, this);
}

void SelectScene::ChangeSceneState_Close(void)
{
	stagingTime_ = MENU_OPENCLOSE_TIME;

	soundController_->Play(SE_ENTER);
	
	sceneStateUpdate_ = bind(&SelectScene::SceneStateUpdate_Close, this);
	sceneStateDraw_ = bind(&SelectScene::SceneStateDraw_Close, this);
}

void SelectScene::SceneStateUpdate_Open(void)
{
	auto deltaTime = TimeManager::GetInstance().GetDeltaTime();

	stagingTime_ -= deltaTime;

	if (stagingTime_ < 0.0f)
	{
		ChangeSceneState(SceneState::Menu);
	}
}

void SelectScene::SceneStateUpdate_Menu(void)
{
	//カーソル移動
	MoveCursor();

	if (inputController_->IsTriggered(ACTION::MENU_ENTER))
	{
		auto& inputMng = InputManager::GetInstance();

		if (PlayWithController())
		{
			//コントローラー操作に設定
			inputMng.SetControlType(CONTROL_TYPE::CONTROLLER);
		}
		else
		{
			//キーマウ操作に設定
			inputMng.SetControlType(CONTROL_TYPE::KEY);
		}

		//画面閉
		ChangeSceneState(SceneState::Close);
	}
}

void SelectScene::SceneStateUpdate_Close(void)
{
	auto deltaTime = TimeManager::GetInstance().GetDeltaTime();

	stagingTime_ -= deltaTime;

	if (stagingTime_ < 0.0f)
	{
		//設定画面を閉じる
		scnMng_.CloseSelectScene();
	}
}

void SelectScene::SceneStateDraw_Open(void)
{
	Vector2 boxSize = MathUtility::Lerp(
		BACKGROND_STARTPOS,BACKGROND_ENDPOS, 
		(MENU_OPENCLOSE_TIME - stagingTime_) / MENU_OPENCLOSE_TIME
	);

	DrawBGBox(BACKGROND_STARTPOS, boxSize);
}

void SelectScene::SceneStateDraw_Menu(void)
{
	Vector2 selectImgPos = Vector2();
	Vector2 selectImgSize = Vector2();
	Vector2f selectImgSizeScale = Vector2f();
	GetGraphSize(cursorHandle_, &selectImgSize.x, &selectImgSize.y);

	DrawBGBox(BACKGROND_STARTPOS, BACKGROND_ENDPOS);

	if (PlayWithController())
	{
		//コントローラー画像を拡大
		controller_.ChangeScale(SELECT_IMG_SCALE);
		keyboard_.ResetScale();
		selectImgPos = {
			Application::SCREEN_SIZE_HALF_X / 2,
			Application::SCREEN_SIZE_HALF_Y
		};
		selectImgSizeScale = IMAGE_SIZE_SCALE_SELECT_CONTROLLER;
	}
	else
	{
		//キーボード画像を拡大
		keyboard_.ChangeScale(SELECT_IMG_SCALE);
		controller_.ResetScale();
		selectImgPos = {
			Application::SCREEN_SIZE_HALF_X + Application::SCREEN_SIZE_HALF_X / 2,
			Application::SCREEN_SIZE_HALF_Y
		};
		selectImgSizeScale = IMAGE_SIZE_SCALE_SELECT_CONTROLLER;
	}

	DrawRotaGraph3(
		selectImgPos.x,selectImgPos.y,
		selectImgSize.x / 2, selectImgSize.y / 2,
		(double)selectImgSizeScale.x, (double)selectImgSizeScale.y,
		0.0, cursorHandle_, true
	);

	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X / 2,
		Application::SCREEN_SIZE_HALF_Y,
		controller_.scl, 0.0, controller_.handle, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X + Application::SCREEN_SIZE_HALF_X / 2,
		Application::SCREEN_SIZE_HALF_Y,
		keyboard_.scl, 0.0, keyboard_.handle, true);

	int strLen = GetDrawFormatStringWidthToHandle(
		fontHandle_,
		PLAY_WITH_CONTROLLER_MESSAGE.c_str()
		);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_HALF_X - (strLen / 2),
		Application::SCREEN_SIZE_HALF_Y / 8,
		COLOR_MENU_FONT, fontHandle_, PLAY_WITH_CONTROLLER_MESSAGE.c_str()
	);
}

void SelectScene::SceneStateDraw_Close(void)
{
	Vector2 boxSize = MathUtility::Lerp(
		BACKGROND_STARTPOS, BACKGROND_ENDPOS,
		stagingTime_ / MENU_OPENCLOSE_TIME
	);

	DrawBGBox(BACKGROND_STARTPOS, boxSize);
}

void SelectScene::DrawBGBox(const Vector2& boxStartPos, const Vector2& boxSize)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)ALPHA);
	DrawBox(
		boxStartPos.x, boxStartPos.y,
		boxSize.x, boxSize.y,
		COLOR_MENU_BG, true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
