

#include <functional>
#include "Model/ModelResourcesManager.h"
#include "Sound/SoundResourcesManager.h"
#include "Image/ImageResourcesManager.h"
#include "Images/ImagesResourcesManager.h"
#include "../TimeManager.h"
#include "Effect/EffectResourcesManager.h"
#include "Shader/ShaderManager.h"
#include "Font/FontResourcesManager.h"
#include "ResourceManager.h"

using namespace std;

using ModelRes = ModelResourcesManager;
using SoundRes = SoundResourcesManager;
using ImgRes = ImageResourcesManager;
using ImgsRes = ImagesResourcesManager;
using EffRes = EffectResourcesManager;
using ShaderRes = ShaderManager;
using FontRes = FontResourcesManager;

const float LOADING_CHECKTIME = 1.0f;

ResourceManager* Singleton<ResourceManager>::instance_ = nullptr;

void ResourceManager::Destroy(void)
{
	Release();

	delete instance_;
}

void ResourceManager::Release(void)
{
	modelResourcesManager_->Release();
	soundResourcesManager_->Release();
	imageResourcesManager_->Release();
	imagesResourcesManager_->Release();
	effectResourcesManager_->Release();
	shaderResourcesManager_->Release();
	fontResourcesManager_->Release();
}

void ResourceManager::Update(void)
{
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	if (loadingFunc_.empty())
	{
		//�Ǎ��֐����S�ďI�����Ă���A�܂��͓o�^����Ă��Ȃ�
		return;
	}

	//�擪�̃��\�[�X����Ǎ��J�n
	const auto& loadingFunc = loadingFunc_.front();
	if (!loadingFunc)
	{
		//�֐��I�u�W�F�N�g�������N�؂�
		return;
	}


	if (!isLoadingNow_)
	{
		//�Ǎ��J�n
		loadingFunc();
		isLoadingNow_ = true;
	}

	//���݂̔񓯊��Ǎ����̃��\�[�X��
	const int IsASyncLoadNum = GetASyncLoadNum();

	loadingCnt_ += deltaTime;

	if (LOADING_CHECKTIME < loadingCnt_)
	{
		//1�b�o�ߎ��ɋ����ҋ@
		WaitHandleASyncLoadAll();
	}

	if (IsASyncLoadNum <= 0)
	{
		//�񓯊��Ǎ����I�����Ă���
		//�Ǎ����郊�\�[�X�̕ύX
		ChangeLoadResource();
	}
}

void ResourceManager::Load(const SCENE_ID scene)
{
	nowLoadingSceneResouce_ = scene;

	loadingFunc_.emplace_back([&] {modelResourcesManager_->LoadUseScene(scene); });
	loadingFunc_.emplace_back([&] {soundResourcesManager_->LoadUseScene(scene); });
	loadingFunc_.emplace_back([&] {imageResourcesManager_->LoadUseScene(scene); });
	loadingFunc_.emplace_back([&] {imagesResourcesManager_->LoadUseScene(scene); });
}

ResourceManager::ResourceManager(void)
	:
	modelResourcesManager_(make_unique<ModelRes>()),
	soundResourcesManager_(make_unique<SoundRes>()),
	imageResourcesManager_(make_unique<ImgRes>()),
	imagesResourcesManager_(make_unique<ImgsRes>()),
	effectResourcesManager_(make_unique<EffRes>()),
	shaderResourcesManager_(make_unique<ShaderRes>()),
	fontResourcesManager_(make_unique<FontRes>()),
	loadingCnt_(0.0f),
	isLoadingNow_(false),
	nowLoadingSceneResouce_(SCENE_ID::NONE)
{
}

ResourceManager::~ResourceManager(void)
{
	WaitHandleASyncLoadAll();
}

void ResourceManager::ChangeLoadResource(void)
{
	loadingFunc_.pop_front();
	isLoadingNow_ = false;
	loadingCnt_ = 0.0f;
}
