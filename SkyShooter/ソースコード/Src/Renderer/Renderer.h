#pragma once

#include <DxLib.h>
#include <functional>
#include <vector>
#include <memory>
#include "../Common/Geometry2D.h"

class PixelMaterial;
class VertexsMaterial;

/// <summary>
/// レンダラー
/// </summary>
class Renderer
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pMaterial">ピクセルシェーダー : weak_ptr</param>
	/// <param name="vMaterial">頂点シェーダー : weak_ptr</param>
	Renderer(std::weak_ptr<PixelMaterial> pMaterial , std::weak_ptr<VertexsMaterial> vMaterial);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pMaterial">ピクセルシェーダー : weak_ptr</param>
	explicit Renderer(std::weak_ptr<PixelMaterial> pMaterial);

	//デストラクタ
	virtual ~Renderer(void);

	//描画矩形の生成
	void MakeSquereVertex(
		const Vector2& pos, const Vector2& size,
		size_t expectedNum = 0,bool trans = false
	);
	void MakeSquereVertex(void);

	// 座標設定
	void SetPos(const Vector2& pos);

	// 画像サイズ設定
	void SetSize(const Vector2& size);

	//シェーダー描画開始
	void DrawStart(void);
	
	//シェーダー描画終了
	void DrawFinish(void);

	/// <summary>
	/// 3D空間に適用する場合の関数
	/// </summary>
	/// <param name="doing">3D描画のラムダ式</param>
	/// <returns></returns>
	void DrawRenderer(std::function<void(void)> doing) noexcept;

	//2D画像に適用する場合の関数
	void Draw2D() noexcept;


private:


	// 座標
	Vector2 pos_;

	// 描画サイズ
	Vector2 size_;

	// 頂点
	std::vector<VERTEX2DSHADER> vertexs_;

	// 頂点インデックス
	std::vector<WORD> indexes_;

	// ピクセルシェーダー用情報
	std::weak_ptr<PixelMaterial> pixelMaterial_;

	// 頂点シェーダー用情報
	std::weak_ptr<VertexsMaterial> vertexsMaterial_;

	//使用するシェーダーの設定
	void SetUseShader(void);

	//使用したシェーダーの解除
	void ReleaseUseShader(void);

	//定数バッファ更新
	void ConstBufUpdate(int cbHandle, int shaderType, int slot);

};

