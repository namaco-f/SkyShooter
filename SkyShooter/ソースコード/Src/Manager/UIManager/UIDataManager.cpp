#include "UIDataManager.h"

using namespace std;

const float DEF_SOUND_VOLUME = 0.5f;
const float DEF_FLOAT = 0.0f;
const int DEF_INT = 0;

UIDataManager* Singleton<UIDataManager>::instance_ = nullptr;

UIDataManager::UIDataManager(void)
	:
	uiDataMap_{}
{
	UIDataInit();
}

UIDataManager::~UIDataManager(void)
{
	uiDataMap_.clear();
}

void UIDataManager::UIDataInit(void)
{
	//intå^à»äOÇÃèâä˙âª
	//AddUIData<float>(SRC_UIData::CLEAR_TIME, DEF_FLOAT);
	//AddUIData<float>(SRC_UIData::PLAYER_SHILD_COOLPAR, DEF_FLOAT);
	//AddUIData<float>(SRC_UIData::PLAYER_SHILD_TIME, DEF_FLOAT);
	//AddUIData<float>(SRC_UIData::PLAYER_SHILD_LIMIT, DEF_FLOAT);
	AddUIData<float>(SRC_UIData::BGM_VOLUME, DEF_SOUND_VOLUME);
	AddUIData<float>(SRC_UIData::SE_VOLUME, DEF_SOUND_VOLUME);

	for (int n = 0 ; n < (int)SRC_UIData::MAX ; n++)
	{
		if (uiDataMap_.contains((SRC_UIData)n))
		{
			//intå^à»äOÇ≈èâä˙âªçœÇ›
			continue;
		}

		uiDataMap_.emplace((SRC_UIData)n, -1);
	}

	
}
