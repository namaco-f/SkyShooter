#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>

/// <summary>
/// �N�H�[�^�j�I��
/// </summary>
class Quaternion
{
public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	Quaternion(void);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="rad">���W�A���p</param>
	explicit Quaternion(const VECTOR& rad);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="wxyz">�N�H�[�^�j�I��</param>
	Quaternion(double w, double x, double y, double z);

	virtual ~Quaternion(void);

	/// <summary>
	/// �I�C���[�p����N�H�[�^�j�I���֕ϊ�
	/// </summary>
	static Quaternion Euler(const VECTOR& rad);
	
	/// <summary>
	/// �I�C���[�p����N�H�[�^�j�I���֕ϊ�
	/// </summary>
	static Quaternion Euler(double radX, double radY, double radZ);

	/// <summary>
	/// �N�H�[�^�j�I���̍���
	/// </summary>
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);

	/// <summary>
	/// �N�H�[�^�j�I���̍���
	/// </summary>
	Quaternion Mult(const Quaternion& q) const;

	/// <summary>
	/// �w�莲���w��p���A��]������
	/// </summary>
	static Quaternion AngleAxis(double rad, VECTOR axis);

	/// <summary>
	/// ���W����]������
	/// </summary>
	static VECTOR PosAxis(const Quaternion& q, VECTOR pos);

	/// <summary>
	/// ���W����]������
	/// </summary>
	VECTOR PosAxis(VECTOR pos) const;

	/// <summary>
	/// �N�H�[�^�j�I������I�C���[�p�֕ϊ�
	/// </summary>
	static VECTOR ToEuler(const Quaternion& q);

	/// <summary>
	/// �N�H�[�^�j�I������I�C���[�p�֕ϊ�
	/// </summary>
	VECTOR ToEuler(void) const;

	/// <summary>
	/// �N�H�[�^�j�I������s��֕ϊ�
	/// </summary>
	static MATRIX ToMatrix(const Quaternion& q);

	/// <summary>
	/// �N�H�[�^�j�I������s��֕ϊ�
	/// </summary>
	MATRIX ToMatrix(void) const;

	/// <summary>
	/// �x�N�g������N�H�[�^�j�I���ɕϊ�
	/// </summary>
	static Quaternion LookRotation(VECTOR dir);

	/// <summary>
	/// �x�N�g������N�H�[�^�j�I���ɕϊ�
	/// </summary>
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	/// <summary>
	/// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	/// </summary>
	static Quaternion GetRotation(MATRIX mat);

	/// <summary>
	/// �O���x�N�g�����擾
	/// </summary>
	VECTOR GetForward(void) const;

	/// <summary>
	/// ����x�N�g�����擾
	/// </summary>
	VECTOR GetBack(void) const;

	/// <summary>
	/// �E���x�N�g�����擾
	/// </summary>
	VECTOR GetRight(void) const;

	/// <summary>
	/// �����x�N�g�����擾
	/// </summary>
	VECTOR GetLeft(void) const;

	/// <summary>
	/// ����x�N�g�����擾
	/// </summary>
	VECTOR GetUp(void) const;

	/// <summary>
	/// �����x�N�g�����擾
	/// </summary>
	VECTOR GetDown(void) const;

	/// <summary>
	/// ����
	/// </summary>
	static double Dot(const Quaternion& q1, const Quaternion& q2);

	/// <summary>
	/// ����
	/// </summary>
	double Dot(const Quaternion& b) const;

	/// <summary>
	/// ���K��
	/// </summary>
	static Quaternion Normalize(const Quaternion& q);

	/// <summary>
	/// ���K��
	/// </summary>
	Quaternion Normalized(void) const;

	/// <summary>
	/// ���K��
	/// </summary>
	void Normalize(void);

	/// <summary>
	/// �t�N�H�[�^�j�I��
	/// </summary>
	Quaternion Inverse(void) const;

	/// <summary>
	/// ���ʕ��
	/// </summary>
	static Quaternion Slerp(const Quaternion from, const Quaternion to, double t);

	/// <summary>
	/// ���ʕ��
	/// </summary>
	static Quaternion Slerp(const Quaternion from, const Quaternion to, float t);

	/// <summary>
	/// �Q�̃x�N�g���Ԃ̉�]�ʂ��擾����
	/// </summary>
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity(void);

	double Length(void) const;
	double LengthSquared(void) const;
	VECTOR xyz(void) const;

	/// <summary>
	/// �Ώە����̉�]
	/// </summary>
	void ToAngleAxis(float* angle, VECTOR* axis);

private:

	//��{�x�N�g�����擾
	VECTOR GetDir(VECTOR dir) const;

	const Quaternion operator*(const float& rhs);
	const Quaternion operator+(const Quaternion& rhs);
};
