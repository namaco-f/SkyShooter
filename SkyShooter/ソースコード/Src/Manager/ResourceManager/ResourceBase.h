#pragma once
#include <string>
#include <vector>
#include "../SceneId.h"

/// <summary>
/// リソースの読込、解放の基底クラス
/// </summary>
class ResourceBase
{

public:

	ResourceBase(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="path">ファイルパス</param>
	explicit ResourceBase(const std::string& path);
	virtual ~ResourceBase(void);

	/// <summary>
	/// 読込
	/// </summary>
	virtual void Load(void) = 0;

	/// <summary>
	/// 解放
	/// </summary>
	virtual void Release(void) = 0;

	/// <summary>
	/// ハンドルIDの取得
	/// </summary>
	virtual const int& GetHandleId(void) const;

	/// <summary>
	/// 指定したシーンでリソースを使用するか
	/// </summary>
	/// <param name="scnId">SCENE_ID</param>
	/// <returns>
	/// true : 使用する
	/// false : 使用しない
	/// </returns>
	virtual const bool IsUseScene(const SCENE_ID scnId) const;
	
	/// <summary>
	/// 使用するシーンを取得
	/// </summary>
	/// <returns>useSceneIds_</returns>
	inline virtual const std::vector<SCENE_ID>& GetUseScenes(void) const
	{
		return useSceneIds_;
	}

protected:

	//ハンドルID
	int handleId_;

	//パス
	std::string path_;

	//使用するシーン
	std::vector<SCENE_ID> useSceneIds_;

private:

};

