#pragma once
#include <vector>
#include "Transform.h"

/// <summary>
/// �ۉe
/// </summary>
class CircleShadow
{
public:

	CircleShadow(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="trans">�Ǐ]���郂�f���̐�����</param>
	/// <param name="polySize">�e�̑傫��</param>
	/// <param name="offSetY">���f���̍��W����̃I�t�Z�b�g�l</param>
	CircleShadow(const Transform& trans,const VECTOR& polySize,const float offSetY = 0.0f);
	virtual ~CircleShadow(void);

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init(void);

	/// <summary>
	/// �Ǐ]������W���X�V
	/// </summary>
	/// <param name="trans">���W���</param>
	virtual void Update(const Transform& trans);

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void);

private:

	//�ۉe�̃n���h��ID
	int circleShadowHandle_;

	//�ۉe�𔽉f����Ώۂ̃��[���h���W
	VECTOR transPos_;

	//��]
	Quaternion quaRot_;

	//�|���S���̃T�C�Y
	VECTOR polySize_;

	//�|���S���̃T�C�Y�̔���
	VECTOR polySizeHalf_;

	//�e���W��Y���W�I�t�Z�b�g�l
	float offSetY_;

	//�|���S���z��
	std::vector<VERTEX3D> vertices_;

	//�C���f�b�N�X�z��
	std::vector<unsigned short> index_;

	const VECTOR GetPolyPos(const VECTOR& size);

	//��`�|���S���𐶐�
	void MakeVertices(void);

	//��`�|���S���̍X�V
	void UpdateVertices(void);

	//y����]�݂̂��擾
	const Quaternion GetQuaRotY(const Quaternion& qua);

	//��]���f��̒��_�ʒu
	const VECTOR GetRotPos(const VECTOR size);

};

