
#include "../Manager/UIManager/UIDataManager.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "../Observer/Observer.h"
#include "SoundController.h"

const float SOUND_VOL_MIN = 0.0f;		//�Œቹ��
const float SOUND_VOL_MAX = 255.0f;		//�ō�����
const int SOUND_VOL_DEF = 125;			//�f�t�H���g����

SoundController::SoundController(void)
	:
	observer_(Observer::GetInstance()),
	soundsMap_{}
{
}

SoundController::~SoundController(void)
{
	for (const auto& sM : soundsMap_)
	{
		StopSoundMem(sM.second.handle);
		DeleteSoundMem(sM.second.handle);
	}
	soundsMap_.clear();
}

void SoundController::Update(void)
{
	for (const auto& message : observer_.ReceiveMessageList())
	{
		if (message != Message::SoundVolumeChange)
		{
			continue;
		}

		//���ʍX�V
		UpdateVolume();
		return;
	}
}

void SoundController::Add(const std::string& soundName, const SoundInfo& soundInfo)
{
	SoundInfo info = soundInfo;

	soundsMap_.emplace(soundName, info);

	if (info.soundType == SoundType::BGM)
	{
		SetSoundVolume(soundName, GetUIDataToVariant<float>(SRC_UIData::BGM_VOLUME));
	}
	else
	{
		SetSoundVolume(soundName, GetUIDataToVariant<float>(SRC_UIData::SE_VOLUME));
	}
}

void SoundController::SetFrequency(
	const std::string& soundName, int soundFreqMin, int soundFreqMax, int soundFreqSpeed)
{
	if (!soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	auto& sM = soundsMap_.at(soundName);

	sM.ableSoundFreq = true;
	sM.soundFreq.soundFreqMin = soundFreqMin;
	sM.soundFreq.soundFreqMax = soundFreqMax;
	sM.soundFreq.soundFreqSpeed = soundFreqSpeed;
	sM.soundFreq.soundFreqCnt = 0;
}

void SoundController::SetFrequency(const std::string& soundName, const SoundFreq& soundFrequency)
{
	if (!soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	auto& sM = soundsMap_.at(soundName);

	sM.ableSoundFreq = true;
	sM.soundFreq.soundFreqMin = soundFrequency.soundFreqMin;
	sM.soundFreq.soundFreqMax = soundFrequency.soundFreqMax;
	sM.soundFreq.soundFreqSpeed = soundFrequency.soundFreqSpeed;
	sM.soundFreq.soundFreqCnt = soundFrequency.soundFreqCnt;
}

void SoundController::Play(const std::string& soundName, float volPar)
{
	if (!soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	if (volPar != 0.0f)
	{
		//�Đ����ɉ��ʂ��ݒ肳��Ă���
		SetSoundVolume(soundName, volPar);
	}

	const auto& sM = soundsMap_.at(soundName);

	PlaySoundMem(sM.handle, sM.playType);
}

void SoundController::Stop(const std::string& soundName) const
{
	if (!soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	const auto& sM = soundsMap_.at(soundName);

	StopSoundMem(sM.handle);
}

bool SoundController::IsPlay(const std::string& soundName) const
{
	if (!soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return false;
	}

	const auto& sM = soundsMap_.at(soundName);

	return CheckSoundMem(sM.handle);
}

const float SoundController::GetSEVolumePar(void)
{
	return GetUIDataToVariant<float>(SRC_UIData::SE_VOLUME);
}

const float SoundController::GetBGMVolumePar(void)
{
	return GetUIDataToVariant<float>(SRC_UIData::BGM_VOLUME);
}

void SoundController::FrequencySound(const std::string& soundName)
{
	if (!soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	auto& sM = soundsMap_.at(soundName);

	if (!sM.ableSoundFreq) 
	{
		//���g���ω����g�p���Ȃ�
		return; 
	}

	sM.soundFreq.soundFreqCnt += sM.soundFreq.soundFreqSpeed;

	if (sM.soundFreq.soundFreqCnt > sM.soundFreq.soundFreqMax)
	{
		sM.soundFreq.soundFreqCnt = sM.soundFreq.soundFreqMax;
	}

	if (sM.soundFreq.soundFreqCnt < sM.soundFreq.soundFreqMin)
	{
		sM.soundFreq.soundFreqCnt = sM.soundFreq.soundFreqMin;
	}

	SetFrequencySoundMem(sM.soundFreq.soundFreqCnt, sM.handle);
}

void SoundController::SetFrequencySpeed(const std::string& soundName, int speed)
{
	if (soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	auto& sM = soundsMap_.at(soundName);

	sM.soundFreq.soundFreqSpeed = speed;
}

void SoundController::SetSoundVolume(const std::string& soundName, float volPar)
{
	if (!soundsMap_.contains(soundName))
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	auto& sM = soundsMap_.at(soundName);

	int vol = static_cast<int>(SOUND_VOL_MAX * volPar);
	if (sM.vol == vol)
	{
		return;
	}

	sM.vol = vol;

	ChangeVolumeSoundMem(sM.vol, sM.handle);
}

void SoundController::UpdateVolume(void)
{
	if (soundsMap_.empty())
	{
		//�����o�^����Ă��Ȃ�
		return;
	}

	//SE���ʎ擾
	float volSE = GetSEVolumePar();

	//BGM���ʎ擾
	float volBGM = GetBGMVolumePar();

	for (auto& soundInfo : soundsMap_)
	{
		if (soundInfo.second.soundType == SoundType::SE)
		{
			//SE���ʍX�V
			SetSoundVolume(soundInfo.first, volSE);
			continue;
		}

		if (soundInfo.second.soundType == SoundType::BGM)
		{
			//BGM���ʍX�V
			SetSoundVolume(soundInfo.first, volBGM);
		}
	}
}
