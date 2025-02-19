#pragma once
#include "MaterialBase.h"

/// <summary>
///�@�s�N�Z���V�F�[�_�̃��\�[�X
/// </summary>
class PixelMaterial 
	: public MaterialBase
{

public:

	/// <summary>
	/// �e�N�X�`���A�h���X�^�C�v
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
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="shaderFileName">Data/Shader/�ȍ~�̃p�X</param>
	/// <param name="constBufFloat4Size">FLOAT4�^�̒萔�o�b�t�@�̐�</param>
	PixelMaterial(const std::string& shaderFileName, const int cbFloat4Size);
	virtual ~PixelMaterial(void);

	/// <summary>
	/// ���\�[�X�̉��
	/// </summary>
	void Release(void) override;

	/// <summary>
	/// �t�@�C�����w�肵�ăV�F�[�_��Ǎ�
	/// </summary>
	/// <param name="str">Data/Shader/�ȍ~�̃p�X</param>
	void LoadShader(const std::string& str) override;

	/// <summary>
	/// �V�F�[�_�̓Ǎ�
	/// </summary>
	void LoadShader(void) override;

	/// <summary>
	/// �e�N�X�`���o�b�t�@��ǉ�
	/// </summary>
	/// <param name="texDiffuse">�e�N�X�`���̉摜�n���h��ID</param>
	void AddTextureBuf(int texDiffuse);

	/// <summary>
	/// �e�N�X�`�����X�V
	/// </summary>
	/// <param name="idx">�C���f�b�N�X</param>
	/// <param name="texDiffuse">�e�N�X�`���̉摜�n���h��ID</param>
	void SetTextureBuf(int idx, int texDiffuse);

	/// <summary>
	/// �e�N�X�`���̔z����擾
	/// </summary>
	/// <returns>textures_</returns>
	inline const std::vector<int>& GetTextures(void) const
	{
		return textures_;
	}

	/// <summary>
	/// �e�N�X�`���A�h���X���擾
	/// </summary>
	/// <returns>texAddress_</returns>
	inline TEXADDRESS GetTextureAddress(void) const
	{
		return texAddress_;
	}

	/// <summary>
	/// �e�N�X�`���A�h���X��ݒ�
	/// </summary>
	/// <param name="texA">TEXADDRESS</param>
	inline void SetTextureAddress(const TEXADDRESS texA)
	{
		texAddress_ = texA;
	}

private:

	// �e�N�X�`���A�h���X
	TEXADDRESS texAddress_;

	// �e�N�X�`��
	std::vector<int> textures_;

};



