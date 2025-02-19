
#include "../Renderer/Material/PixelMaterial.h"
#include "../Renderer/Material/VertexsMaterial.h"
#include "Renderer.h"

// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
const int CONSTANT_BUF_SLOT_BEGIN_PS = 4;
const int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

//頂点数
const int NUM_VERTEX = 4;

//頂点インデックス数
const int NUM_VERTEX_IDX = 6;

//ポリゴン数
const int NUM_POLYGON = 2;

Renderer::Renderer(
	std::weak_ptr<PixelMaterial> pMaterial, std::weak_ptr<VertexsMaterial> vMaterial)
{
	pixelMaterial_ = pMaterial;
	
	vertexsMaterial_ = vMaterial;
}

Renderer::Renderer(std::weak_ptr<PixelMaterial> pMaterial)
{
	pixelMaterial_ = pMaterial;
}

Renderer::~Renderer(void)
{ 
	vertexs_.clear();
	indexes_.clear();
}

void Renderer::MakeSquereVertex(const Vector2& pos, const Vector2& size, size_t expectedNum, bool trans)
{
	pos_ = pos;
	size_ = size;

	vertexs_.clear();
	vertexs_.reserve(expectedNum * NUM_VERTEX);
	vertexs_.resize(vertexs_.size() + 4);
	
	indexes_.clear();
	indexes_.reserve(expectedNum * NUM_VERTEX_IDX);
	indexes_.resize(indexes_.size() + 6);

	int stVertex = (int)vertexs_.size();
	int stIndices_ = (int)indexes_.size();

	for (int i = 0; i < (int)vertexs_.size(); i++)
	{
		auto& v = vertexs_[i];
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(255, 255, 255, 255);
		v.rhw = 1.0f;
	}

	//4頂点の設定(Z字になるように)
	int cnt = 0;
	vertexs_[cnt].pos = VGet((float)pos_.x, (float)pos_.y, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	vertexs_[cnt].pos = VGet((float)(pos_.x + size_.x), (float)pos_.y, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	vertexs_[cnt].pos = VGet((float)pos_.x, (float)(pos_.y + size_.y), 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	vertexs_[cnt].pos = VGet((float)(pos_.x + size_.x), (float)(pos_.y + size_.y), 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;


	//0,1,2 1,3,2
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;
	indexes_[cnt++] = 2;
}

void Renderer::MakeSquereVertex(void)
{
	//MakeSquereVertex(pos, size_);
}

void Renderer::SetPos(const Vector2& pos)
{
	pos_ = pos;
}

void Renderer::SetSize(const Vector2& size)
{
	size_ = size;
}

//描画開始
void Renderer::DrawStart(void)
{
	MV1SetUseOrigShader(true);

	SetUseShader();
}

//描画終了
void Renderer::DrawFinish(void)
{	
	MV1SetUseOrigShader(false);

	ReleaseUseShader();
}


//3D空間に適用する場合の関数
void Renderer::DrawRenderer(std::function<void(void)> doing) noexcept
{
	if (GetUseDirect3DVersion() != DX_DIRECT3D_11)
	{
		doing();
		return;
	}

	size_t size;

	SetUseShader();
	MV1SetUseOrigShader(true);



#pragma region 頂点シェーダー
	if (!vertexsMaterial_.expired())
	{
		ConstBufUpdate(
			vertexsMaterial_.lock()->GetConstBuf(),
			DX_SHADERTYPE_VERTEX,
			CONSTANT_BUF_SLOT_BEGIN_VS
		);
	}



#pragma endregion


#pragma region ピクセルシェーダー
	
	const auto& textures = pixelMaterial_.lock()->GetTextures();
	size = textures.size();
	for (int n = 0; n < (int)size; n++)
	{
		//テクスチャのセット
		SetUseTextureToShader(n, textures[n]);
	}

	ConstBufUpdate(
		pixelMaterial_.lock()->GetConstBuf(),
		DX_SHADERTYPE_PIXEL, 
		CONSTANT_BUF_SLOT_BEGIN_PS
	);

#pragma endregion


	auto texA = pixelMaterial_.lock()->GetTextureAddress();
	int texAType = static_cast<int>(texA);

	//テクスチャアドレスタイプの変更
	SetTextureAddressModeUV(texAType, texAType);

	doing();

	//テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	MV1SetUseOrigShader(false);

	ReleaseUseShader();

	
	size = textures.size();
	for (int n = 0; n < (int)size; n++)
	{
		//テクスチャの解除
		SetUseTextureToShader(n, -1);
	}
}

//2D画像に適用する場合の関数
void Renderer::Draw2D() noexcept
{
	if (GetUseDirect3DVersion() != DX_DIRECT3D_11) { return; }


	DrawRenderer(
		[&] {
			DrawPolygonIndexed2DToShader(
				vertexs_.data(), (int)vertexs_.size(),
				indexes_.data(), (int)indexes_.size());
		}
	);

}

//シェーダーの設定
void Renderer::SetUseShader(void)
{
	if (!vertexsMaterial_.expired())
	{
		//頂点シェーダーの設定
		SetUseVertexShader(vertexsMaterial_.lock()->GetShader());
	}


	//ピクセルシェーダーの設定
	SetUsePixelShader(pixelMaterial_.lock()->GetShader());
}

//シェーダーの解除
void Renderer::ReleaseUseShader(void)
{
	//頂点シェーダーの解除
	SetUseVertexShader(-1);

	//ピクセルシェーダーの解除
	SetUsePixelShader(-1);
}

void Renderer::ConstBufUpdate(int cbHandle, int shaderType, int slot)
{
	if (shaderType == DX_SHADERTYPE_VERTEX)
	{
		if (!vertexsMaterial_.expired())
		{
			FLOAT4* cBufferPtr = (FLOAT4*)GetBufferShaderConstantBuffer(cbHandle);
			const auto& cBuffers = vertexsMaterial_.lock()->GetConstBufs();

			auto size = (int)cBuffers.size();
			for (int n = 0; n < size; n++)
			{
				if (n != 0)
				{
					cBufferPtr++;
				}
				cBufferPtr->x = cBuffers[n].x;
				cBufferPtr->y = cBuffers[n].y;
				cBufferPtr->z = cBuffers[n].z;
				cBufferPtr->w = cBuffers[n].w;
			}

			UpdateShaderConstantBuffer(cbHandle);

			//頂点シェーダー用の定数バッファーにセット
			SetShaderConstantBuffer(
				cbHandle, shaderType, slot);
			return;
		}
	}

	if (shaderType == DX_SHADERTYPE_PIXEL)
	{
		FLOAT4* cBufferPtr = (FLOAT4*)GetBufferShaderConstantBuffer(cbHandle);
		const auto& cBuffers = pixelMaterial_.lock()->GetConstBufs();

		auto size = (int)cBuffers.size();
		for (int n = 0; n < size; n++)
		{
			if (n != 0)
			{
				cBufferPtr++;
			}
			cBufferPtr->x = cBuffers[n].x;
			cBufferPtr->y = cBuffers[n].y;
			cBufferPtr->z = cBuffers[n].z;
			cBufferPtr->w = cBuffers[n].w;
		}

		UpdateShaderConstantBuffer(cbHandle);

		//ピクセルシェーダー用の定数バッファーにセット
		SetShaderConstantBuffer(
			cbHandle, shaderType, slot);
		return;
	}

}
