#pragma once

#include<string>
#include<vector>
#include<unordered_map>
#include <Dxlib.h>
#include"../Lib/nlohmann/json.hpp"

class JSONResource
{
public:

	JSONResource(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fileName">Data/JSON/以降のパス</param>
	explicit JSONResource(const std::string& fileName);
	virtual ~JSONResource(void);

	/// <summary>
	/// 読込
	/// </summary>
	void Load(void);

	/// <summary>
	/// 読込済JSONリソースの取得
	/// </summary>
	const nlohmann::json& GetJSONData(void) const
	{
		return jsonData_;
	}

private:

	//ファイルパス
	std::string fileName_;

	//JSONリソース
	nlohmann::json jsonData_;
};


