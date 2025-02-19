#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ResourceManager/Images/ImagesResourcesManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ActorsManager/Actor/ActorManager.h"
#include "../Manager/ActorsManager/Charactor/CharactorManager.h"
#include "../Manager/ResourceManager/Font/FontResourcesManager.h"
#include "../Manager/UIManager/UIManager.h"
#include "../Manager/Input/InputController.h"
#include "../Application.h"
#include "../Observer/Observer.h"
#include "../Sound/SoundController.h"
#include "../Common/Grid3d.h"
#include "SceneBase.h"

using namespace std;

using ResM = ResourceManager;
using ScnM = SceneManager;
using Input = InputController;

//ローディング時のダミー待機時間
const float TIME_LOADING_WAIT = 5.0f;

//ローディングのインデックスが進むまでの時間
const float TIME_LOADING_IMAGE_COUNT = 0.1f;

const int LOADING_ICON_X = 
Application::SCREEN_SIZE_X - (Application::SCREEN_SIZE_HALF_X / 4);

const int LOADING_ICON_Y =
Application::SCREEN_SIZE_Y - (Application::SCREEN_SIZE_HALF_Y / 4);

const int TUTORIAL_IMAGES_MAX = 2;

SceneBase::SceneBase(void)
	: 
	scnMng_(ScnM::GetInstance()),
	resMng_(ResM::GetInstance()),
	camera_(ScnM::GetInstance().GetCamera()),
	resMngModel_(ResM::GetInstance().GetModelResourcesInstance()),
	resMngSound_(ResM::GetInstance().GetSoundResourcesInstance()),
	resMngEffect_(ResM::GetInstance().GetEffectResourcesInstance()),
	resMngImage_(ResM::GetInstance().GetImageResourcesInstance()),
	resMngImages_(ResM::GetInstance().GetImagesResourcesInstance()),
	resMngShader_(ResM::GetInstance().GetShaderResourcesInstance()),
	resMngFont_(ResM::GetInstance().GetFontResourcesInstance()),
	actorMng_(ActorManager::GetInstance()),
	charaMng_(CharactorManager::GetInstance()),
	uiMng_(UIManager::GetInstance()),
	observer_(Observer::GetInstance()),
	inputController_(make_unique<Input>()),
	soundController_(make_unique<SoundController>()),
	grid_(make_unique<Grid3d>()),
	state_(State::None),
	loadState_(LoadState::None),
	loadingTime_(0.0f),
	loadingImgsHandles_{},
	loadingImgsIdxCnt_(0),
	loadingImgsIdxMax_(0),
	loadingCntStep_(TIME_LOADING_IMAGE_COUNT),
	loadingEndStartImgsHandle_(-1),
	tutorialImgsHandle_{},
	tutorialImgsIdxCnt_(0)
{
	//読込画像読込開始
	resMngImages_.Load(SRC_IMAGES::LOADING);
	
	//操作説明画像読込開始
	resMngImage_.Load(SRC_IMAGE::OperationInstructions_Controller);

	//操作説明画像読込開始
	resMngImage_.Load(SRC_IMAGE::OperationInstructions_KeyBoard);

	//操作説明の際に表示する矢印画像読込開始
	resMngImage_.Load(SRC_IMAGE::Arrow);
	
	//画像の非同期読込待機
	WaitHandleASyncLoadAll();

	//読込画像読込開始
	const auto& loadingImagesInfo = resMngImages_.Load(SRC_IMAGES::LOADING).GetImagesInfo();

	//操作説明画像読込開始
	const auto tutorialController = resMngImage_.Load(
		SRC_IMAGE::OperationInstructions_Controller).GetHandleId();

	//操作説明画像読込開始
	const auto tutorialKeyboard = resMngImage_.Load(
		SRC_IMAGE::OperationInstructions_KeyBoard).GetHandleId();

	//操作説明の際に表示する矢印画像読込開始
	tutorialArrowsImgsHandle_ = resMngImage_.Load(SRC_IMAGE::Arrow).GetHandleId();

	//配列のメモリを事前確保
	loadingImgsHandles_.reserve((size_t)(loadingImgsIdxMax_ + 1));
	tutorialImgsHandle_.reserve((size_t)(TUTORIAL_IMAGES_MAX + 1));

	//読込画像
	loadingImgsIdxMax_ = loadingImagesInfo.GetNumMax();
	for (int idx = 0; idx < loadingImgsIdxMax_; idx++)
	{
		loadingImgsHandles_.emplace_back(loadingImagesInfo.handleIds[idx]);
	}
	loadingImgsHandles_.resize(loadingImgsHandles_.size());

	//操作説明画像
	tutorialImgsHandle_.emplace_back(tutorialController);
	tutorialImgsHandle_.emplace_back(tutorialKeyboard);
	tutorialImgsHandle_.resize(tutorialImgsHandle_.size());


	InitChangeLoadState();
}

SceneBase::~SceneBase(void)
{
	const int ASyncLoadNum = GetASyncLoadNum();

	if (ASyncLoadNum > 0)
	{
		//強制的に読込終了待機
		WaitHandleASyncLoadAll();
	}

	if (aSyncLoad_.valid())
	{
		//Dxlib非対応用の非同期読込が登録されている
		//念のため終了待機
		aSyncLoad_.wait();
	}
}

void SceneBase::Init(void)
{
}

void SceneBase::InitSound(void)
{
}

void SceneBase::InitChangeState(void)
{
}

void SceneBase::InitChangeLoadState(void)
{
	loadStateChange_.emplace(
		LoadState::IsAsyncLoad,
		bind(&SceneBase::ChangeLoadState_IsAsyncLoad, this)
	);

	loadStateChange_.emplace(
		LoadState::IsLoadEnd,
		bind(&SceneBase::ChangeLoadState_IsLoadEnd, this)
	);
}

void SceneBase::ChangeState(const State state)
{
	if (!stateChange_.contains(state))
	{
		//状態遷移が登録されていない
		return;
	}

	state_ = state;

	const auto& stateChangeFunc = stateChange_.at(state_);
	if (!stateChangeFunc)
	{
		//関数オブジェクトがリンク切れ
		return;
	}
	stateChangeFunc();
}

void SceneBase::ChangeState_Load(void)
{
}

void SceneBase::ChangeState_Play(void)
{
}

void SceneBase::ChangeState_End(void)
{
}

void SceneBase::StateUpdate_Load(void)
{
}

void SceneBase::StateUpdate_Play(void)
{
}

void SceneBase::StateUpdate_End(void)
{
}

void SceneBase::StateDraw_Load(void)
{
}

void SceneBase::StateDraw_Play(void)
{
}

void SceneBase::StateDraw_End(void)
{
}

void SceneBase::ChangeLoadState(const LoadState loadState)
{
	if (!loadStateChange_.contains(loadState))
	{
		//登録されていない
		return;
	}

	loadState_ = loadState;

	const auto& loadStateChange = loadStateChange_.at(loadState_);

	if (!loadStateChange)
	{
		//関数オブジェクトがリンク切れ
		return;
	}

	loadStateChange();
}

void SceneBase::ChangeLoadState_IsAsyncLoad(void)
{
	loadStateUpdate_ = bind(&SceneBase::LoadStateUpdate_IsAsyncLoad, this);
	loadStateDraw_ = bind(&SceneBase::LoadStateDraw_IsAsyncLoad, this);
}

void SceneBase::ChangeLoadState_IsLoadEnd(void)
{
	loadingEndStartImgsHandle_ = resMngImage_.Load(
		SRC_IMAGE::LoadEnd_Start
	).GetHandleId();

	loadStateUpdate_ = bind(&SceneBase::LoadStateUpdate_IsLoadEnd, this);
	loadStateDraw_ = bind(&SceneBase::LoadStateDraw_IsLoadEnd, this);
}

void SceneBase::LoadStateUpdate_IsAsyncLoad(void)
{
	const int ASyncLoadNum = GetASyncLoadNum();

	resMng_.Update();

	UpdateIsASyncLoad();

	UpdateTutorial();

	if (loadingTime_ > TIME_LOADING_WAIT && !resMng_.GetIsLoadingNow())
	{
		//ダミー待機時間が終了

		if(aSyncLoad_.valid())
		{
			//Dxlib非対応用の非同期読込が登録されている
			//終了待機
			aSyncLoad_.wait();
		}

		if (effectLoad_)
		{
			effectLoad_();
		}

		ChangeLoadState(LoadState::IsLoadEnd);
	}

	loadingTime_ += TimeManager::GetInstance().GetDeltaTime();
}

void SceneBase::LoadStateUpdate_IsLoadEnd(void)
{
	UpdateTutorial();

	if (inputController_->IsTriggered(ACTION::MENU_ENTER))
	{
		//決定入力で
		ChangeState(State::Play);
	}
}

void SceneBase::LoadStateDraw_IsAsyncLoad(void)
{
	DrawTutorial();

	DrawIsASyncLoad();
}

void SceneBase::LoadStateDraw_IsLoadEnd(void)
{
	DrawTutorial();

	DrawRotaGraph(
		LOADING_ICON_X,
		LOADING_ICON_Y,
		1.0, 0.0,
		loadingEndStartImgsHandle_, true
	);
}

void SceneBase::UpdateTutorial(void)
{
	if (inputController_->IsTriggered(ACTION::MENU_RIGHT))
	{
		++tutorialImgsIdxCnt_;
		if (tutorialImgsIdxCnt_ >= TUTORIAL_IMAGES_MAX)
		{
			//最初の画像に戻る
			tutorialImgsIdxCnt_ = 0;
		}
	}
	else if (inputController_->IsTriggered(ACTION::MENU_LEFT))
	{
		--tutorialImgsIdxCnt_;
		if (tutorialImgsIdxCnt_ < 0)
		{
			//最後の画像に戻る
			tutorialImgsIdxCnt_ = TUTORIAL_IMAGES_MAX - 1;
		}
	}
}

void SceneBase::DrawTutorial(void)
{
	if (tutorialImgsHandle_.empty())
	{
		return;
	}

	DrawRotaGraph(
		Application::SCREEN_SIZE_HALF_X,
		Application::SCREEN_SIZE_HALF_Y,
		1.5, 0.0,
		tutorialImgsHandle_[tutorialImgsIdxCnt_], true
	);

	DrawRotaGraph(
		LOADING_ICON_X, Application::SCREEN_SIZE_HALF_Y,
		0.1, 0.0,
		tutorialArrowsImgsHandle_, true
	);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X - LOADING_ICON_X, Application::SCREEN_SIZE_HALF_Y,
		0.1, 3.14,
		tutorialArrowsImgsHandle_, true
	);
}

void SceneBase::UpdateIsASyncLoad(void)
{
	loadingCntStep_ -= TimeManager::GetInstance().GetDeltaTime();
	if (loadingCntStep_ < 0.0f)
	{
		//ローディング画像のインデックスを進める
		loadingCntStep_ = TIME_LOADING_IMAGE_COUNT;
		++loadingImgsIdxCnt_;
	}
}

void SceneBase::DrawIsASyncLoad(void)
{
	if (loadingImgsHandles_.empty())
	{
		return;
	}

	int idx = loadingImgsIdxCnt_ % loadingImgsIdxMax_;
	if (idx > loadingImgsIdxMax_)
	{
		//インデックスのエラー回避
		idx = 0;
	}

	DrawRotaGraph(
		LOADING_ICON_X,
		LOADING_ICON_Y,
		1.0, 0.0,
		loadingImgsHandles_[idx], true
	);
}

void SceneBase::UpdateSound(void)
{
	if (soundController_ != nullptr)
	{
		soundController_->Update();
	}
}
