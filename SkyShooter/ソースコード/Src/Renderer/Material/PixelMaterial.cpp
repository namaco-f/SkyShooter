
#include "../../Application.h"
#include "PixelMaterial.h"

const int MAX_TEXTURES = 4;		// 設定できるテクスチャの最大数

PixelMaterial::PixelMaterial(const std::string& shaderFileName, const int cbFloat4Size)
	:
	MaterialBase((Application::PATH_SHADER + shaderFileName), cbFloat4Size),
	texAddress_(TEXADDRESS::CLAMP),
	textures_{}
{
}

PixelMaterial::~PixelMaterial(void)
{
	textures_.clear();
}

void PixelMaterial::Release(void)
{
	textures_.clear();
}

void PixelMaterial::LoadShader(const std::string& shaderFileName)
{	
	shaderHandle_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileName).c_str()
	);
}

void PixelMaterial::LoadShader(void)
{
	shaderHandle_ = LoadPixelShader(filePath_.c_str());
	
	CreateConstBuf();
}

void PixelMaterial::AddTextureBuf(int texDiffuse)
{
	//テクスチャの最大数ではないか確認
	if (MAX_TEXTURES > textures_.size())
	{
		textures_.emplace_back(texDiffuse);
	}
}

void PixelMaterial::SetTextureBuf(int idx, int texDiffuse)
{
	//配列の数以上ではないか確認
	if (idx >= textures_.size())
	{
		return;
	}

	textures_[idx] = texDiffuse;
}
