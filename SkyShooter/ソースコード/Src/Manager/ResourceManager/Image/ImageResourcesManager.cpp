
#include "../../../Application.h"
#include "ImageResourcesManager.h"

using namespace std;

ImageResourcesManager::ImageResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	AddResourceMap(
		SRC_IMAGE::WATERWAVE_CUBE_TEX, "Texture/Stage/Water/Water.png");
	AddResourceMap(
		SRC_IMAGE::TrailTexture, "Texture/Trail.png",USESCENE_GAME_ONLY);
	AddResourceMap(SRC_IMAGE::CircleShadow, "Shadow/Shadow.png", USESCENE_GAME_ONLY);
	AddResourceMap(SRC_IMAGE::Keyboard_Mouse, "UI/Keyboard/Keyboard_Mouse.png", USESCENE_TITLE_ONLY);
	AddResourceMap(SRC_IMAGE::Controller, "UI/Xbox/Controller.png", USESCENE_TITLE_ONLY);
	AddResourceMap(SRC_IMAGE::TitleLogo, "Text/AGS2025w_TitleLogo_3.png",USESCENE_TITLE_ONLY);
	AddResourceMap(SRC_IMAGE::Cursor, "UI/Cursor.png",USESCENE_ALL);
	AddResourceMap(SRC_IMAGE::Arrow	 , "UI/Arrow.png", USESCENE_ALL);
	AddResourceMap(SRC_IMAGE::ClearText, "Text/Clear_Text3.png");
	AddResourceMap(SRC_IMAGE::LoadEnd_Start, "Text/Start_Text.png",USESCENE_ALL);
	AddResourceMap(
		SRC_IMAGE::OperationInstructions_Controller, 
		"UI/OperationInstructions_Controller.png",USESCENE_ALL);
	AddResourceMap(
		SRC_IMAGE::OperationInstructions_KeyBoard, 
		"UI/OperationInstructions_KeyBoard.png",USESCENE_ALL);
	AddResourceMap(
		SRC_IMAGE::HP_TEXT,
		"Text/HP.png", USESCENE_GAME_ONLY);
	AddResourceMap(
		SRC_IMAGE::MP_TEXT,
		"Text/MP_s20.png", USESCENE_GAME_ONLY);
	AddResourceMap(
		SRC_IMAGE::SE_TEXT,
		"Text/SE.png", USESCENE_ALL);
	AddResourceMap(
		SRC_IMAGE::BGM_TEXT,
		"Text/BGM.png", USESCENE_ALL);
	AddResourceMap(
		SRC_IMAGE::BOSS_ENEMY_TEXT,
		"Text/BossEnemy.png", USESCENE_GAME_ONLY);
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
		//“ÇžÏ
		return loadedMap_.at(src);
	}

	if (!resourcesMap_.contains(src))
	{
		//–¢“o˜^
		return unregistered;
	}

	//“o˜^ÏA–¢“Çž
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
