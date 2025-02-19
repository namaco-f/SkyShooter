#include <string>
#include <DxLib.h>
#include "../Application.h"
//#include "../Utility/MathUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/RandManager.h"
#include "../Manager/ActorsManager/Actor/ActorManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../Manager/ResourceManager/Font/FontResourcesManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/Input/InputController.h"
#include "../Manager/JSON/JSONManager.h"
#include "../Sound/SoundController.h"
#include "../Manager/TimeManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage/SkyDome.h"
#include "../Object/Actor/Stage/Ground.h"
#include "../Utility/DebugUtility.h"
#include "../Common/Grid3d.h"
#include "../Object/Actor/ActorFactory.h"
#include "TitleScene.h"

using namespace std;

using ScnMng = SceneManager;
using CameraMode = Camera::CAMERA_MODE;

const double DEF_ROTAGRAPH_EXTEND_RATE = 1.0;		//RotaGraph使用時のデフォルトの拡大率
const double DEF_ROTAGRAPH_ANGLE_RATE = 0.0;		//RotaGraph使用時のデフォルトの回転

const string BGM_TITLE = "BGM_Title";

const string START_MESSAGE = "PUSH START";
const string GAMESTART_MESSAGE = "GAME START";

TitleScene::TitleScene(void)
	:
	SceneBase(),
	titleLogoHandle_(-1),
	cursorHandle_(-1),
	s50FontHandle_(-1),
	sceneState_(SceneState::Start)
{
	InitChangeState();

	ChangeState(State::Load);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	actorMng_.AddActor([&] {return CreateActor<Ground>(); });
	actorMng_.AddActor([&] {return CreateActor<SkyDome>(); });

	sceneStateChange_.emplace(SceneState::Start, bind(&TitleScene::ChangeSceneState_Start, this));
	sceneStateChange_.emplace(SceneState::Menu, bind(&TitleScene::ChangeSceneState_Menu, this));
}

void TitleScene::Update(void)
{
	if (!stateUpdate_)
	{
		return;
	}

	stateUpdate_();
}

void TitleScene::Draw(void)
{
	if (!stateDraw_)
	{
		return;
	}

	stateDraw_();
}

void TitleScene::ASyncLoad(void)
{
}

void TitleScene::InitSound(void)
{
	SoundInfo soundInfo = SoundInfo();
	soundInfo.soundType = SoundType::BGM;
	soundInfo.handle = resMngSound_.LoadSoundDuplicate(SRC_SOUND::BGM_TITLE);

	soundController_->Add(BGM_TITLE, soundInfo);
}

void TitleScene::Draw2D(void)
{
}

void TitleScene::DrawTitleLogo(void)
{
	//タイトルロゴ
	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y / 2,
		DEF_ROTAGRAPH_EXTEND_RATE,
		DEF_ROTAGRAPH_ANGLE_RATE,
		titleLogoHandle_, true);
}

void TitleScene::DrawDebugUI(void)
{
	auto& debugUtility = scnMng_.GetDebugUtility();
	auto& randMng = RandManager::GetInstance();
	
	auto& timeMng = TimeManager::GetInstance();
	auto sceneActiveTime = timeMng.GetTime();
	auto deltaTime = timeMng.GetDeltaTime();
	Vector2f times = { sceneActiveTime ,deltaTime };

	debugUtility.DebugStringFormat(0, "TitleScene",0xffffff);
	
	debugUtility.DebugStringFormat(0, "Times = %.2f,%.5f",0xffffff, times);
}

void TitleScene::DrawDebug3D(void)
{
	grid_->Draw();
}

void TitleScene::ChangeSceneState(const SceneState state)
{
	if (!sceneStateChange_.contains(state))
	{
		//状態遷移が登録されていない
		return;
	}

	sceneState_ = state;

	auto& stateChange = sceneStateChange_.at(sceneState_);
	if (!stateChange)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	stateChange();
}

void TitleScene::ChangeSceneState_Start(void)
{
	sceneStateUpdate_ = bind(&TitleScene::SceneStateUpdate_Start, this);
	sceneStateDraw_ = bind(&TitleScene::SceneStateDraw_Start, this);
}

void TitleScene::ChangeSceneState_Menu(void)
{
	sceneStateUpdate_ = bind(&TitleScene::SceneStateUpdate_Menu, this);
	sceneStateDraw_ = bind(&TitleScene::SceneStateDraw_Menu, this);
}

void TitleScene::SceneStateUpdate_Start(void)
{
	if (inputController_->IsTriggered(ACTION::GAME_START))
	{
		ChangeSceneState(SceneState::Menu);
	}
}

void TitleScene::SceneStateUpdate_Menu(void)
{
	if (inputController_->IsTriggered(ACTION::GAME_START))
	{
		scnMng_.OpenSelectScene();
	}

	if (inputController_->IsTriggered(ACTION::MENU_OPEN))
	{
		scnMng_.OpenSettingScene();
	}
}

void TitleScene::SceneStateDraw_Start(void)
{
	DrawTitleLogo();

	int strLen = GetDrawFormatStringWidthToHandle(s50FontHandle_, START_MESSAGE.c_str());

	Vector2 size = Vector2();
	GetGraphSize(cursorHandle_, &size.x, &size.y);

	int fontSize = GetFontSizeToHandle(s50FontHandle_);

	double sizeScaleX = (double)strLen * 1.5 / (double)size.x;
	double sizeScaleY = (double)fontSize * 1.5 / (double)size.y;

	DrawRotaGraph3(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y + fontSize / 2,
		size.x / 2, size.y / 2,
		sizeScaleX, sizeScaleY, 0.0, cursorHandle_, true
	);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_HALF_X - (strLen / 2),
		Application::SCREEN_SIZE_HALF_Y,
		0xffffff, s50FontHandle_, START_MESSAGE.c_str());
}

void TitleScene::SceneStateDraw_Menu(void)
{
	DrawTitleLogo();

	int strLen = GetDrawFormatStringWidthToHandle(s50FontHandle_, GAMESTART_MESSAGE.c_str());

	Vector2 size = Vector2();
	GetGraphSize(cursorHandle_, &size.x, &size.y);

	int fontSize = GetFontSizeToHandle(s50FontHandle_);

	double sizeScaleX = (double)strLen * 1.5 / (double)size.x;
	double sizeScaleY = (double)fontSize * 1.5 / (double)size.y;

	DrawRotaGraph3(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y + fontSize / 2,
		size.x / 2, size.y / 2,
		sizeScaleX, sizeScaleY, 0.0, cursorHandle_, true
	);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_HALF_X - (strLen / 2),
		Application::SCREEN_SIZE_HALF_Y,
		0xffffff, s50FontHandle_, GAMESTART_MESSAGE.c_str());
}

void TitleScene::InitChangeState(void)
{
	stateChange_.emplace(State::Load, bind(&TitleScene::ChangeState_Load, this));
	stateChange_.emplace(State::Play, bind(&TitleScene::ChangeState_Play, this));
	stateChange_.emplace(State::End, bind(&TitleScene::ChangeState_End, this));
}

void TitleScene::ChangeState_Load(void)
{
	stateUpdate_ = bind(&TitleScene::StateUpdate_Load, this);
	stateDraw_ = bind(&TitleScene::StateDraw_Load, this);

	resMng_.Load(SCENE_ID::TITLE);

	ChangeLoadState(LoadState::IsAsyncLoad);
}

void TitleScene::ChangeState_Play(void)
{
	Init();

	camera_.ChangeMode(CameraMode::FIXED_POINT);

	titleLogoHandle_ = resMngImage_.Load(SRC_IMAGE::TitleLogo).GetHandleId();

	s50FontHandle_ = resMngFont_.Load(SRC_FONT::Pgothic_s50).GetHandleId();

	cursorHandle_ = resMngImage_.Load(SRC_IMAGE::Cursor).GetHandleId();

	resMngSound_.Load(SRC_SOUND::BGM_TITLE);

	InitSound();

	soundController_->Play(BGM_TITLE);

	stateUpdate_ = bind(&TitleScene::StateUpdate_Play, this);
	stateDraw_ = bind(&TitleScene::StateDraw_Play, this);

	ChangeSceneState(SceneState::Start);
}

void TitleScene::ChangeState_End(void)
{
	stateUpdate_ = bind(&TitleScene::StateUpdate_End, this);
	stateDraw_ = bind(&TitleScene::StateDraw_End, this);
}

void TitleScene::StateUpdate_Load(void)
{
	if (!loadStateUpdate_)
	{
		return;
	}

	loadStateUpdate_();
}

void TitleScene::StateUpdate_Play(void)
{
	const auto& actors = actorMng_.GetActors();
	for (const weak_ptr<ActorBase> actor : actors)
	{
		if (actor.expired())
		{
			//実体が存在していない
			continue;
		}
		actor.lock()->Update();
	}

	if (!sceneStateUpdate_)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	sceneStateUpdate_();
}

void TitleScene::StateUpdate_End(void)
{
}

void TitleScene::StateDraw_Load(void)
{
	if (!loadStateDraw_)
	{
		return;
	}

	loadStateDraw_();
}

void TitleScene::StateDraw_Play(void)
{
	const auto& actors = actorMng_.GetActors();
	for (const weak_ptr<ActorBase> actor : actors)
	{
		if (actor.expired())
		{
			//実体が存在していない
			continue;
		}
		actor.lock()->Draw();
	}

	if (!sceneStateDraw_)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	sceneStateDraw_();

	//DrawDebugUI();
}

void TitleScene::StateDraw_End(void)
{
}

