#pragma once


#include <random>
#include <vector>
#include "../Singleton.h"

/// <summary>
/// �����n�N���X
/// </summary>
class RandManager
	: public Singleton<RandManager>
{
public:

	/// <summary>
	/// �^���������g�p���Aint�^�̗����𐶐�
	/// </summary>
	/// <param name="min">�Œ�l</param>
	/// <param name="max">�ō��l</param>
	/// <returns>int�^�̐��l</returns>
	const int GetRand_mt(int min, int max);

	/// <summary>
	/// �^���������g�p���Afloat�^�̗����𐶐�
	/// </summary>
	/// <param name="min">�Œ�l</param>
	/// <param name="max">�ō��l</param>
	/// <returns>float�^�̐��l</returns>
	const float GetRand_mt(float min,float max);

private:

	friend class Singleton<RandManager>;

	RandManager(void);
	virtual ~RandManager(void) override;

	//�����_���V�[�h�𐶐�
	void GenerateRand(void);

	//�����_���V�[�h�i�[�p
	std::random_device::result_type randomDeviceResult_;

};
