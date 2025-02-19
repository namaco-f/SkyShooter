#pragma once

#include <unordered_map>
#include "ShaderResource.h"
#include "../ResourcesManagerTemplate.h"

/// <summary>
/// �V�F�[�_�[��
/// </summary>
enum class SRC_SHADER
{
	UV_SCROLL,			//UV�X�N���[��
	STANDARD_MODEL,		//���f���̃f�t�H���g�V�F�[�_
	//MRT_TEST,			//�}���`�����_�[�^�[�Q�b�g�e�X�g�p
	BOX_SHADER,			//���U���̃V�F�[�_
	//TRAIL_TEST,			//�O�՗p
};

/// <summary>
/// �V�F�[�_�̊Ǘ�
/// </summary>
class ShaderManager
	: 
	public ResourcesManagerTemplate<
	ShaderManager,
	ShaderResource,
	SRC_SHADER
	>
{
public:

	ShaderManager(void);
	virtual ~ShaderManager(void) override;

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void);

	/// <summary>
	/// �������
	/// </summary>
	void Release(void);

private:

	//�Ǎ�
	ShaderResource& _Load(const SRC_SHADER src) override;

	/// <summary>
	/// ���\�[�X�}�b�v�ɒǉ�
	/// </summary>
	/// <param name="src">�V�F�[�_�[��</param>
	/// <param name="psPath">�s�N�Z���V�F�[�_�[�̃p�X</param>
	/// <param name="psCbSizeFloat4">�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@ : FLOAT4�̐�</param>
	void AddResourceMap(const SRC_SHADER src,const std::string& psPath , const int& psCbSizeFloat4);
	
	/// <summary>
	/// ���\�[�X�}�b�v�ɒǉ�
	/// </summary>
	/// <param name="src">�V�F�[�_�[��</param>
	/// <param name="psPath">�s�N�Z���V�F�[�_�[�̃p�X</param>
	/// <param name="psCbSizeFloat4">�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@ : FLOAT4�̐�</param>
	/// <param name="vsPath">���_�V�F�[�_�[�̃p�X</param>
	/// <param name="vsCbSizeFloat4">���_�V�F�[�_�[�̒萔�o�b�t�@ : FLOAT4�̐�</param>
	void AddResourceMap(
		const SRC_SHADER src,
		const std::string& psPath, const int& psCbSizeFloat4,
		const std::string& vsPath, const int& vsCbSizeFloat4);

};

