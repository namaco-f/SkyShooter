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

//���[�f�B���O���̃_�~�[�ҋ@����
const float TIME_LOADING_WAIT = 5.0f;

//���[�f�B���O�̃C���f�b�N�X���i�ނ܂ł̎���
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
	//�Ǎ��摜�Ǎ��J�n
	resMngImages_.Load(SRC_IMAGES::LOADING);
	
	//��������摜�Ǎ��J�n
	resMngImage_.Load(SRC_IMAGE::OperationInstructions_Controller);

	//��������摜�Ǎ��J�n
	resMngImage_.Load(SRC_IMAGE::OperationInstructions_KeyBoard);

	//��������̍ۂɕ\��������摜�Ǎ��J�n
	resMngImage_.Load(SRC_IMAGE::Arrow);
	
	//�摜�̔񓯊��Ǎ��ҋ@
	WaitHandleASyncLoadAll();

	//�Ǎ��摜�Ǎ��J�n
	const auto& loadingImagesInfo = resMngImages_.Load(SRC_IMAGES::LOADING).GetImagesInfo();

	//��������摜�Ǎ��J�n
	const auto tutorialController = resMngImage_.Load(
		SRC_IMAGE::OperationInstructions_Controller).GetHandleId();

	//��������摜�Ǎ��J�n
	const auto tutorialKeyboard = resMngImage_.Load(
		SRC_IMAGE::OperationInstructions_KeyBoard).GetHandleId();

	//��������̍ۂɕ\��������摜�Ǎ��J�n
	tutorialArrowsImgsHandle_ = resMngImage_.Load(SRC_IMAGE::Arrow).GetHandleId();

	//�z��̃����������O�m��
	loadingImgsHandles_.reserve((size_t)(loadingImgsIdxMax_ + 1));
	tutorialImgsHandle_.reserve((size_t)(TUTORIAL_IMAGES_MAX + 1));

	//�Ǎ��摜
	loadingImgsIdxMax_ = loadingImagesInfo.GetNumMax();
	for (int idx = 0; idx < loadingImgsIdxMax_; idx++)
	{
		loadingImgsHandles_.emplace_back(loadingImagesInfo.handleIds[idx]);
	}
	loadingImgsHandles_.resize(loadingImgsHandles_.size());

	//��������摜
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
		//�����I�ɓǍ��I���ҋ@
		WaitHandleASyncLoadAll();
	}

	if (aSyncLoad_.valid())
	{
		//Dxlib��Ή��p�̔񓯊��Ǎ����o�^����Ă���
		//�O�̂��ߏI���ҋ@
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
		//��ԑJ�ڂ��o�^����Ă��Ȃ�
		return;
	}

	state_ = state;

	const auto& stateChangeFunc = stateChange_.at(state_);
	if (!stateChangeFunc)
	{
		//�֐��I�u�W�F�N�g�������N�؂�
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
		//�o�^����Ă��Ȃ�
		return;
	}

	loadState_ = loadState;

	const auto& loadStateChange = loadStateChange_.at(loadState_);

	if (!loadStateChange)
	{
		//�֐��I�u�W�F�N�g�������N�؂�
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
		//�_�~�[�ҋ@���Ԃ��I��

		if(aSyncLoad_.valid())
		{
			//Dxlib��Ή��p�̔񓯊��Ǎ����o�^����Ă���
			//�I���ҋ@
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
		//������͂�
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
			//�ŏ��̉摜�ɖ߂�
			tutorialImgsIdxCnt_ = 0;
		}
	}
	else if (inputController_->IsTriggered(ACTION::MENU_LEFT))
	{
		--tutorialImgsIdxCnt_;
		if (tutorialImgsIdxCnt_ < 0)
		{
			//�Ō�̉摜�ɖ߂�
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
		//���[�f�B���O�摜�̃C���f�b�N�X��i�߂�
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
		//�C���f�b�N�X�̃G���[���
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
