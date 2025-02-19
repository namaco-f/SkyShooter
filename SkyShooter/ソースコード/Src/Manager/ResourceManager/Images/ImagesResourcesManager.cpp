#include "ImagesResourcesManager.h"

using namespace std;

ImagesResourcesManager::ImagesResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	AddResourceMap(
		SRC_IMAGES::KEY_ENTER,
		"UI/ControllerType/Keyboard/key/Enter/ENTER.png",
		Vector2(2, 1),
		Vector2(42, 31)
	);

	AddResourceMap(
		SRC_IMAGES::LOADING,
		"UI/Loading/Loading.png",
		Vector2(12, 1),
		Vector2(131, 128),
		USESCENE_ALL
	);
}

ImagesResourcesManager::~ImagesResourcesManager(void)
{
}

void ImagesResourcesManager::Release(void)
{
	for (auto& lM : loadedMap_)
	{
		lM.second.Release();
	}
	loadedMap_.clear();
}

ImagesResource& ImagesResourcesManager::_Load(const SRC_IMAGES src)
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

void ImagesResourcesManager::AddResourceMap(
	const SRC_IMAGES src, 
	const std::string& file,
	const Vector2& num,
	const Vector2& size
)
{
	std::unique_ptr<ImagesResource> res;

	res = make_unique<ImagesResource>(file, num, size);
	resourcesMap_.emplace(src, move(res));
}

void ImagesResourcesManager::AddResourceMap(const SRC_IMAGES src, const std::string& file, const Vector2& num, const Vector2& size, const std::vector<SCENE_ID>& useScenes)
{
	std::unique_ptr<ImagesResource> res;

	res = make_unique<ImagesResource>(file, num, size);
	resourcesMap_.emplace(src, move(res));
	resourcesUseMap_.emplace(src, useScenes);
}
