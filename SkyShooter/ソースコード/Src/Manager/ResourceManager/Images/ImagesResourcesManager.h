#pragma once

#include "ImagesResource.h"
#include "../ResourcesManagerTemplate.h"

/// <summary>
/// 複数画像のリソース名
/// </summary>
enum class SRC_IMAGES
{
	KEY_ENTER,		//エンターキー画像
	LOADING,		//ローディング画像
	MAX,
};

/// <summary>
/// 複数画像の管理
/// </summary>
class ImagesResourcesManager
	: public ResourcesManagerTemplate<
	ImagesResourcesManager,
	ImagesResource,
	SRC_IMAGES
	>
{

public:

	ImagesResourcesManager(void);
	virtual ~ImagesResourcesManager(void) override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);


private:

	//読込処理
	ImagesResource& _Load(const SRC_IMAGES src) override;

	/// <summary>
	/// リソース配列に追加
	/// </summary>
	/// <param name="src">リソース名</param>
	/// <param name="file">パス</param>
	/// <param name="num">分割数</param>
	/// <param name="size">一枚のサイズ</param>
	void AddResourceMap(
		const SRC_IMAGES src, 
		const std::string& file,
		const Vector2& num,
		const Vector2& size
	);

	/// <summary>
	/// リソース配列に追加
	/// </summary>
	/// <param name="src">リソース名</param>
	/// <param name="file">パス</param>
	/// <param name="num">分割数</param>
	/// <param name="size">一枚のサイズ</param>
	void AddResourceMap(
		const SRC_IMAGES src,
		const std::string& file,
		const Vector2& num,
		const Vector2& size,
		const std::vector<SCENE_ID>& useScenes
	);

};

