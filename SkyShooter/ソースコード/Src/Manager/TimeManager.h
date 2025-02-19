#pragma once

#include <chrono>
#include <string>
#include "../Utility/TimeUnit.h"
#include "../Singleton.h"

/// <summary>
/// �f���^�^�C�������Ԃ̊Ǘ�
/// </summary>
class TimeManager 
	: public Singleton<TimeManager>
{
public:

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �f���^�^�C���̃��Z�b�g
	/// </summary>
	void ResetDeltaTime(void);

	/// <summary>
	/// ���Ԃ̒�~
	/// </summary>
	void SetStop(bool isStop);

	/// <summary>
	/// ���Ԓ�~����
	/// </summary>
	const bool GetIsStop(void);

	/// <summary>
	/// ���Ԃ̎擾
	/// </summary>
	const float GetTime(void);

	/// <summary>
	/// �f���^�^�C���̎擾
	/// </summary>
	const float GetDeltaTime(void);

	/// <summary>
	/// ���݂̌v�����Ԃ𕪕b�~���b�Ŏ擾
	/// </summary>
	/// <returns>Time_Unit</returns>
	const Time_Unit GetTime_Unit(void);

	/// <summary>
	/// �w�肵�����l�𕪕b�~���b�ɕϊ�
	/// </summary>
	/// <returns>Time_Unit</returns>
	const Time_Unit GetTime_Unit(const float time);

private:

	friend class Singleton<TimeManager>;

	//1f�O�̎���
	std::chrono::system_clock::time_point preTime_;

	//����
	float time_;

	//�f���^�^�C��
	float deltaTime_;

	//���Ԓ�~����
	bool isStop_;

	TimeManager(void);
	virtual ~TimeManager(void) override;

	//�f���^�^�C���v��
	void MeasureDeltaTime(void);

};

