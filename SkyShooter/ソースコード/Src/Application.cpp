
#include <memory>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/Input/InputManager.h"
#include "Manager/ResourceManager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Manager/ActorsManager/Actor/ActorManager.h"
#include "Manager/ActorsManager/Charactor/CharactorManager.h"
#include "Manager/ActorsManager/Shot/ShotManager.h"
#include "Manager/ColliderManager.h"
#include "Manager/JSON/JSONManager.h"
#include "Manager/UIManager/UIManager.h"
#include "Manager/UIManager/UIDataManager.h"
#include "Manager/TimeManager.h"
#include "Manager/RandManager.h"
#include "Observer/Observer.h"
//#include "../Common/FpsControl.h"
#include "Application.h"

using namespace std;

const string Application::PATH_IMAGE = "Data/Image/";
const string Application::PATH_MODEL = "Data/Model/";
const string Application::PATH_EFFECT = "Data/Effect/";
const string Application::PATH_SHADER = "Data/Shader/";
const string Application::PATH_SHADER_PIXEL = "Data/Shader/Pixel/";
const string Application::PATH_SHADER_VERTEXS = "Data/Shader/Vertexs/";
const string Application::PATH_JSON = "Data/JSON/";
const string Application::PATH_SOUND = "Data/Sound/";
const string Application::PATH_FONT = "Data/Fonts/";

const int FRAMERATE = (1000/60);

Application* Singleton<Application>::instance_ = nullptr;

void Application::Run(void)
{
	auto& sceneMng = SceneManager::GetInstance();
	auto& observer = Observer::GetInstance();
	auto& inputMng = InputManager::GetInstance();

	//ゲームループ開始
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		
		currentTime_ = GetNowCount();
		if (currentTime_ - lastTime_ >= FRAMERATE)
		{
			lastTime_ = currentTime_;
			++frameCnt_;
		}

		observer.Update();
		inputMng.Update();
		sceneMng.Update();


		sceneMng.Draw();

		//fps計算と描画
		CalcFrameRate();
		//DrawFrameRateCnt();

		ScreenFlip();
	}
}

void Application::Destroy(void)
{
	SceneManager::GetInstance().Destroy();
	Observer::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	JSONManager::GetInstance().Destroy();
	InputManager::GetInstance().Destroy();
	RandManager::GetInstance().Destroy();
	TimeManager::GetInstance().Destroy();
	UIManager::GetInstance().Destroy();
	UIDataManager::GetInstance().Destroy();
	CharactorManager::GetInstance().Destroy();
	ActorManager::GetInstance().Destroy();
	ShotManager::GetInstance().Destroy();
	ColliderManager::GetInstance().Destroy();


	//Effekseerを終了する。
	Effkseer_End();

	//ルールで解放はDxLib_End()前にする必要がある
	//DxLib終了
	if (DxLib_End() == -1)
	{
		//Dxlib終了時エラー
		isReleaseFail_ = true;
	}

	delete instance_;
}

const bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

Application::Application(void)
	:
	isInitFail_(false),
	isReleaseFail_(false)
{	

	//アプリケーションの初期設定
	SetWindowText("SkyShooter");

	//ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	//SetGraphMode(1000, 750, 32);

	//ウィンドウモード
	//ChangeWindowMode(false);
	ChangeWindowMode(true);

	//DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		//Dxlibの初期化失敗
		isInitFail_ = true;
		return;
	}

	//Effekseerの初期化
	InitEffekseer();

	Effekseer_InitDistortion(1.0f);

	//キー制御初期化
	SetUseDirectInputFlag(true);

	//時間制御
	TimeManager::CreateInstace();

	//乱数制御
	RandManager::CreateInstace();
	
	//オブザーバー?
	Observer::CreateInstace();

	//入力制御
	InputManager::CreateInstace();

	//リソース管理
	ResourceManager::CreateInstace();

	//JSONデータ管理
	JSONManager::CreateInstace();

	//オブジェクト
	ActorManager::CreateInstace();

	//キャラクター
	CharactorManager::CreateInstace();

	//弾 : オブジェクトとくっつけてもいいかも
	ShotManager::CreateInstace();

	//衝突判定制御
	ColliderManager::CreateInstace();
	
	//UIデータ管理
	UIDataManager::CreateInstace();
	
	//UI管理
	UIManager::CreateInstace();

	//シーン管理
	SceneManager::CreateInstace();
}

Application::~Application(void)
{
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init (8000) == -1)
	{
		//Effekseerの初期化失敗
		DxLib_End();
	}

	// 画面モード変更時( とウインドウモード変更時 )に
	// グラフィックスシステムの設定やグラフィックハンドルをリセットしないよう設定
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

void Application::CalcFrameRate(void)
{
	int difTime = currentTime_ - updateFrameRateTime_;

	if (difTime > 1000)
	{
		float fFrameCnt = (float)(frameCnt_ * 1000);

		frameRate_ = fFrameCnt / difTime;

		frameCnt_ = 0;

		updateFrameRateTime_ = currentTime_;
	}
}

void Application::DrawFrameRateCnt(void)
{
	DrawFormatString(Application::SCREEN_SIZE_HALF_X, 0, 0xffffff,
		"fps = %.2f", frameRate_);
}

