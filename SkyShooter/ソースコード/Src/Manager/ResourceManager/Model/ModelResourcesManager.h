#pragma once

#include <mutex>
#include "ModelResource.h"
#include "../ResourcesManagerTemplate.h"

/// <summary>
/// モデルのリソース名
/// </summary>
enum class SRC_MODEL
{
	BOT_X,					//プレイヤー
	Idle,					//静止状態アニメーション
	Run,					//走るアニメーション
	Flying,					//飛ぶアニメーション
	Attack,					//攻撃アニメーション
	WATERWAVE_CUBE,			//水面
	SKYDOME,				//スカイドーム
	SHOT,					//弾
	ENEMY,					//敵
	ENEMY_BOXATTACK_CUBE,	//敵の箱攻撃	
	UltAttack,				//Ultアニメーションの
	Plane,					//Ultアニメーションの
	MAX,					//最大数カウント用
};

/// <summary>
/// モデルの管理
/// </summary>
class ModelResourcesManager
	: 
	public ResourcesManagerTemplate<
	ModelResourcesManager, 
	ModelResource,
	SRC_MODEL
	>
{

public:

	ModelResourcesManager(void);
	virtual ~ModelResourcesManager(void) override;

	/// <summary>
	/// 指定したリソースの複製ロード
	/// </summary>
	/// <returns>複製されたモデルのハンドルID</returns>
	int LoadModelDuplicate(const SRC_MODEL src);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// 指定したリソースを解放
	/// </summary>
	/// <param name="src">リソース名</param>
	void Release(const SRC_MODEL src);

private:

	std::mutex mutex_;

	//読込
	ModelResource& _Load(const SRC_MODEL src) override;

	//モデルの登録
	void AddResourceMap(const SRC_MODEL src, const std::string& file);
	void AddResourceMap(
		const SRC_MODEL src, 
		const std::string& file,
		const std::vector<SCENE_ID>& useScene
	);

};

