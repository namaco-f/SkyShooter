#include "EffectResourcesManager.h"

using namespace std;

EffectResourcesManager::EffectResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	AddResourceMap(SRC_EFFECT::HIT, "BlastShot/Blast.efk", USESCENE_GAME_ONLY);
	AddResourceMap(SRC_EFFECT::ULT, "Ult/Ult_Laser/Ult_Laser.efk", USESCENE_GAME_ONLY);
	AddResourceMap(SRC_EFFECT::WARP, "Warp/Warp.efk", USESCENE_GAME_ONLY);
	AddResourceMap(SRC_EFFECT::SLASTER, "Enemy_Slaster/Slaster.efk", USESCENE_GAME_ONLY);
	AddResourceMap(SRC_EFFECT::SHIELD, "Shield/Shield.efk", USESCENE_GAME_ONLY);
	AddResourceMap(SRC_EFFECT::ULT_CHARGE, "Ult/Ult_Charge/Ult_Charge.efk", USESCENE_GAME_ONLY);
}

EffectResourcesManager::~EffectResourcesManager(void)
{
}

void EffectResourcesManager::Release(void)
{
	for (auto& lM : loadedMap_)
	{
		lM.second.Release();
	}
	loadedMap_.clear();
}

EffectResource& EffectResourcesManager::_Load(const SRC_EFFECT src)
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

void EffectResourcesManager::AddResourceMap(const SRC_EFFECT src, const std::string& file)
{
	unique_ptr<EffectResource> res;

	res = make_unique<EffectResource>(file);
	resourcesMap_.emplace(src, move(res));
}

void EffectResourcesManager::AddResourceMap(const SRC_EFFECT src, const std::string& file, const std::vector<SCENE_ID>& useSceneId)
{
	unique_ptr<EffectResource> res;

	res = make_unique<EffectResource>(file);
	resourcesMap_.emplace(src, move(res));
	resourcesUseMap_.emplace(src, useSceneId);
}
