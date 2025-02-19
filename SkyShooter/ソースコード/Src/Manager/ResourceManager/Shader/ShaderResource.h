#pragma once

#include <memory>
#include <string>
#include <Dxlib.h>

class PixelMaterial;
class VertexsMaterial;
class Renderer;

/// <summary>
/// �e�V�F�[�_�[�⃌���_���[�̊Ǘ�
/// </summary>
class ShaderResource
{
public:

	ShaderResource(void) = default;
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="psFileName">�s�N�Z���V�F�[�_�[�̃t�@�C����</param>
	/// <param name="psCbSizeFloat4">�s�N�Z���V�F�[�_�[�萔�o�b�t�@�p : FLOAT4�̍ő吔</param>
	/// <param name="vsFileName">���_�V�F�[�_�[�̃t�@�C����</param>
	/// <param name="vsCbSizeFloat4">���_�V�F�[�_�[�萔�o�b�t�@�p : FLOAT4�̍ő吔</param>
	ShaderResource(
		const std::string& psFileName, 
		const int& psCbSizeFloat4,
		const std::string& vsFileName,
		const int& vsCbSizeFloat4
	);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="psFileName">�s�N�Z���V�F�[�_�[�̃t�@�C����</param>
	/// <param name="psCbSizeFloat4">�s�N�Z���V�F�[�_�[�萔�o�b�t�@�p : FLOAT4�̍ő吔</param>
	ShaderResource(
		const std::string& psFileName,
		const int& psCbSizeFloat4);

	virtual ~ShaderResource(void);

	/// <summary>
	/// �������
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// �V�F�[�_�[�̓Ǎ�����
	/// </summary>
	void Load(void);

	/// <summary>
	/// ���_�V�F�[�_�[���g�p���Ă��邩
	/// </summary>
	const bool IsUsedVS(void) const;

	/// <summary>
	/// ���_�V�F�[�_�[�̒萔�o�b�t�@�ɏ���
	/// </summary>
	/// <param name="val">FLOAT4�^</param>
	void AddConstBuf_VS(const FLOAT4& val);

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̒萔�o�b�t�@�ɏ���
	/// </summary>
	/// <param name="val">FLOAT4�^</param>
	void AddConstBuf_PS(const FLOAT4& val);
	
	/// <summary>
	/// �s�N�Z���V�F�[�_�[�N���X�̎擾
	/// </summary>
	/// <returns>weak_ptr</returns>
	std::weak_ptr<PixelMaterial> GetPS(void) const;

	/// <summary>
	/// ���_�V�F�[�_�[�N���X�̎擾
	/// </summary>
	/// <returns>weak_ptr</returns>
	std::weak_ptr<VertexsMaterial> GetVS(void) const;

	/// <summary>
	/// �����_���[�N���X�̎擾
	/// </summary>
	/// <returns>weak_ptr</returns>
	std::weak_ptr<Renderer> GetRenderer(void);

	/// <summary>
	/// �����_���[�쐬
	/// </summary>
	void MakeRenderer(void);

private:

	//�s�N�Z���V�F�[�_�[
	std::shared_ptr<PixelMaterial> psMaterial_;

	//���_�V�F�[�_�[
	std::shared_ptr<VertexsMaterial> vsMaterial_;

	//�����_���[
	std::shared_ptr<Renderer> renderer_;
};

