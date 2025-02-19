

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
		//読込関数が全て終了している、または登録されていない
		return;
	}

	//先頭のリソースから読込開始
	const auto& loadingFunc = loadingFunc_.front();
	if (!loadingFunc)
	{
		//関数オブジェクトがリンク切れ
		return;
	}


	if (!isLoadingNow_)
	{
		//読込開始
		loadingFunc();
		isLoadingNow_ = true;
	}

	//現在の非同期読込中のリソース数
	const int IsASyncLoadNum = GetASyncLoadNum();

	loadingCnt_ += deltaTime;

	if (LOADING_CHECKTIME < loadingCnt_)
	{
		//1秒経過時に強制待機
		WaitHandleASyncLoadAll();
	}

	if (IsASyncLoadNum <= 0)
	{
		//非同期読込が終了している
		//読込するリソースの変更
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
