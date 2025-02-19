#pragma once

#include <unordered_map>
#include "ShaderResource.h"
#include "../ResourcesManagerTemplate.h"

/// <summary>
/// シェーダー名
/// </summary>
enum class SRC_SHADER
{
	UV_SCROLL,			//UVスクロール
	STANDARD_MODEL,		//モデルのデフォルトシェーダ
	//MRT_TEST,			//マルチレンダーターゲットテスト用
	BOX_SHADER,			//箱攻撃のシェーダ
	//TRAIL_TEST,			//軌跡用
};

/// <summary>
/// シェーダの管理
/// </summary>
class ShaderManager
	: 
	public ResourcesManagerTemplate<
	ShaderManager,
	ShaderResource,
	SRC_SHADER
	>
{
public:

	ShaderManager(void);
	virtual ~ShaderManager(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);

private:

	//読込
	ShaderResource& _Load(const SRC_SHADER src) override;

	/// <summary>
	/// リソースマップに追加
	/// </summary>
	/// <param name="src">シェーダー名</param>
	/// <param name="psPath">ピクセルシェーダーのパス</param>
	/// <param name="psCbSizeFloat4">ピクセルシェーダーの定数バッファ : FLOAT4の数</param>
	void AddResourceMap(const SRC_SHADER src,const std::string& psPath , const int& psCbSizeFloat4);
	
	/// <summary>
	/// リソースマップに追加
	/// </summary>
	/// <param name="src">シェーダー名</param>
	/// <param name="psPath">ピクセルシェーダーのパス</param>
	/// <param name="psCbSizeFloat4">ピクセルシェーダーの定数バッファ : FLOAT4の数</param>
	/// <param name="vsPath">頂点シェーダーのパス</param>
	/// <param name="vsCbSizeFloat4">頂点シェーダーの定数バッファ : FLOAT4の数</param>
	void AddResourceMap(
		const SRC_SHADER src,
		const std::string& psPath, const int& psCbSizeFloat4,
		const std::string& vsPath, const int& vsCbSizeFloat4);

};

