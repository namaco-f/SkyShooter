#pragma once
#include <unordered_map>
#include <memory>
#include "JSONResource.h" 
#include "../../Singleton.h" 

/// <summary>
/// JSONリソース名
/// </summary>
enum class SRC_JSON
{
	INPUT_INIT_DATA,
	BOX_ATTACK_POS_DATA,
};

/// <summary>
/// JSONの登録、管理
/// </summary>
class JSONManager
	: public Singleton<JSONManager>
{

public:

	/// <summary>
	/// 指定したJSONデータ読み込み
	/// </summary>
	/// <param name="src">指定するデータ名</param>
	/// <returns>JSONResourceの参照</returns>
	const JSONResource& Load(const SRC_JSON src);

private:

	friend class Singleton<JSONManager>;

	//未登録
	JSONResource unregistered;

	JSONManager(void);
	virtual ~JSONManager(void) override;

	//登録済
	std::unordered_map<SRC_JSON, std::unique_ptr<JSONResource>> jsonDatas_;

	//読込済
	std::unordered_map<SRC_JSON, JSONResource&> jsonLoadedDatas_;

	//読込処理
	JSONResource& _Load(const SRC_JSON src);

	void AddJSONData(const SRC_JSON src, const std::string& path);

};

