#pragma once

#include <Dxlib.h>
#include <memory>
#include <string>
#include <unordered_map>

class Observer;

/// <summary>
/// ���̎��
/// </summary>
enum class SoundType
{
	BGM,	//BGM
	SE,		//SE
};

/// <summary>
/// ���g���֘A
/// </summary>
struct SoundFreq
{
	int soundFreqCnt;			//���g���ω��J�E���g
	int soundFreqMin;			//�Œ���g��
	int soundFreqMax;			//�ō����g��
	int soundFreqSpeed;			//���g���ω��X�s�[�h

	SoundFreq(void)
		:
		soundFreqCnt(0),
		soundFreqMin(5000),
		soundFreqMax(48000),
		soundFreqSpeed(2000)
	{};

	SoundFreq(const int min, const int max, const int speed)
		:
		soundFreqCnt(0),
		soundFreqMin(min),
		soundFreqMax(max),
		soundFreqSpeed(speed)
	{};
};

/// <summary>
/// �����
/// </summary>
struct SoundInfo
{
	int handle;				//�T�E���h�n���h��ID
	int vol;				//����
	int playType;			//DX�Đ��^�C�v
	SoundType soundType;
	bool ableSoundFreq;		//���g���ω����g�p���邩
	SoundFreq soundFreq;	//���g���֘A

	SoundInfo(void) 
		:
		handle(-1),
		vol(0),
		playType(DX_PLAYTYPE_LOOP),
		soundType(SoundType::SE),
		ableSoundFreq(false),
		soundFreq(SoundFreq())
	{};

	SoundInfo(const int handle,const SoundType sT, const int pT = DX_PLAYTYPE_LOOP)
		:
		handle(handle),
		vol(0),
		playType(pT),
		soundType(sT),
		ableSoundFreq(false),
		soundFreq(SoundFreq())
	{};
};

/// <summary>
/// ���̑���
/// </summary>
class SoundController
{
public:

	SoundController(void);
	virtual ~SoundController(void);
	
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// ���̓o�^
	/// </summary>
	/// <param name="soundName">���̖��O</param>
	/// <param name="soundInfo">�����</param>
	void Add(const std::string& soundName, const SoundInfo& soundInfo);

	/// <summary>
	/// ���̎��g���̐ݒ�
	/// </summary>
	/// <param name="soundName">�T�E���h���</param>
	/// <param name="soundFreqMin">���g������</param>
	/// <param name="soundFreqMax">���g�����</param>
	/// <param name="soundFreqSpeed">���g���ω����x</param>
	void SetFrequency(
		const std::string& soundName, int soundFreqMin, int soundFreqMax, int soundFreqSpeed);

	/// <summary>
	/// ���̎��g���̐ݒ�
	/// </summary>
	/// <param name="soundName">�T�E���h���</param>
	/// <param name="soundFrequency">���g���֘A</param>
	void SetFrequency(
		const std::string& soundName, const SoundFreq& soundFrequency);

	/// <summary>
	/// �w��̉��̍Đ�
	/// </summary>
	/// <param name="soundName">���̖��O</param>
	/// <param name="volPar">����:����(0.0f~1.0f)</param>
	void Play(const std::string& soundName, float volPar = 0.0f);

	/// <summary>
	/// �w��̉��̒�~
	/// </summary>
	void Stop(const std::string& soundName) const;

	/// <summary>
	/// �w�肵�����̎��g���ω�
	/// </summary>
	void FrequencySound(const std::string& soundName);

	/// <summary>
	/// �w�肵�����̎��g���ω����x��ݒ�
	/// </summary>
	void SetFrequencySpeed(const std::string& soundName, int speed);

	/// <summary>
	/// �w�肵�����̉��ʂ�ݒ�
	/// </summary>
	/// <param name="soundName">���̖��O</param>
	/// <param name="volPar">����:����(0.0f~1.0f)</param>
	void SetSoundVolume(const std::string& soundName, float volPar);

	/// <summary>
	/// �w�肵�������Đ�����
	/// </summary>
	/// <returns>
	/// true : �Đ���
	/// false : �Đ����Ă��Ȃ��A�o�^����Ă��Ȃ�
	/// </returns>
	bool IsPlay(const std::string& soundName) const;


private:

	Observer& observer_;

	/// <summary>
	/// ���̏��
	/// </summary>
	std::unordered_map<std::string, SoundInfo> soundsMap_;

	/// <summary>
	/// ���ʂ��X�V
	/// </summary>
	void UpdateVolume(void);

	/// <summary>
	/// UIData����SE���ʂ��擾
	/// </summary>
	const float GetSEVolumePar(void);

	/// <summary>
	/// UIData����BGM���ʂ��擾
	/// </summary>
	const float GetBGMVolumePar(void);
};


