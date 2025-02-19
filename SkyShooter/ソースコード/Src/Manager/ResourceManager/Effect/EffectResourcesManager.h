#pragma once

#include "../ResourcesManagerTemplate.h"
#include "EffectResource.h"

//エフェクトリソース名
enum class SRC_EFFECT
{
	HIT,		//ヒットエフェクト
	ULT,		//Ultエフェクト
	ULT_CHARGE,	//シールド
	WARP,		//ワープエフェクト
	SLASTER,	//スラスターエフェクト
	WATER,		//水飛沫エフェクト
	SHIELD,		//シールド
	MAX,		//最大数カウント用
};

/// <summary>
/// エフェクトの管理
/// </summary>
class EffectResourcesManager
	: 
	public ResourcesManagerTemplate<
	EffectResourcesManager,
	EffectResource,
	SRC_EFFECT
	>
{

public:

	EffectResourcesManager(void);
	virtual ~EffectResourcesManager(void) override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);


private:

	//読込
	EffectResource& _Load(const SRC_EFFECT src) override;

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="src">リソース名</param>
	/// <param name="file">ファイル名</param>
	void AddResourceMap(const SRC_EFFECT src, const std::string& file);
	void AddResourceMap(
		const SRC_EFFECT src, 
		const std::string& file,
		const std::vector<SCENE_ID>& useSceneId);

};

