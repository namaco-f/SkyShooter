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

const Vector2f FOG_STARTEND = { 800.0f, 10000.0f };		//�t�H�O�̊J�n�I������

int COLOR_RED = 0;
int COLOR_GREEN = 1;
int COLOR_BLUE = 2;

const int FOG_COLOR[3] = { 127, 200, 255 };				//�t�H�O�̐F

const int DEF_BACKCOLOR[3] = { 255,255,255 };			//�f�t�H���g�̔w�i�F

SceneManager* Singleton<SceneManager>::instance_ = nullptr;

void SceneManager::Init(void)
{
	//SceneManager�̎Q�Ƃ��g�p���Ă��邽��
	//���̐�����Ɏ��s
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Update(void)
{
	auto& timeMng = TimeManager::GetInstance();
	auto& colMng = ColliderManager::GetInstance();

	if (scenes_.empty())
	{
		//�V�[�������݂��Ȃ�
		return;
	}

	timeMng.Update();

	camera_->Update();

	fader_->Update();
	if (IsSceneChanging())
	{	
		//�V�[���J�ڒ�
		Fade();
	}
	else
	{
		//�Փ˔���X�V
		colMng.UpdateCollider();
		
		for (const auto& scene : scenes_)
		{
			//���ʍX�V
			scene->UpdateSound();
		}

		//�Ō�ɒǉ�����Ă���V�[���̂ݍX�V
		scenes_.back()->Update();
	}
}

void SceneManager::Draw(void)
{
	//�f�o�b�O�p
	debugUtility_->DebugStringCntReset();

	//�`���O���t�B�b�N�̈�̎w��
	//�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����
	SetDrawScreen(mainScreen_);

	// ��ʂ�������
	ClearDrawScreen();

	camera_->SetBeforeDraw();

	//�`��
	for (const auto& scene : scenes_)
	{
		scene->Draw();
	}

	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();

	//�Ó]�E���]
	fader_->Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawGraph(0, 0, mainScreen_,true);
}

void SceneManager::ChangeScene(const SCENE_ID nextId)
{
	//�t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	//�J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	//�t�F�[�h�A�E�g(�Ó])���J�n����
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

	//�J�������[�h�����R���_�ɕύX
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

	// �V�[����ύX����
	sceneId_ = sceneId;

	// ���݂̃V�[�������
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
	//�J��������𖳌���
	camera_->CameraMoveLock(true);

	//�ݒ��ʐ���
	scenes_.emplace_back(move(make_unique<SettingScene>()));
}

void SceneManager::CloseSettingScene(void)
{
	//�ݒ��ʕ`��I��
	scenes_.pop_back();

	//�J���������L����
	camera_->CameraMoveLock(false);
}

void SceneManager::OpenSelectScene(void)
{
	//�Z���N�g�V�[������
	scenes_.emplace_back(move(make_unique<SelectScene>()));
}

void SceneManager::CloseSelectScene(void)
{
	//�Z���N�g�V�[���`��I��
	scenes_.pop_back();

	//�Q�[���V�[���ɑJ��
	ChangeScene(SCENE_ID::GAME);
}

void SceneManager::RetryGame(void)
{
	//�Q�[���V�[���ɑJ��
	ChangeScene(SCENE_ID::GAME);
}

void SceneManager::ReturnTitleScene(void)
{
	//�^�C�g���V�[���ɑJ��
	ChangeScene(SCENE_ID::TITLE);
}

void SceneManager::SceneChange_None(void)
{
	//��������
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
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->ChangeState(Fader::State::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::State::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// ���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
			// �Ó]���疾�]��
			fader_->ChangeState(Fader::State::FADE_IN);
		}
		break;
	}
}

void SceneManager::Init3D(void)
{
	//�w�i�F��ݒ�
	SetBackgroundColor(
		DEF_BACKCOLOR[COLOR_RED],
		DEF_BACKCOLOR[COLOR_GREEN], 
		DEF_BACKCOLOR[COLOR_BLUE]
	);
	
	//3D�`���Z�o�b�t�@��L����
	SetUseZBuffer3D(true);

	//3D�`���Z�o�b�t�@�ւ̏�����L����
	SetWriteZBuffer3D(true);

	//�o�b�N�J�����O��L����
	SetUseBackCulling(true);

	//�t�H�O��L����
	SetFogEnable(true);

	//�t�H�O�̐F
	SetFogColor(
		FOG_COLOR[COLOR_RED],
		FOG_COLOR[COLOR_GREEN],
		FOG_COLOR[COLOR_BLUE]
	);

	//�t�H�O�̋���
	SetFogStartEnd(FOG_STARTEND.x, FOG_STARTEND.y);
}

void SceneManager::InitSceneChange(void)
{
	//�������p
	sceneChanges_.emplace(SCENE_ID::NONE, bind(&SceneManager::SceneChange_None, this));

	//�^�C�g��
	sceneChanges_.emplace(SCENE_ID::TITLE, bind(&SceneManager::SceneChange_Title, this));
	
	//�Q�[��
	sceneChanges_.emplace(SCENE_ID::GAME, bind(&SceneManager::SceneChange_Game, this));
	
	//�N���A
	sceneChanges_.emplace(SCENE_ID::CLEAR, bind(&SceneManager::SceneChange_Clear, this));
	
	//�Q�[���I�[�o�[
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




