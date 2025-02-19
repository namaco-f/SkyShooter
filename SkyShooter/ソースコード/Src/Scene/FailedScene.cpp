
#include <string>
#include <DxLib.h>
#include "../Application.h"
//#include "../Utility/MathUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/RandManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
//#include "../Manager/Resource/Image/ImageResourcesManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/Input/InputController.h"
#include "../Manager/JSON/JSONManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/Camera.h"
#include "../Utility/DebugUtility.h"
#include "../Common/Grid3d.h"
#include "../Observer/Observer.h"
#include "../Sound/SoundController.h"
#include "FailedScene.h"

using namespace std;

const string BGM_FAILED = "BGM_Failed";

FailedScene::FailedScene(void)
{
	observer_.ClearAllMessage();
	camera_.ChangeMode(Camera::CAMERA_MODE::FIXED_POINT);

	resMngSound_.Load(SRC_SOUND::BGM_FAILED);

	InitSound();

	soundController_->Play(BGM_FAILED);
}

FailedScene::~FailedScene(void)
{
}

void FailedScene::Init(void)
{
}

void FailedScene::Update(void)
{
	if (inputController_->IsTriggered(ACTION::MENU_ENTER))
	{
		//ゲームをリトライ
		scnMng_.RetryGame();
	}
	
	if (inputController_->IsTriggered(ACTION::MENU_NO))
	{
		//タイトルに戻る
		scnMng_.ReturnTitleScene();
	}
}

void FailedScene::Draw(void)
{
	auto& debug = scnMng_.GetDebugUtility();
	debug.DebugStringFormat(0, "FailedScene", 0x00ffff);
}

void FailedScene::ASyncLoad(void)
{
}

void FailedScene::InitSound(void)
{
	SoundInfo soundInfo = SoundInfo(
		resMngSound_.LoadSoundDuplicate(SRC_SOUND::BGM_FAILED),
		SoundType::BGM
	);

	soundController_->Add(BGM_FAILED, soundInfo);
}

void FailedScene::DrawDebugUI(void)
{
}

void FailedScene::DrawDebug3D(void)
{
}