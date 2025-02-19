#pragma once
#include "MaterialBase.h"

/// <summary>
///　ピクセルシェーダのリソース
/// </summary>
class PixelMaterial 
	: public MaterialBase
{

public:

	/// <summary>
	/// テクスチャアドレスタイプ
	/// </summary>
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="shaderFileName">Data/Shader/以降のパス</param>
	/// <param name="constBufFloat4Size">FLOAT4型の定数バッファの数</param>
	PixelMaterial(const std::string& shaderFileName, const int cbFloat4Size);
	virtual ~PixelMaterial(void);

	/// <summary>
	/// リソースの解放
	/// </summary>
	void Release(void) override;

	/// <summary>
	/// ファイルを指定してシェーダを読込
	/// </summary>
	/// <param name="str">Data/Shader/以降のパス</param>
	void LoadShader(const std::string& str) override;

	/// <summary>
	/// シェーダの読込
	/// </summary>
	void LoadShader(void) override;

	/// <summary>
	/// テクスチャバッファを追加
	/// </summary>
	/// <param name="texDiffuse">テクスチャの画像ハンドルID</param>
	void AddTextureBuf(int texDiffuse);

	/// <summary>
	/// テクスチャを更新
	/// </summary>
	/// <param name="idx">インデックス</param>
	/// <param name="texDiffuse">テクスチャの画像ハンドルID</param>
	void SetTextureBuf(int idx, int texDiffuse);

	/// <summary>
	/// テクスチャの配列を取得
	/// </summary>
	/// <returns>textures_</returns>
	inline const std::vector<int>& GetTextures(void) const
	{
		return textures_;
	}

	/// <summary>
	/// テクスチャアドレスを取得
	/// </summary>
	/// <returns>texAddress_</returns>
	inline TEXADDRESS GetTextureAddress(void) const
	{
		return texAddress_;
	}

	/// <summary>
	/// テクスチャアドレスを設定
	/// </summary>
	/// <param name="texA">TEXADDRESS</param>
	inline void SetTextureAddress(const TEXADDRESS texA)
	{
		texAddress_ = texA;
	}

private:

	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// テクスチャ
	std::vector<int> textures_;

};



