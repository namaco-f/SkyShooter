
#include "../../Application.h"
#include "PixelMaterial.h"

const int MAX_TEXTURES = 4;		// �ݒ�ł���e�N�X�`���̍ő吔

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
	//�e�N�X�`���̍ő吔�ł͂Ȃ����m�F
	if (MAX_TEXTURES > textures_.size())
	{
		textures_.emplace_back(texDiffuse);
	}
}

void PixelMaterial::SetTextureBuf(int idx, int texDiffuse)
{
	//�z��̐��ȏ�ł͂Ȃ����m�F
	if (idx >= textures_.size())
	{
		return;
	}

	textures_[idx] = texDiffuse;
}
