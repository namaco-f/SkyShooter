#pragma once

#include <memory>
#include <string>
#include <Dxlib.h>

class PixelMaterial;
class VertexsMaterial;
class Renderer;

/// <summary>
/// 各シェーダーやレンダラーの管理
/// </summary>
class ShaderResource
{
public:

	ShaderResource(void) = default;
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="psFileName">ピクセルシェーダーのファイル名</param>
	/// <param name="psCbSizeFloat4">ピクセルシェーダー定数バッファ用 : FLOAT4の最大数</param>
	/// <param name="vsFileName">頂点シェーダーのファイル名</param>
	/// <param name="vsCbSizeFloat4">頂点シェーダー定数バッファ用 : FLOAT4の最大数</param>
	ShaderResource(
		const std::string& psFileName, 
		const int& psCbSizeFloat4,
		const std::string& vsFileName,
		const int& vsCbSizeFloat4
	);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="psFileName">ピクセルシェーダーのファイル名</param>
	/// <param name="psCbSizeFloat4">ピクセルシェーダー定数バッファ用 : FLOAT4の最大数</param>
	ShaderResource(
		const std::string& psFileName,
		const int& psCbSizeFloat4);

	virtual ~ShaderResource(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// シェーダーの読込処理
	/// </summary>
	void Load(void);

	/// <summary>
	/// 頂点シェーダーを使用しているか
	/// </summary>
	const bool IsUsedVS(void) const;

	/// <summary>
	/// 頂点シェーダーの定数バッファに書込
	/// </summary>
	/// <param name="val">FLOAT4型</param>
	void AddConstBuf_VS(const FLOAT4& val);

	/// <summary>
	/// ピクセルシェーダーの定数バッファに書込
	/// </summary>
	/// <param name="val">FLOAT4型</param>
	void AddConstBuf_PS(const FLOAT4& val);
	
	/// <summary>
	/// ピクセルシェーダークラスの取得
	/// </summary>
	/// <returns>weak_ptr</returns>
	std::weak_ptr<PixelMaterial> GetPS(void) const;

	/// <summary>
	/// 頂点シェーダークラスの取得
	/// </summary>
	/// <returns>weak_ptr</returns>
	std::weak_ptr<VertexsMaterial> GetVS(void) const;

	/// <summary>
	/// レンダラークラスの取得
	/// </summary>
	/// <returns>weak_ptr</returns>
	std::weak_ptr<Renderer> GetRenderer(void);

	/// <summary>
	/// レンダラー作成
	/// </summary>
	void MakeRenderer(void);

private:

	//ピクセルシェーダー
	std::shared_ptr<PixelMaterial> psMaterial_;

	//頂点シェーダー
	std::shared_ptr<VertexsMaterial> vsMaterial_;

	//レンダラー
	std::shared_ptr<Renderer> renderer_;
};

