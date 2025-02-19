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
		//�֐��I�u�W�F�N�g�������N�؂�
		return;
	}

	stateUpdate_();
}

void GameScene::Draw(void)
{
	if (!stateDraw_)
	{
		//�֐��I�u�W�F�N�g�������N�؂�
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
			//���̂����݂��Ă��Ȃ�
			continue; 
		}

		//�L�����N�^�[�̕`��
		charactor->Load();
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr) 
		{
			//���̂����݂��Ă��Ȃ�
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
	//�I������Ă��鑀����@���擾
	CONTROL_TYPE ctlType = InputManager::GetInstance().GetControlType();
	if (ctlType == CONTROL_TYPE::KEY)
	{
		//�J�������L�[�}�E�p�ɐݒ�
		camera_.ChangeMode(Camera::CAMERA_MODE::FOLLOW_MOUSE);
	}
	else
	{
		//�J�������R���g���[���[�p�ɐݒ�
		camera_.ChangeMode(Camera::CAMERA_MODE::FOLLOW);
	}

	//���@�ɒǏ]
	weak_ptr<CharactorBase> player = charaMng_.GetCharactor<PlayerBase>(OBJECT_TYPE::PLAYER);
	
	if (player.expired())
	{
		//���@�����݂��Ă��Ȃ�
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

	//BGM�Đ�
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
		//�֐��I�u�W�F�N�g�������N�؂�
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
			//�ʒm���o�^����Ă��Ȃ�
			continue;
		}

		update = messageUpdate_.at(mes);

		if (!update)
		{
			//�֐��I�u�W�F�N�g�������N�؂�
			continue;
		}

		update();

		//���U���g�ɑJ�ڂ��邽�ߏ����͕s�v
		return;
	}

	for (const auto& actor : actorMng_.GetActors())
	{
		if (actor == nullptr)
		{
			//���̂����݂��Ă��Ȃ�
			continue;
		}

		//�L�����N�^�[�ȊO�̃I�u�W�F�N�g�X�V
		actor->Update();
	}

	for (const auto& charactor : charaMng_.GetCharactors())
	{
		if (!charactor)
		{
			//���̂����݂��Ă��Ȃ�
			continue;
		}

		//�L�����N�^�[�̍X�V
		charactor->Update();
	}

	auto& shotMng = ShotManager::GetInstance();
	for (const auto& shot : shotMng.GetShots())
	{
		if (!shot)
		{
			//���̂����݂��Ă��Ȃ�
			continue;
		}

		//�e�̍X�V
		shot->Update();
	}

	//�e�̏�ԍX�V
	shotMng.Update();

	for (const auto& ui : uiMng_.GetUIs())
	{
		if (ui == nullptr)
		{
			//���̂����݂��Ă��Ȃ�
			continue;
		}
		ui->Update();
	}

	if (inputController_->IsTriggered(ACTION::MENU_OPEN))
	{
		//���j���[��ʊJ
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
		//�֐��I�u�W�F�N�g�������N�؂�
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
			//���̂����݂��Ă��Ȃ�
			continue;
		}
		actor->Draw();
	}

	auto& shotMng = ShotManager::GetInstance();
	for (const auto& shot : shotMng.GetShots())
	{
		if (!shot)
		{
			//���̂����݂��Ă��Ȃ�
			continue;
		}

		//�e�̕`��
		shot->Draw();
	}

	const auto& charactors = charaMng_.GetCharactors();
	for (const auto& charactor : charactors)
	{
		if (!charactor)
		{
			//���̂����݂��Ă��Ȃ�
			continue;
		}

		//�L�����N�^�[�̕`��
		charactor->Draw();
	}

	for (const auto& ui : uiMng_.GetUIs())
	{
		if (ui == nullptr)
		{
			//���̂����݂��Ă��Ȃ�
			continue;
		}

		ui->Draw();
	}

	//DrawDebug();
}

void GameScene::StateDraw_End(void)
{
}
