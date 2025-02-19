#pragma once
#include "MaterialBase.h"

/// <summary>
/// ���_�V�F�[�_�̃��\�[�X
/// </summary>
class VertexsMaterial 
	: public MaterialBase
{
public:

	/// <summary>
	///�R���X�g���N�^
	/// </summary>
	/// <param name="shaderFilePath">Data/Shader/�ȍ~�̃p�X</param>
	/// <param name="_cbSize">FLOAT4�^�̒萔�o�b�t�@�̐�</param>
	VertexsMaterial(
		const std::string& shaderFileName, 
		const int cbSize
	);
	virtual ~VertexsMaterial(void);

	/// <summary>
	/// �t�@�C�����w�肵�ăV�F�[�_��Ǎ�
	/// </summary>
	/// <param name="shaderFileName"></param>
	void LoadShader(const std::string& shaderFileName) override;

	/// <summary>
	/// �V�F�[�_�̓Ǎ�
	/// </summary>
	void LoadShader(void) override;

private:

};

