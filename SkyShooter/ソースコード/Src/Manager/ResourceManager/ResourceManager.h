#pragma once

//リソース管理クラスの管理

#include <memory>
#include <functional>
#include <list>
#include "../../Singleton.h"
#include "../SceneId.h"

class ModelResourcesManager;
class SoundResourcesManager;
class ImageResourcesManager;
class ImagesResourcesManager;
class EffectResourcesManager;
class FontResourcesManager;
class ShaderManager;

/// <summary>
/// 各リソース管理クラスの管理
/// </summary>
class ResourceManager
	: public Singleton<ResourceManager>
{

public:

	/// <summary>
	/// Dxlib_End前に解放
	/// </summary>
	void Destroy(void) override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);

	void Update(void);

	void Load(const SCENE_ID scene);

	inline const bool GetIsLoadingNow(void) const
	{
		return (int)(loadingFunc_.size()) > 0;
	}

	/// <summary>
	/// モデル管理クラスの取得
	/// </summary>
	inline  ModelResourcesManager& GetModelResourcesInstance(void)
	{
		return *modelResourcesManager_;
	}

	/// <summary>
	/// 音管理クラスの取得
	/// </summary>
	inline SoundResourcesManager& GetSoundResourcesInstance(void)
	{
		return *soundResourcesManager_;
	}

	/// <summary>
	/// 画像管理クラスの取得
	/// </summary>
	inline ImageResourcesManager& GetImageResourcesInstance(void)
	{
		return *imageResourcesManager_;
	}

	/// <summary>
	/// 複数画像管理クラスの取得
	/// </summary>
	inline ImagesResourcesManager& GetImagesResourcesInstance(void)
	{
		return *imagesResourcesManager_;
	}

	/// <summary>
	///	エフェクト管理クラスの取得
	/// </summary>
	inline EffectResourcesManager& GetEffectResourcesInstance(void)
	{
		return *effectResourcesManager_;
	}	
	
	/// <summary>
	///	エフェクト管理クラスの取得
	/// </summary>
	inline FontResourcesManager& GetFontResourcesInstance(void)
	{
		return *fontResourcesManager_;
	}

	/// <summary>
	///	シェーダー管理クラスの取得
	/// </summary>
	inline ShaderManager& GetShaderResourcesInstance(void)
	{
		return *shaderResourcesManager_;
	}


private:

	friend class Singleton<ResourceManager>;

	//モデル
	std::unique_ptr<ModelResourcesManager> modelResourcesManager_;

	//音
	std::unique_ptr<SoundResourcesManager> soundResourcesManager_;

	//画像
	std::unique_ptr<ImageResourcesManager> imageResourcesManager_;
	
	//複数画像
	std::unique_ptr<ImagesResourcesManager> imagesResourcesManager_;

	//エフェクト
	std::unique_ptr<EffectResourcesManager> effectResourcesManager_;

	//シェーダー
	std::unique_ptr<ShaderManager> shaderResourcesManager_;

	//フォント
	std::unique_ptr<FontResourcesManager> fontResourcesManager_;

	std::list<std::function<void(void)>> loadingFunc_;

	float loadingCnt_;
	bool isLoadingNow_;

	SCENE_ID nowLoadingSceneResouce_;
	
	ResourceManager(void);
	virtual ~ResourceManager(void) override;

	void ChangeLoadResource(void);

};

