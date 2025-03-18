#include "ShaderManager.h"

using namespace std;

void ShaderManager::Init(void)
{
	
}

void ShaderManager::Release(void)
{
	for (auto& lM : loadedMap_)
	{
		lM.second.Release();
	}
	loadedMap_.clear();
}

ShaderManager::ShaderManager(void)
	:
	ResourcesManagerTemplate()
{
	AddResourceMap(
		SRC_SHADER::UV_SCROLL,
		"UVScroll/uv_scroll.pso", 2,
		"UVScroll/uv_scroll.vso",1
	);

	AddResourceMap(
		SRC_SHADER::STANDARD_MODEL,
		"Standard/model_standard.pso", 1,
		"Standard/model_standard.vso", 3
	);

	AddResourceMap(
		SRC_SHADER::BOX_SHADER,
		"enemy_attack_box.pso", 3,
		"enemy_attack_box.vso", 2
	);
}

ShaderManager::~ShaderManager(void)
{
}

ShaderResource& ShaderManager::_Load(const SRC_SHADER src)
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

void ShaderManager::AddResourceMap(const SRC_SHADER src, const std::string& psPath, const int& psCbSizeFloat4)
{
	using SR = ShaderResource;
	std::unique_ptr<SR> sr;

	sr = make_unique<SR>(psPath, psCbSizeFloat4);
	resourcesMap_.emplace(src, move(sr));
}

void ShaderManager::AddResourceMap(const SRC_SHADER src, const std::string& psPath, const int& psCbSizeFloat4, const std::string& vsPath, const int& vsCbSizeFloat4)
{
	using SR = ShaderResource;
	std::unique_ptr<SR> sr;

	sr = make_unique<SR>(
		psPath, psCbSizeFloat4, vsPath, vsCbSizeFloat4);
	resourcesMap_.emplace(src, move(sr));

}
