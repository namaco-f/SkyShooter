#include "../../../Application.h"
#include "FontResourcesManager.h"

using namespace std;

FontResourcesManager::FontResourcesManager(void)
{
	//サイズ60のpゴシック
	AddResourceMap(SRC_FONT::Pgothic_s60, "ms_p_gothic_s60.dft",3);

	//サイズ50のpゴシック
	AddResourceMap(SRC_FONT::Pgothic_s50, "ms_p_gothic_s50.dft",3);

	//サイズ40のpゴシック
	AddResourceMap(SRC_FONT::Pgothic_s40, "ms_p_gothic_s40.dft",3);

	//サイズ32のpゴシック
	AddResourceMap(SRC_FONT::Pgothic_s32, "ms_p_gothic_s32.dft",2);

	//サイズ20のpゴシック
	AddResourceMap(SRC_FONT::Pgothic_s20, "ms_p_gothic_s20.dft",2);
}

FontResourcesManager::~FontResourcesManager(void)
{
}

void FontResourcesManager::Release(void)
{
	for (auto& lM : loadedMap_)
	{
		lM.second.Release();
	}
	loadedMap_.clear();
}

FontResource& FontResourcesManager::_Load(const SRC_FONT src)
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

void FontResourcesManager::AddResourceMap(const SRC_FONT src, const std::string& file, const int edgeSize)
{
	unique_ptr<FontResource> res;

	res = make_unique<FontResource>(file);
	res->SetEdgeSize(edgeSize);

	resourcesMap_.emplace(src, move(res));
}
