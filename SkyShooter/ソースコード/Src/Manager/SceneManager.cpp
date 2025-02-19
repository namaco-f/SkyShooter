#include <chrono>
//#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "TimeManager.h"
#include "ColliderManager.h"
#include "ResourceManager/ResourceManager.h"
#include "UIManager/UIManager.h"
#include "ActorsManager/Actor/ActorManager.h"
#include "ActorsManager/Charactor/CharactorManager.h"
#include "ActorsManager/Shot/ShotManager.h"
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/SettingScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/ClearScene.h"
#include "../Scene/FailedScene.h"
#include "Camera.h"
#include "../Utility/DebugUtility.h"
#include "SceneManager.h"


using namespace std;

const Vector2f FOG_STARTEND = { 800.0f, 10000.0f };		//フォグの開始終了距離

int COLOR_RED = 0;
int COLOR_GREEN = 1;
int COLOR_BLUE = 2;

const int FOG_COLOR[3] = { 127, 200, 255 };				//フォグの色

const int DEF_BACKCOLOR[3] = { 255,255,255 };			//デフォルトの背景色

SceneManager* Singleton<SceneManager>::instance_ = nullptr;

void SceneManager::Init(void)
{
	//SceneManagerの参照を使用しているため
	//実体生成後に実行
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Update(void)
{
	auto& timeMng = TimeManager::GetInstance();
	auto& colMng = ColliderManager::GetInstance();

	if (scenes_.empty())
	{
		//シーンが存在しない
		return;
	}

	timeMng.Update();

	camera_->Update();

	fader_->Update();
	if (IsSceneChanging())
	{	
		//シーン遷移中
		Fade();
	}
	else
	{
		//衝突判定更新
		colMng.UpdateCollider();
		
		for (const auto& scene : scenes_)
		{
			//音量更新
			scene->UpdateSound();
		}

		//最後に追加されているシーンのみ更新
		scenes_.back()->Update();
	}
}

void SceneManager::Draw(void)
{
	//デバッグ用
	debugUtility_->DebugStringCntReset();

	//描画先グラフィック領域の指定
	//３Ｄ描画で使用するカメラの設定などがリセットされる
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	camera_->SetBeforeDraw();

	//描画
	for (const auto& scene : scenes_)
	{
		scene->Draw();
	}

	//Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();

	//暗転・明転
	fader_->Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawGraph(0, 0, mainScreen_,true);
}

void SceneManager::ChangeScene(const SCENE_ID nextId)
{
	//フェード処理が終わってからシーンを変える場合もあるため、
	//遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	//フェードアウト(暗転)を開始する
	fader_->ChangeState(Fader::State::FADE_OUT);
	isSceneChanging_ = true;
}

SceneManager::SceneManager(void)
	:
	debugUtility_(make_unique<DebugUtility>()),
	fader_(make_unique<Fader>()),
	scenes_{},
	camera_(make_unique<Camera>()),
	sceneId_(SCENE_ID::TITLE),
	waitSceneId_(SCENE_ID::NONE),
	isSceneChanging_(false),
	mainScreen_(MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true)),
	sceneChanges_{}
{
	InitSceneChange();

	Init3D();

	//カメラモードを自由視点に変更
	camera_->ChangeMode(Camera::CAMERA_MODE::FREE);
}

SceneManager::~SceneManager(void)
{
	DeleteGraph(mainScreen_);

	sceneChanges_.clear();
}

void SceneManager::DoChangeScene(const SCENE_ID sceneId)
{
	if (IsSceneError(sceneId))
	{
		return;
	}

	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放
	if (!scenes_.empty())
	{
		scenes_.pop_front();
	}

	sceneChanges_[sceneId_]();

	scenes_.back()->Init();

	TimeManager::GetInstance().ResetDeltaTime();
	
	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::OpenSettingScene(void)
{
	//カメラ操作を無効化
	camera_->CameraMoveLock(true);

	//設定画面生成
	scenes_.emplace_back(move(make_unique<SettingScene>()));
}

void SceneManager::CloseSettingScene(void)
{
	//設定画面描画終了
	scenes_.pop_back();

	//カメラ操作を有効化
	camera_->CameraMoveLock(false);
}

void SceneManager::OpenSelectScene(void)
{
	//セレクトシーン生成
	scenes_.emplace_back(move(make_unique<SelectScene>()));
}

void SceneManager::CloseSelectScene(void)
{
	//セレクトシーン描画終了
	scenes_.pop_back();

	//ゲームシーンに遷移
	ChangeScene(SCENE_ID::GAME);
}

void SceneManager::RetryGame(void)
{
	//ゲームシーンに遷移
	ChangeScene(SCENE_ID::GAME);
}

void SceneManager::ReturnTitleScene(void)
{
	//タイトルシーンに遷移
	ChangeScene(SCENE_ID::TITLE);
}

void SceneManager::SceneChange_None(void)
{
	//処理無し
}

void SceneManager::SceneChange_Title(void)
{
	ClearActors();
	scenes_.emplace_back(move(make_unique<TitleScene>()));
}

void SceneManager::SceneChange_Game(void)
{
	ClearActors();
	scenes_.emplace_back(move(make_unique<GameScene>()));
}

void SceneManager::SceneChange_Clear(void)
{
	ClearActors();
	scenes_.emplace_back(move(make_unique<ClearScene>()));
}

void SceneManager::SceneChange_Failed(void)
{
	ClearActors();
	scenes_.emplace_back(move(make_unique<FailedScene>()));
}

void SceneManager::Fade(void)
{
	Fader::State fState = fader_->GetState();
	switch (fState)
	{
	case Fader::State::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->ChangeState(Fader::State::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::State::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->ChangeState(Fader::State::FADE_IN);
		}
		break;
	}
}

void SceneManager::Init3D(void)
{
	//背景色を設定
	SetBackgroundColor(
		DEF_BACKCOLOR[COLOR_RED],
		DEF_BACKCOLOR[COLOR_GREEN], 
		DEF_BACKCOLOR[COLOR_BLUE]
	);
	
	//3D描画のZバッファを有効化
	SetUseZBuffer3D(true);

	//3D描画のZバッファへの書込を有効化
	SetWriteZBuffer3D(true);

	//バックカリングを有効化
	SetUseBackCulling(true);

	//フォグを有効化
	SetFogEnable(true);

	//フォグの色
	SetFogColor(
		FOG_COLOR[COLOR_RED],
		FOG_COLOR[COLOR_GREEN],
		FOG_COLOR[COLOR_BLUE]
	);

	//フォグの距離
	SetFogStartEnd(FOG_STARTEND.x, FOG_STARTEND.y);
}

void SceneManager::InitSceneChange(void)
{
	//初期化用
	sceneChanges_.emplace(SCENE_ID::NONE, bind(&SceneManager::SceneChange_None, this));

	//タイトル
	sceneChanges_.emplace(SCENE_ID::TITLE, bind(&SceneManager::SceneChange_Title, this));
	
	//ゲーム
	sceneChanges_.emplace(SCENE_ID::GAME, bind(&SceneManager::SceneChange_Game, this));
	
	//クリア
	sceneChanges_.emplace(SCENE_ID::CLEAR, bind(&SceneManager::SceneChange_Clear, this));
	
	//ゲームオーバー
	sceneChanges_.emplace(SCENE_ID::FAILED, bind(&SceneManager::SceneChange_Failed, this));
}

const bool SceneManager::IsSceneError(void) const
{
	return sceneId_ == SCENE_ID::NONE || sceneId_ == SCENE_ID::MAX;
}

const bool SceneManager::IsSceneError(const SCENE_ID scnId) const
{
	return scnId == SCENE_ID::NONE || scnId == SCENE_ID::MAX;
}

void SceneManager::ClearActors(void)
{
	ActorManager::GetInstance().ClearActors();
	CharactorManager::GetInstance().ClearCharactors();
	ShotManager::GetInstance().ClearShots();

	UIManager::GetInstance().ClearUIs();
}




