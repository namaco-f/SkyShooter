#include <Dxlib.h>
#include <algorithm>
#include "ModelResourcesManager.h"

using namespace std;

ModelResourcesManager::ModelResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	//モデル
	//プレイヤー
	AddResourceMap(
		SRC_MODEL::BOT_X,"Mixamo/x_bot.mv1",
		USESCENE_GAME_ONLY
	);

	//水面
	AddResourceMap(
		SRC_MODEL::WATERWAVE_CUBE, "Stage/Water/waterwave_cube.mv1",
		USESCENE_ALL
	);

	//スカイドーム
	AddResourceMap(SRC_MODEL::SKYDOME, "Stage/Skydome/skydome.mv1",
		USESCENE_ALL
	);

	//弾
	AddResourceMap(SRC_MODEL::SHOT, "Shot/shot.mv1",
		USESCENE_GAME_ONLY
	);

	//ボス敵
	AddResourceMap(
		SRC_MODEL::ENEMY, "Enemy/boss.mv1",
		USESCENE_GAME_ONLY
	);

	//ボス矩形攻撃
	AddResourceMap(
		SRC_MODEL::ENEMY_BOXATTACK_CUBE, "Primitive/Cube/primitive_cube.mv1",
		USESCENE_GAME_ONLY
	);

	AddResourceMap(SRC_MODEL::Plane, "Primitive/Plane/plane.mv1");

	//アニメーション
	//静止
	AddResourceMap(
		SRC_MODEL::Idle, "Animation/Mixamo/idle.mv1",
		USESCENE_GAME_ONLY
	);

	//飛行
	AddResourceMap(
		SRC_MODEL::Flying, "Animation/Mixamo/flying.mv1",
		USESCENE_GAME_ONLY
	);

	//攻撃モーション
	AddResourceMap(
		SRC_MODEL::Attack, "Animation/Mixamo/Player/attack.mv1",
		USESCENE_GAME_ONLY
	);

	//ウルトモーション
	AddResourceMap(
		SRC_MODEL::UltAttack, "Animation/Mixamo/Player/ult_attack.mv1",
		USESCENE_GAME_ONLY
	);
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