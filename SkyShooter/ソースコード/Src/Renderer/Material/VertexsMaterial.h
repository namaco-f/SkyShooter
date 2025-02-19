#pragma once
#include "MaterialBase.h"

/// <summary>
/// 頂点シェーダのリソース
/// </summary>
class VertexsMaterial 
	: public MaterialBase
{
public:

	/// <summary>
	///コンストラクタ
	/// </summary>
	/// <param name="shaderFilePath">Data/Shader/以降のパス</param>
	/// <param name="_cbSize">FLOAT4型の定数バッファの数</param>
	VertexsMaterial(
		const std::string& shaderFileName, 
		const int cbSize
	);
	virtual ~VertexsMaterial(void);

	/// <summary>
	/// ファイルを指定してシェーダを読込
	/// </summary>
	/// <param name="shaderFileName"></param>
	void LoadShader(const std::string& shaderFileName) override;

	/// <summary>
	/// シェーダの読込
	/// </summary>
	void LoadShader(void) override;

private:

};

