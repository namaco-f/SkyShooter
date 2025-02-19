
#include <Dxlib.h>
#include <algorithm>
#include "ModelResourcesManager.h"

using namespace std;

ModelResourcesManager::ModelResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	AddResourceMap(
		SRC_MODEL::BOT_X,"Mixamo/X_Bot.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(
		SRC_MODEL::Idle, "Animation/Mixamo/Idle.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(
		SRC_MODEL::Flying, "Animation/Mixamo/Flying.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(
		SRC_MODEL::WATERWAVE_CUBE, "Stage/Water/WaterWaveCube.mv1",
		USESCENE_ALL
	);
	AddResourceMap(SRC_MODEL::SKYDOME, "Stage/Skydome/Skydome.mv1",
		USESCENE_ALL
	);
	AddResourceMap(SRC_MODEL::SHOT, "Shot/Shot.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(
		SRC_MODEL::ENEMY, "Enemy/uploads_files_3373629_star+ship.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(
		SRC_MODEL::ENEMY_BOXATTACK_CUBE, "Primitive/Cube/PrimitiveModel_Cube.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(
		SRC_MODEL::Attack, "Animation/Mixamo/Player/Mutant_Swiping.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(
		SRC_MODEL::UltAttack, "Animation/Mixamo/Player/Magic_Attack_02.mv1",
		USESCENE_GAME_ONLY
	);
	AddResourceMap(SRC_MODEL::Plane, "Primitive/Plane/Plane.mv1");
}

ModelResourcesManager::~ModelResourcesManager(void)
{
}

int ModelResourcesManager::LoadModelDuplicate(const SRC_MODEL src)
{

	//モデルの登録確認(未読込の場合二重ロードになるため注意)
	ModelResource& res = _Load(src);

	//モデルの複製ロード
	int duId = MV1DuplicateModel(res.GetHandleId());
	res.Add_DuplicateModelId(duId);

	return duId;

}

void ModelResourcesManager::Release(void)
{

	for (auto& lM : loadedMap_)
	{
		lM.second.Release();
	}
	loadedMap_.clear();

}

void ModelResourcesManager::Release(const SRC_MODEL src)
{
	if (loadedMap_.contains(src))
	{
		//リソース解放
		auto& lM = loadedMap_.at(src);
		lM.Release();
		loadedMap_.erase(src);
	}
}


ModelResource& ModelResourcesManager::_Load(const SRC_MODEL src)
{
	unique_lock<mutex>(mutex_);

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

void ModelResourcesManager::AddResourceMap(const SRC_MODEL src, const std::string& file)
{
	unique_ptr<ModelResource> res;

	res = make_unique<ModelResource>(file);
	resourcesMap_.emplace(src, move(res));
}

void ModelResourcesManager::AddResourceMap(const SRC_MODEL src, const std::string& file, const std::vector<SCENE_ID>& useScene)
{
	unique_ptr<ModelResource> res;

	res = make_unique<ModelResource>(file);
	resourcesMap_.emplace(src, move(res));
	resourcesUseMap_.emplace(src, useScene);
}