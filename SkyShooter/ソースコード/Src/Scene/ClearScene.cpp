
#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../UI/GetUIDataToVariantTemplate.h"
//#include "../Utility/MathUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/RandManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../Manager/ResourceManager/Font/FontResourcesManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ActorsManager/Actor/ActorManager.h"
#include "../Object/Actor/ActorFactory.h"
#include "../Object/Actor/Stage/SkyDome.h"
#include "../Object/Actor/Stage/Ground.h"
#include "../Manager/Input/InputController.h"
#include "../Manager/JSON/JSONManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/Camera.h"
#include "../Utility/DebugUtility.h"
#include "../Common/Grid3d.h"
#include "../Observer/Observer.h"
#include "../Sound/SoundController.h"
#include "ClearScene.h"

using namespace std;

const int OFFSET = 20;					//”wŒi•`‰æƒIƒtƒZƒbƒg’ls

const Vector2 BACKGROND_STARTPOS = Vector2(0 + OFFSET, 0 + OFFSET);
const Vector2 BACKGROND_ENDPOS = Vector2(
	Application::SCREEN_SIZE_X - OFFSET,
	Application::SCREEN_SIZE_Y - OFFSET
);

const string BGM_CLEAR = "BGM_Clear";
const string CHANGE_TITLESCENE = "ƒ^ƒCƒgƒ‹‚É–ß‚é";

const string DRAW_CLEAR_TIME_TEMP = "%d : %d : %d";

const float ALPHA = 255.0f * 0.5f;

const int CLEAR_BG_OFFSET_X = 50;
const int CLEAR_BG_OFFSET_Y = 50;
const int CLEAR_BGFRAME_OFFSET_X = 15;
const int CLEAR_BGFRAME_OFFSET_Y = 15;

const unsigned int COLOR_BG = 0x000000;
const unsigned int COLOR_BG_FRAME = 0xffffff;
const unsigned int COLOR_CLEARTIME_FONT = 0xffff00;
const unsigned int COLOR_MENU_FONT = 0xffffff;

ClearScene::ClearScene(void)
	:
	timeMng_(TimeManager::GetInstance()),
	clearTime_(GetUIDataToVariant<float>(SRC_UIData::CLEAR_TIME)),
	clearTimeUnit_(timeMng_.GetTime_Unit(clearTime_))
{
	//”wŒiF‚ðÝ’è
	SetBackgroundColor(255, 255, 255);

	observer_.ClearAllMessage();
	camera_.ChangeMode(Camera::CAMERA_MODE::FIXED_POINT);

	resMngSound_.Load(SRC_SOUND::BGM_CLEAR);

	s60FontHandle_ = resMngFont_.Load(SRC_FONT::Pgothic_s60).GetHandleId();
	s40FontHandle_ = resMngFont_.Load(SRC_FONT::Pgothic_s40).GetHandleId();
	
	Init();

	InitSound();

	soundController_->Play(BGM_CLEAR);

	ChangeSceneState(SceneState::Result);
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Init(void)
{
	actorMng_.AddActor([&] {return CreateActor<Ground>(); });
	actorMng_.AddActor([&] {return CreateActor<SkyDome>(); });

	sceneStateChange_.emplace(SceneState::Result, bind(&ClearScene::ChangeSceneState_Result, this));
	sceneStateChange_.emplace(SceneState::Menu, bind(&ClearScene::ChangeSceneState_Menu, this));

	cursorHandle_ = resMngImage_.Load(SRC_IMAGE::Cursor).GetHandleId();
	clearTextHandle_ = resMngImage_.Load(SRC_IMAGE::ClearText).GetHandleId();
}

void ClearScene::Update(void)
{
	for (const weak_ptr<ActorBase> actor : actorMng_.GetActors())
	{
		if (actor.expired())
		{
			continue;
		}
		actor.lock()->Update();
	}

	if (!sceneStateUpdate_)
	{
		return;
	}

	sceneStateUpdate_();
}

void ClearScene::Draw(void)
{
	for (const weak_ptr<ActorBase> actor : actorMng_.GetActors())
	{
		if (actor.expired())
		{
			continue;
		}
		actor.lock()->Draw();
	}

	if (!sceneStateDraw_)
	{
		return;
	}

	sceneStateDraw_();
}

void ClearScene::ASyncLoad(void)
{
}

void ClearScene::InitSound(void)
{
	SoundInfo soundInfo = SoundInfo(
		resMngSound_.LoadSoundDuplicate(SRC_SOUND::BGM_CLEAR),
		SoundType::BGM
	);

	soundController_->Add(BGM_CLEAR, soundInfo);
}

void ClearScene::DrawDebugUI(void)
{
	auto& debug = scnMng_.GetDebugUtility();

	debug.DebugStringFormat(0, "•ª = %d", 0x00ffff, clearTimeUnit_.minutes);
	debug.DebugStringFormat(0, "•b = %d", 0x00ffff, clearTimeUnit_.seconds);
	debug.DebugStringFormat(0, "ƒ~ƒŠ•b = %d", 0x00ffff, clearTimeUnit_.milliSeconds);
}

void ClearScene::DrawDebug3D(void)
{
}

void ClearScene::DrawClearTime(void)
{
	int fontSize = GetFontSizeToHandle(s60FontHandle_);


	int strLen = GetDrawFormatStringWidthToHandle(
		s60FontHandle_,
		DRAW_CLEAR_TIME_TEMP.c_str(),
		clearTimeUnit_.minutes,
		clearTimeUnit_.seconds,
		clearTimeUnit_.milliSeconds
	);

	Vector2 BGPosStart = {
		Application::SCREEN_SIZE_HALF_X - (strLen / 2) - CLEAR_BG_OFFSET_X,
		Application::SCREEN_SIZE_HALF_Y - Application::SCREEN_SIZE_HALF_Y / 4 - CLEAR_BG_OFFSET_Y
	};
	Vector2 BGPosEnd = {
		Application::SCREEN_SIZE_HALF_X + (strLen / 2) + CLEAR_BG_OFFSET_X,
		Application::SCREEN_SIZE_HALF_Y - Application::SCREEN_SIZE_HALF_Y / 4 + fontSize + CLEAR_BG_OFFSET_Y
	};

	DrawBox(
		BGPosStart.x - CLEAR_BGFRAME_OFFSET_X, BGPosStart.y - CLEAR_BGFRAME_OFFSET_Y,
		BGPosEnd.x + CLEAR_BGFRAME_OFFSET_X, BGPosEnd.y + CLEAR_BGFRAME_OFFSET_Y,
		COLOR_BG_FRAME, true
	);

	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)ALPHA);
	DrawBox(
		BGPosStart.x,BGPosStart.y,
		BGPosEnd.x,BGPosEnd.y,
		COLOR_BG, true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_HALF_X - (strLen / 2),
		Application::SCREEN_SIZE_HALF_Y - Application::SCREEN_SIZE_HALF_Y / 4,
		COLOR_CLEARTIME_FONT, s60FontHandle_,
		DRAW_CLEAR_TIME_TEMP.c_str(),
		clearTimeUnit_.minutes,
		clearTimeUnit_.seconds,
		clearTimeUnit_.milliSeconds
	);
}

void ClearScene::ChangeSceneState(const SceneState state)
{
	if (!sceneStateChange_.contains(state))
	{
		//ó‘Ô‘JˆÚ‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
		return;
	}

	sceneState_ = state;

	auto& stateChange = sceneStateChange_.at(sceneState_);
	if (!stateChange)
	{
		//ŠÖ”ƒIƒuƒWƒFƒNƒg‚ªƒŠƒ“ƒNØ‚ê
		return;
	}

	stateChange();
}

void ClearScene::ChangeSceneState_Result(void)
{
	sceneStateUpdate_ = bind(&ClearScene::SceneStateUpdate_Result, this);
	sceneStateDraw_ = bind(&ClearScene::SceneStateDraw_Result, this);
}

void ClearScene::ChangeSceneState_Menu(void)
{
	sceneStateUpdate_ = bind(&ClearScene::SceneStateUpdate_Menu, this);
	sceneStateDraw_ = bind(&ClearScene::SceneStateDraw_Menu, this);
}

void ClearScene::SceneStateUpdate_Result(void)
{
	if (inputController_->IsTriggered(ACTION::MENU_ENTER))
	{
		ChangeSceneState(SceneState::Menu);
	}
}

void ClearScene::SceneStateUpdate_Menu(void)
{
	if (inputController_->IsTriggered(ACTION::MENU_ENTER))
	{
		//ƒ^ƒCƒgƒ‹‚É–ß‚é
		scnMng_.ReturnTitleScene();
	}
}

void ClearScene::SceneStateDraw_Result(void)
{
	DrawBGBox(BACKGROND_STARTPOS, BACKGROND_ENDPOS);

	DrawClearTime();

	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y / 2,
		1.0, 0.0, clearTextHandle_, true
	);
}

void ClearScene::SceneStateDraw_Menu(void)
{
	DrawBGBox(BACKGROND_STARTPOS, BACKGROND_ENDPOS);

	DrawClearTime();

	int strLen = GetDrawFormatStringWidthToHandle(s40FontHandle_, CHANGE_TITLESCENE.c_str());

	Vector2 size = Vector2();
	GetGraphSize(cursorHandle_, &size.x, &size.y);

	int fontSize = GetFontSizeToHandle(s40FontHandle_);

	double sizeScaleX = (double)strLen * 1.5 / (double)size.x;
	double sizeScaleY = (double)fontSize * 1.5 / (double)size.y;

	DrawRotaGraph3(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y + Application::SCREEN_SIZE_HALF_Y * 3 / 5 + fontSize / 2,
		size.x / 2, size.y / 2,
		sizeScaleX, sizeScaleY, 0.0, cursorHandle_, true
	);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_HALF_X - (strLen / 2),
		Application::SCREEN_SIZE_HALF_Y + Application::SCREEN_SIZE_HALF_Y * 3 / 5,
		COLOR_MENU_FONT, s40FontHandle_, CHANGE_TITLESCENE.c_str()
	);

	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y / 2,
		1.0, 0.0, clearTextHandle_, true
	);
}

void ClearScene::DrawBGBox(const Vector2& boxStartPos, const Vector2& boxSize)
{
	float par = 255.0f * 0.5f;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)par);
	DrawBox(
		boxStartPos.x, boxStartPos.y,
		boxSize.x, boxSize.y,
		COLOR_BG, true
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
