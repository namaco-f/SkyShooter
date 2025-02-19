#pragma once

#include <DxLib.h>
#include <vector>
#include <string>


/// <summary>
/// シェーダリソースの基底クラス
/// </summary>
class MaterialBase
{

public:

	MaterialBase(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="shaderFileName">Data/Shader/以降のファイルパス</param>
	/// <param name="cbFloat4Size">FLOAT4型の定数バッファの数</param>
	MaterialBase(
		const std::string& shaderFileName, const int cbFloat4Size);
	virtual ~MaterialBase(void);

	/// <summary>
	/// リソースの解放
	/// </summary>
	virtual void Release(void);

	/// <summary>
	/// シェーダの読込
	/// </summary>
	/// <param name="str">ファイルを指定して読込</param>
	virtual void LoadShader(const std::string& str) = 0;

	/// <summary>
	/// シェーダの読込
	/// </summary>
	virtual void LoadShader(void) = 0;

	/// <summary>
	/// 定数バッファを追加
	/// </summary>
	/// <param name="cbParam">FLOAT4データ型</param>
	void AddConstBuf(const FLOAT4& cbParam);
	void AddConstBuf(const VECTOR& valA,const float& valB);

	/// <summary>
	/// 定数バッファを更新
	/// </summary>
	/// <param name="idx">更新する定数バッファのインデックス</param>
	/// <param name="cbParam">更新する定数バッファのFLOAT4データ型</param>
	void SetConstBuf(const int idx, const FLOAT4& cbParam);
	void SetConstBuf(const int idx, const VECTOR& valA, const float& valB);

	/// <summary>
	/// シェーダハンドルの取得
	/// </summary>
	/// <returns>シェーダハンドルの参照</returns>
	inline const int& GetShader(void) const
	{
		return shaderHandle_;
	}

	/// <summary>
	/// 定数バッファハンドルの取得
	/// </summary>
	/// <returns>定数バッファハンドルの参照</returns>
	inline const int& GetConstBuf(void) const
	{
		return constBufHandle_;
	}

	/// <summary>
	/// 定数バッファ配列の取得
	/// </summary>
	/// <returns>定数バッファ配列の参照</returns>
	inline const std::vector<FLOAT4>& GetConstBufs(void) const
	{
		return constBufParams_;
	}


protected:

	//シェーダーハンドル
	int shaderHandle_;
	
	//定数バッファハンドル
	int constBufHandle_;

	//ファイルパス
	std::string filePath_;
	
	//定数バッファのFLOAT4型配列のサイズ
	int constBufFloat4Size_;
	
	// 定数バッファの値
	std::vector<FLOAT4> constBufParams_;

	//定数バッファハンドルの初期化
	void CreateConstBuf(void);

private:

};

