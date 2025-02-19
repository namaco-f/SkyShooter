#pragma once

#include <DxLib.h>
#include <functional>
#include <vector>
#include <memory>
#include "../Common/Geometry2D.h"

class PixelMaterial;
class VertexsMaterial;

/// <summary>
/// �����_���[
/// </summary>
class Renderer
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pMaterial">�s�N�Z���V�F�[�_�[ : weak_ptr</param>
	/// <param name="vMaterial">���_�V�F�[�_�[ : weak_ptr</param>
	Renderer(std::weak_ptr<PixelMaterial> pMaterial , std::weak_ptr<VertexsMaterial> vMaterial);
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pMaterial">�s�N�Z���V�F�[�_�[ : weak_ptr</param>
	explicit Renderer(std::weak_ptr<PixelMaterial> pMaterial);

	//�f�X�g���N�^
	virtual ~Renderer(void);

	//�`���`�̐���
	void MakeSquereVertex(
		const Vector2& pos, const Vector2& size,
		size_t expectedNum = 0,bool trans = false
	);
	void MakeSquereVertex(void);

	// ���W�ݒ�
	void SetPos(const Vector2& pos);

	// �摜�T�C�Y�ݒ�
	void SetSize(const Vector2& size);

	//�V�F�[�_�[�`��J�n
	void DrawStart(void);
	
	//�V�F�[�_�[�`��I��
	void DrawFinish(void);

	/// <summary>
	/// 3D��ԂɓK�p����ꍇ�̊֐�
	/// </summary>
	/// <param name="doing">3D�`��̃����_��</param>
	/// <returns></returns>
	void DrawRenderer(std::function<void(void)> doing) noexcept;

	//2D�摜�ɓK�p����ꍇ�̊֐�
	void Draw2D() noexcept;


private:


	// ���W
	Vector2 pos_;

	// �`��T�C�Y
	Vector2 size_;

	// ���_
	std::vector<VERTEX2DSHADER> vertexs_;

	// ���_�C���f�b�N�X
	std::vector<WORD> indexes_;

	// �s�N�Z���V�F�[�_�[�p���
	std::weak_ptr<PixelMaterial> pixelMaterial_;

	// ���_�V�F�[�_�[�p���
	std::weak_ptr<VertexsMaterial> vertexsMaterial_;

	//�g�p����V�F�[�_�[�̐ݒ�
	void SetUseShader(void);

	//�g�p�����V�F�[�_�[�̉���
	void ReleaseUseShader(void);

	//�萔�o�b�t�@�X�V
	void ConstBufUpdate(int cbHandle, int shaderType, int slot);

};

