#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

/// <summary>
/// ���f������̊�{���
/// �傫���FVECTOR�
/// ��]�@�FQuaternion�
/// �ʒu�@�FVECTOR�
/// </summary>


/// <summary>
/// ���f��������
/// </summary>
class Transform
{

public:

	/// <summary>
	/// ���f���̃n���h��ID
	/// </summary>
	int modelId_;

	/// <summary>
	/// �傫��
	/// </summary>
	VECTOR scl;
	
	/// <summary>
	/// ��](�f�O���[)
	/// </summary>
	VECTOR rot;
	
	/// <summary>
	/// �ʒu
	/// </summary>
	VECTOR pos;
	VECTOR localPos;

	/// <summary>
	/// �g��s��
	/// </summary>
	MATRIX matScl;

	/// <summary>
	/// ��]�s��
	/// </summary>
	MATRIX matRot;

	/// <summary>
	/// ���W�s��
	/// </summary>
	MATRIX matPos;

	/// <summary>
	/// ��](�N�H�[�^�j�I��)
	/// </summary>
	Quaternion quaRot;

	/// <summary>
	/// ���[�J����](�N�H�[�^�j�I��)
	/// </summary>
	Quaternion quaRotLocal;
	

	Transform(void);
	explicit Transform(const int model);
	~Transform(void);

	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	void Update(void);

	////���f���̌����`��
	//void DrawDirection(float len = DEBUG_DIR_LEN);

	/// <summary>
	/// ���f����ݒ�
	/// </summary>
	/// <param name="modelHId">�Ώۂ̃��f���̃n���h��ID</param>
	void SetModel(const int modelHId);

	/// <summary>
	/// �O���������擾
	/// </summary>
	VECTOR GetForward(void) const;

	/// <summary>
	/// ����������擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	VECTOR GetBack(void) const;

	/// <summary>
	/// �E�������擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	VECTOR GetRight(void) const;

	/// <summary>
	/// ���������擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	VECTOR GetLeft(void) const;

	/// <summary>
	/// ��������擾
	/// </summary>
	VECTOR GetUp(void) const;

	/// <summary>
	/// ���������擾
	/// </summary>
	VECTOR GetDown(void) const;

	/// <summary>
	/// �w�肵���������擾
	/// </summary>
	/// <param name="vec">�w�肷�����</param>s
	VECTOR GetDir(const VECTOR& vec) const;

	/// <summary>
	/// �w�肵���t���[���̍��W���擾
	/// </summary>
	/// <param name="frameNo">�w�肷��t���[��</param>
	/// <returns>�t���[���̍��W</returns>
	const VECTOR GetPosFrame(const int& frameNo) const;

private:

};
