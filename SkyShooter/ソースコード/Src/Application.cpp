
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

	//�Q�[�����[�v�J�n
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

		//fps�v�Z�ƕ`��
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


	//Effekseer���I������B
	Effkseer_End();

	//���[���ŉ����DxLib_End()�O�ɂ���K�v������
	//DxLib�I��
	if (DxLib_End() == -1)
	{
		//Dxlib�I�����G���[
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

	//�A�v���P�[�V�����̏����ݒ�
	SetWindowText("SkyShooter");

	//�E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	//SetGraphMode(1000, 750, 32);

	//�E�B���h�E���[�h
	//ChangeWindowMode(false);
	ChangeWindowMode(true);

	//DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		//Dxlib�̏��������s
		isInitFail_ = true;
		return;
	}

	//Effekseer�̏�����
	InitEffekseer();

	Effekseer_InitDistortion(1.0f);

	//�L�[���䏉����
	SetUseDirectInputFlag(true);

	//���Ԑ���
	TimeManager::CreateInstace();

	//��������
	RandManager::CreateInstace();
	
	//�I�u�U�[�o�[?
	Observer::CreateInstace();

	//���͐���
	InputManager::CreateInstace();

	//���\�[�X�Ǘ�
	ResourceManager::CreateInstace();

	//JSON�f�[�^�Ǘ�
	JSONManager::CreateInstace();

	//�I�u�W�F�N�g
	ActorManager::CreateInstace();

	//�L�����N�^�[
	CharactorManager::CreateInstace();

	//�e : �I�u�W�F�N�g�Ƃ������Ă���������
	ShotManager::CreateInstace();

	//�Փ˔��萧��
	ColliderManager::CreateInstace();
	
	//UI�f�[�^�Ǘ�
	UIDataManager::CreateInstace();
	
	//UI�Ǘ�
	UIManager::CreateInstace();

	//�V�[���Ǘ�
	SceneManager::CreateInstace();
}

Application::~Application(void)
{
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init (8000) == -1)
	{
		//Effekseer�̏��������s
		DxLib_End();
	}

	// ��ʃ��[�h�ύX��( �ƃE�C���h�E���[�h�ύX�� )��
	// �O���t�B�b�N�X�V�X�e���̐ݒ��O���t�B�b�N�n���h�������Z�b�g���Ȃ��悤�ݒ�
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

