#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "../../Common/Vector2/Vector2.h"
#include "../SceneId.h"

/// <summary>
/// �e���\�[�X�Ǘ��N���X
/// </summary>
/// <typeparam name="T">���\�[�X�Ǘ��N���X</typeparam>
/// <typeparam name="U">���\�[�X�N���X</typeparam>
/// <typeparam name="SRC">���\�[�X��</typeparam>
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
	/// �Ǎ��ς̃��\�[�X��Ԃ��֐�
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

	//���o�^�̍ۂɕԂ�����
	U unregistered;

	//�o�^���ꂽ���\�[�X
	std::unordered_map<SRC, std::unique_ptr<U>> resourcesMap_;

	//�Ǎ��ς̃��\�[�X
	std::unordered_map<SRC, U&> loadedMap_;

	//�o�^���ꂽ���\�[�X
	std::unordered_map<SRC, std::vector<SCENE_ID>> resourcesUseMap_;

	//�Ǎ�����
	virtual U& _Load(SRC src) = 0;


private:

};
