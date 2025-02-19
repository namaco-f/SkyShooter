#pragma once

#include <DxLib.h>
#include <vector>
#include <string>


/// <summary>
/// �V�F�[�_���\�[�X�̊��N���X
/// </summary>
class MaterialBase
{

public:

	MaterialBase(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="shaderFileName">Data/Shader/�ȍ~�̃t�@�C���p�X</param>
	/// <param name="cbFloat4Size">FLOAT4�^�̒萔�o�b�t�@�̐�</param>
	MaterialBase(
		const std::string& shaderFileName, const int cbFloat4Size);
	virtual ~MaterialBase(void);

	/// <summary>
	/// ���\�[�X�̉��
	/// </summary>
	virtual void Release(void);

	/// <summary>
	/// �V�F�[�_�̓Ǎ�
	/// </summary>
	/// <param name="str">�t�@�C�����w�肵�ēǍ�</param>
	virtual void LoadShader(const std::string& str) = 0;

	/// <summary>
	/// �V�F�[�_�̓Ǎ�
	/// </summary>
	virtual void LoadShader(void) = 0;

	/// <summary>
	/// �萔�o�b�t�@��ǉ�
	/// </summary>
	/// <param name="cbParam">FLOAT4�f�[�^�^</param>
	void AddConstBuf(const FLOAT4& cbParam);
	void AddConstBuf(const VECTOR& valA,const float& valB);

	/// <summary>
	/// �萔�o�b�t�@���X�V
	/// </summary>
	/// <param name="idx">�X�V����萔�o�b�t�@�̃C���f�b�N�X</param>
	/// <param name="cbParam">�X�V����萔�o�b�t�@��FLOAT4�f�[�^�^</param>
	void SetConstBuf(const int idx, const FLOAT4& cbParam);
	void SetConstBuf(const int idx, const VECTOR& valA, const float& valB);

	/// <summary>
	/// �V�F�[�_�n���h���̎擾
	/// </summary>
	/// <returns>�V�F�[�_�n���h���̎Q��</returns>
	inline const int& GetShader(void) const
	{
		return shaderHandle_;
	}

	/// <summary>
	/// �萔�o�b�t�@�n���h���̎擾
	/// </summary>
	/// <returns>�萔�o�b�t�@�n���h���̎Q��</returns>
	inline const int& GetConstBuf(void) const
	{
		return constBufHandle_;
	}

	/// <summary>
	/// �萔�o�b�t�@�z��̎擾
	/// </summary>
	/// <returns>�萔�o�b�t�@�z��̎Q��</returns>
	inline const std::vector<FLOAT4>& GetConstBufs(void) const
	{
		return constBufParams_;
	}


protected:

	//�V�F�[�_�[�n���h��
	int shaderHandle_;
	
	//�萔�o�b�t�@�n���h��
	int constBufHandle_;

	//�t�@�C���p�X
	std::string filePath_;
	
	//�萔�o�b�t�@��FLOAT4�^�z��̃T�C�Y
	int constBufFloat4Size_;
	
	// �萔�o�b�t�@�̒l
	std::vector<FLOAT4> constBufParams_;

	//�萔�o�b�t�@�n���h���̏�����
	void CreateConstBuf(void);

private:

};

