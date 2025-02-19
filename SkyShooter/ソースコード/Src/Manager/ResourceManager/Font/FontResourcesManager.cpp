
#include "../../../Application.h"
#include "FontResourcesManager.h"

using namespace std;

FontResourcesManager::FontResourcesManager(void)
{
	AddResourceMap(SRC_FONT::Pgothic_s60, "MS_Pgothic_s60.dft",3);
	AddResourceMap(SRC_FONT::Pgothic_s50, "MS_Pgothic_s50.dft",3);
	AddResourceMap(SRC_FONT::Pgothic_s40, "MS_Pgothic_s40.dft",3);
	AddResourceMap(SRC_FONT::Pgothic_s32, "MS_Pgothic_s32.dft",2);
	AddResourceMap(SRC_FONT::Pgothic_s20, "MS_Pgothic_s20.dft",2);
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
		//“ÇžÏ
		return loadedMap_.at(src);
	}

	if (!resourcesMap_.contains(src))
	{
		//–¢“o˜^
		return unregistered;
	}

	//“o˜^ÏA–¢“Çž
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
