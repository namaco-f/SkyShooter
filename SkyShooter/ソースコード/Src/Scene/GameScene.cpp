#include <DxLib.h>
#include <thread>
#include <functional>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/Input/InputManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../Manager/ActorsManager/Shot/ShotManager.h"
#include "../Manager/ActorsManager/Actor/ActorManager.h"
#include "../Manager/ResourceManager/Shader/ShaderManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../Manager/Input/InputController.h"
#include "../Manager/UIManager/UIManager.h"
#include "../Manager/UIManager/UIDataManager.h"

#include "../Observer/Observer.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/Material/PixelMaterial.h"

#include "../Object/Actor/ActorFactory.h"

#include "../Object/Actor/Charactor/CharactorBase.h"
#include "../Object/Actor/Charactor/CharactorFactory.h"
#include "../Object/Actor/Charactor/Player/PlayerUnit01.h"
#include "../Object/Actor/Charactor/Enemy/EnemyUnit01.h"
#include "../Object/Actor/Shot/ShotBase.h"
#include "../Object/Actor/Stage/Ground.h"
#include "../Object/Actor/Stage/SkyDome.h"
#include "../UI/HP/UI_PlayerParameter.h"
#include "../UI/HP/UI_EnemyHP.h"
#include "../UI/MiniMap/UI_MiniMap.h"
#include "../UI/UIFactory.h"
#include "../UI/GetUIDataToVariantTemplate.h"

#include "../Sound/SoundController.h"

#include "../Utility/DebugUtility.h"
#include "../Common/Grid3d.h"
#include "GameScene.h"


using namespace std;

using ImgRes = ImageResourcesManager;
using ImgsRes = ImagesResourcesManager;
using ModelRes = ModelResourcesManager;
using SoundRes = SoundResourcesManager;
using EffectRes = EffectResourcesManager;

using CameraMode = Camera::CAMERA_MODE;

const string BGM_GAME = "GameBGM";
const float INIT_BGM_VOL = 0.5f;
const float INIT_SE_VOL = 0.5f;

GameScene::GameScene(void)
	: 
	SceneBase(),
	uiDataMng_(UIDataManager::GetInstance())
{
	InitMessageUpdate();

	InitChangeState();

	ChangeState(State::Load);
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
}

void GameScene::Update(void)
{
	if (!stateUpdate_)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	stateUpdate_();
}

void GameScene::Draw(void)
{
	if (!stateDraw_)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	stateDraw_();
}

void GameScene::ASyncLoad(void)
{
	const auto& charactors = charaMng_.GetCharactors();
	const auto& actors = actorMng_.GetActors();

	for (const auto& charactor : charactors)
	{
		if (charactor == nullptr) 
		{
			//実体が存在していない
			continue; 
		}

		//キャラクターの描画
		charactor->Load();
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr) 
		{
			//実体が存在していない
			continue; 
		}
		actor->Load();
	}

	SetUseASyncLoadFlag(false);

	int aSyncNum = GetASyncLoadNum();

	WaitHandleASyncLoadAll();
}

void GameScene::DrawDebug(void)
{
	auto& debugUtility = scnMng_.GetDebugUtility();

	auto& timeMng = TimeManager::GetInstance();
	auto sceneActiveTime = timeMng.GetTime();
	auto deltaTime = timeMng.GetDeltaTime();
	Vector2f times = { sceneActiveTime ,deltaTime };

	debugUtility.DebugStringFormat(0, "GameScene", 0xffffff);

	debugUtility.DebugStringFormat(0, "Times = %.2f,%.5f", 0xffffff, times);
}

void GameScene::AddActors(void)
{
	actorMng_.AddActor([&] {return CreateActor<Ground>(); });
	actorMng_.AddActor([&] {return CreateActor<SkyDome>(); });

	charaMng_.AddCharactor([&] {return Create<PlayerUnit01>(); });
	charaMng_.AddCharactor([&] {return Create<EnemyUnit01>(); });
}

void GameScene::AddUI(void)
{
	uiMng_.AddUI([&] {return CreateUI<UI_PlayerParameter>(); });
	uiMng_.AddUI([&] {return CreateUI<UI_EnemyHP>(); });
	uiMng_.AddUI([&] {return CreateUI<UI_MiniMap>(); });
}

void GameScene::GameClear(void)
{
	float clearTime = TimeManager::GetInstance().GetTime();

	uiDataMng_.SetUIData<float>(SRC_UIData::CLEAR_TIME, clearTime);

	scnMng_.ChangeScene(SCENE_ID::CLEAR);
}

void GameScene::GameFailed(void)
{
	scnMng_.ChangeScene(SCENE_ID::FAILED);
}

void GameScene::InitSound(void)
{
	SoundInfo gameBGM = SoundInfo(
		resMngSound_.LoadSoundDuplicate(SRC_SOUND::BGM_GAME), 
		SoundType::BGM
	);
	soundController_->Add(BGM_GAME, gameBGM);
}

void GameScene::InitCamera(void)
{
	//選択されている操作方法を取得
	CONTROL_TYPE ctlType = InputManager::GetInstance().GetControlType();
	if (ctlType == CONTROL_TYPE::KEY)
	{
		//カメラをキーマウ用に設定
		camera_.ChangeMode(Camera::CAMERA_MODE::FOLLOW_MOUSE);
	}
	else
	{
		//カメラをコントローラー用に設定
		camera_.ChangeMode(Camera::CAMERA_MODE::FOLLOW);
	}

	//自機に追従
	weak_ptr<CharactorBase> player = charaMng_.GetCharactor<PlayerBase>(OBJECT_TYPE::PLAYER);
	
	if (player.expired())
	{
		//自機が存在していない
		return;
	}
	camera_.SetFollow(&player.lock()->GetTransform());
}

void GameScene::InitMessageUpdate(void)
{
	messageUpdate_.emplace(
		Message::Clear, bind(&GameScene::GameClear, this));

	messageUpdate_.emplace(
		Message::GameOver, bind(&GameScene::GameFailed, this));
}

void GameScene::InitASyncLoad(void)
{
	resMng_.Load(SCENE_ID::GAME);
}

void GameScene::InitChangeState(void)
{
	stateChange_.emplace(State::Load, bind(&GameScene::ChangeState_Load, this));
	stateChange_.emplace(State::Play, bind(&GameScene::ChangeState_Play, this));
	stateChange_.emplace(State::End, bind(&GameScene::ChangeState_End, this));
}

void GameScene::ChangeState_Load(void)
{
	stateUpdate_ = bind(&GameScene::StateUpdate_Load,this);
	stateDraw_ = bind(&GameScene::StateDraw_Load, this);

	//aSyncLoad_ = std::async([&]() {resMngEffect_.LoadUseScene(SCENE_ID::GAME); });
	effectLoad_ = std::function([&] {resMngEffect_.LoadUseScene(SCENE_ID::GAME); });

	ChangeLoadState(LoadState::IsAsyncLoad);
	InitASyncLoad();
}

void GameScene::ChangeState_Play(void)
{
	stateUpdate_ = bind(&GameScene::StateUpdate_Play, this);
	stateDraw_ = bind(&GameScene::StateDraw_Play, this);

	AddActors();
	AddUI();

	Init();

	InitCamera();

	InitSound();

	//BGM再生
	soundController_->Play(BGM_GAME);
}

void GameScene::ChangeState_End(void)
{
	stateUpdate_ = bind(&GameScene::StateUpdate_End, this);
	stateDraw_ = bind(&GameScene::StateDraw_End, this);
}

void GameScene::StateUpdate_Load(void)
{
	if (!loadStateUpdate_)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	loadStateUpdate_();
}

void GameScene::StateUpdate_Play(void)
{
	std::function<void(void)> update;

	for (const auto& mes : observer_.ReceiveMessageList())
	{
		if (!messageUpdate_.contains(mes))
		{
			//通知が登録されていない
			continue;
		}

		update = messageUpdate_.at(mes);

		if (!update)
		{
			//関数オブジェクトがリンク切れ
			continue;
		}

		update();

		//リザルトに遷移するため処理は不要
		return;
	}

	for (const auto& actor : actorMng_.GetActors())
	{
		if (actor == nullptr)
		{
			//実体が存在していない
			continue;
		}

		//キャラクター以外のオブジェクト更新
		actor->Update();
	}

	for (const auto& charactor : charaMng_.GetCharactors())
	{
		if (!charactor)
		{
			//実体が存在していない
			continue;
		}

		//キャラクターの更新
		charactor->Update();
	}

	auto& shotMng = ShotManager::GetInstance();
	for (const auto& shot : shotMng.GetShots())
	{
		if (!shot)
		{
			//実体が存在していない
			continue;
		}

		//弾の更新
		shot->Update();
	}

	//弾の状態更新
	shotMng.Update();

	for (const auto& ui : uiMng_.GetUIs())
	{
		if (ui == nullptr)
		{
			//実体が存在していない
			continue;
		}
		ui->Update();
	}

	if (inputController_->IsTriggered(ACTION::MENU_OPEN))
	{
		//メニュー画面開
		scnMng_.OpenSettingScene();
	}
}

void GameScene::StateUpdate_End(void)
{
}

void GameScene::StateDraw_Load(void)
{
	if (!loadStateDraw_)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	loadStateDraw_();
}

void GameScene::StateDraw_Play(void)
{
	const auto& actors = actorMng_.GetActors();
	for (const auto& actor : actors)
	{
		if (actor == nullptr)
		{
			//実体が存在していない
			continue;
		}
		actor->Draw();
	}

	auto& shotMng = ShotManager::GetInstance();
	for (const auto& shot : shotMng.GetShots())
	{
		if (!shot)
		{
			//実体が存在していない
			continue;
		}

		//弾の描画
		shot->Draw();
	}

	const auto& charactors = charaMng_.GetCharactors();
	for (const auto& charactor : charactors)
	{
		if (!charactor)
		{
			//実体が存在していない
			continue;
		}

		//キャラクターの描画
		charactor->Draw();
	}

	for (const auto& ui : uiMng_.GetUIs())
	{
		if (ui == nullptr)
		{
			//実体が存在していない
			continue;
		}

		ui->Draw();
	}

	//DrawDebug();
}

void GameScene::StateDraw_End(void)
{
}
