#include "../../../Application.h"
#include "ImageResourcesManager.h"

using namespace std;

ImageResourcesManager::ImageResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	//軌跡テクスチャ
	AddResourceMap(SRC_IMAGE::TrailTexture,"Texture/trail.png",USESCENE_GAME_ONLY);
	
	//丸影
	AddResourceMap(SRC_IMAGE::CircleShadow,"Shadow/shadow.png", USESCENE_GAME_ONLY);
	
	//キーマウ画像
	AddResourceMap(SRC_IMAGE::Keyboard_Mouse,"UI/Keyboard/keyboard_mouse.png", USESCENE_TITLE_ONLY);
	
	//コントローラー画像
	AddResourceMap(SRC_IMAGE::Controller,"UI/Xbox/controller.png", USESCENE_TITLE_ONLY);
	
	//タイトルロゴ
	AddResourceMap(SRC_IMAGE::TitleLogo,"Text/titlelogo.png",USESCENE_TITLE_ONLY);
	
	//カーソル画像
	AddResourceMap(SRC_IMAGE::Cursor,"UI/cursor.png",USESCENE_ALL);
	
	//矢印画像
	AddResourceMap(SRC_IMAGE::Arrow,"UI/arrow.png", USESCENE_ALL);
	
	//クリアテキスト画像
	AddResourceMap(SRC_IMAGE::ClearText,"Text/clear.png");
	
	//ロード終了テキスト画像
	AddResourceMap(SRC_IMAGE::LoadEnd_Start,"Text/start.png",USESCENE_ALL);
	
	//コントローラー操作説明画像
	AddResourceMap(
		SRC_IMAGE::OperationInstructions_Controller, 
		"UI/instructions_controller.png",USESCENE_ALL);
	
	//キーボード操作説明画像
	AddResourceMap(
		SRC_IMAGE::OperationInstructions_KeyBoard, 
		"UI/instructions_keyboard.png",USESCENE_ALL);
	
	//hpテキスト画像
	AddResourceMap(
		SRC_IMAGE::HP_TEXT,
		"Text/hp.png", USESCENE_GAME_ONLY);
	
	//mpテキスト画像
	AddResourceMap(
		SRC_IMAGE::MP_TEXT,
		"Text/mp_s20.png", USESCENE_GAME_ONLY);
	
	//seテキスト画像
	AddResourceMap(
		SRC_IMAGE::SE_TEXT,
		"Text/se.png", USESCENE_ALL);
	
	//bgmテキスト画像
	AddResourceMap(
		SRC_IMAGE::BGM_TEXT,
		"Text/bgm.png", USESCENE_ALL);
	
	//ボス敵テキスト画像
	AddResourceMap(
		SRC_IMAGE::BOSS_ENEMY_TEXT,
		"Text/enemy_boss.png", USESCENE_GAME_ONLY);
}

ImageResourcesManager::~ImageResourcesManager(void)
{
}

void ImageResourcesManager::Release(void)
{
	for (auto& lM : loadedMap_)
	{
		lM.second.Release();
	}
	loadedMap_.clear();
}

ImageResource& ImageResourcesManager::_Load(const SRC_IMAGE src)
{
	if (loadedMap_.contains(src))
	{
		//読込済
		return loadedMap_.at(src);
	}

	if (!resourcesMap_.contains(src))
	{
		//未登録
		return unregistered;
	}

	//登録済、未読込
	auto& ret = resourcesMap_.at(src);
	ret->Load();

	loadedMap_.emplace(src, *ret);

	return *ret;
}

void ImageResourcesManager::AddResourceMap(const SRC_IMAGE src, const std::string& file)
{
	unique_ptr<ImageResource> res;

	res = make_unique<ImageResource>(file);
	resourcesMap_.emplace(src, move(res));
}

void ImageResourcesManager::AddResourceMap(const SRC_IMAGE src, const std::string& file, const std::vector<SCENE_ID>& useScenes)
{
	unique_ptr<ImageResource> res;

	res = make_unique<ImageResource>(file);
	resourcesMap_.emplace(src, move(res));
	resourcesUseMap_.emplace(src, useScenes);
}
