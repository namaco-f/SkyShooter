#pragma once

#include "../ResourcesManagerTemplate.h"
#include"FontResource.h"

/// <summary>
/// 画像リソースの名前
/// </summary>
enum class SRC_FONT
{
	Pgothic_s20,
	Pgothic_s32,
	Pgothic_s40,
	Pgothic_s50,
	Pgothic_s60,
	MAX,
};

/// <summary>
/// 画像の管理
/// </summary>
class FontResourcesManager
	:
	public ResourcesManagerTemplate<
	FontResourcesManager,
	FontResource,
	SRC_FONT
	>
{

public:

	FontResourcesManager(void);
	virtual ~FontResourcesManager(void) override;

	/// <summary>
	/// 解放
	/// </summary>
	void Release(void);

private:

	//読込処理
	FontResource& _Load(const SRC_FONT src) override;

	//エフェクトの追加
	void AddResourceMap(const SRC_FONT src, const std::string& file,const int edgeSize = 0);

};

