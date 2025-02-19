#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "../../Common/Vector2/Vector2.h"
#include "../SceneId.h"

/// <summary>
/// 各リソース管理クラス
/// </summary>
/// <typeparam name="T">リソース管理クラス</typeparam>
/// <typeparam name="U">リソースクラス</typeparam>
/// <typeparam name="SRC">リソース名</typeparam>
template<class T,class U,typename SRC>
class ResourcesManagerTemplate
{
	
public:

	ResourcesManagerTemplate(void)
		:
		unregistered(U()),
		resourcesMap_ {},
		loadedMap_{},
		resourcesUseMap_{}
	{
	}

	virtual ~ResourcesManagerTemplate(void)
	{
		resourcesMap_.clear();
	}

	/// <summary>
	/// 読込済のリソースを返す関数
	/// </summary>
	virtual U& Load(const SRC src)
	{
		U& ret = _Load(src);
		return ret;
	}

	virtual void LoadUseScene(const SCENE_ID sceneId)
	{
		for (const auto& resourcesScenes : resourcesUseMap_)
		{
			for (const auto& scenes : resourcesScenes.second)
			{
				if (scenes == sceneId)
				{
					Load(resourcesScenes.first);
				}
			}
		}
	}


protected:

	//未登録の際に返す実体
	U unregistered;

	//登録されたリソース
	std::unordered_map<SRC, std::unique_ptr<U>> resourcesMap_;

	//読込済のリソース
	std::unordered_map<SRC, U&> loadedMap_;

	//登録されたリソース
	std::unordered_map<SRC, std::vector<SCENE_ID>> resourcesUseMap_;

	//読込処理
	virtual U& _Load(SRC src) = 0;


private:

};
