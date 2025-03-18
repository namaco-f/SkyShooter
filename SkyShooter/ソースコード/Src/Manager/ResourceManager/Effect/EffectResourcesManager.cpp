#include "EffectResourcesManager.h"

using namespace std;

EffectResourcesManager::EffectResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	//ヒット
	AddResourceMap(SRC_EFFECT::HIT, "BlastShot/blast.efk", USESCENE_GAME_ONLY);

	//ウルト
	AddResourceMap(SRC_EFFECT::ULT, "Ult/Ult_Laser/ult_laser.efk", USESCENE_GAME_ONLY);

	//ワープ
	AddResourceMap(SRC_EFFECT::WARP, "Warp/warp.efk", USESCENE_GAME_ONLY);

	//スラスター
	AddResourceMap(SRC_EFFECT::SLASTER, "Enemy_Slaster/enemy_slaster.efk", USESCENE_GAME_ONLY);

	//シールド
	AddResourceMap(SRC_EFFECT::SHIELD, "Shield/shield.efk", USESCENE_GAME_ONLY);

	//ウルトチャージ
	AddResourceMap(SRC_EFFECT::ULT_CHARGE, "Ult/Ult_Charge/ult_charge.efk", USESCENE_GAME_ONLY);
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
