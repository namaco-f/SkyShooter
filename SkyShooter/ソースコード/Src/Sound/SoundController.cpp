
#include "../Manager/UIManager/UIDataManager.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "../Observer/Observer.h"
#include "SoundController.h"

const float SOUND_VOL_MIN = 0.0f;		//Å’á‰¹—Ê
const float SOUND_VOL_MAX = 255.0f;		//Å‚‰¹—Ê
const int SOUND_VOL_DEF = 125;			//ƒfƒtƒHƒ‹ƒg‰¹—Ê

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

		//‰¹—ÊXV
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
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
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
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
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
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
		return;
	}

	if (volPar != 0.0f)
	{
		//Ä¶‚É‰¹—Ê‚ªİ’è‚³‚ê‚Ä‚¢‚é
		SetSoundVolume(soundName, volPar);
	}

	const auto& sM = soundsMap_.at(soundName);

	PlaySoundMem(sM.handle, sM.playType);
}

void SoundController::Stop(const std::string& soundName) const
{
	if (!soundsMap_.contains(soundName))
	{
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
		return;
	}

	const auto& sM = soundsMap_.at(soundName);

	StopSoundMem(sM.handle);
}

bool SoundController::IsPlay(const std::string& soundName) const
{
	if (!soundsMap_.contains(soundName))
	{
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
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
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
		return;
	}

	auto& sM = soundsMap_.at(soundName);

	if (!sM.ableSoundFreq) 
	{
		//ü”g”•Ï‰»‚ğg—p‚µ‚È‚¢
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
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
		return;
	}

	auto& sM = soundsMap_.at(soundName);

	sM.soundFreq.soundFreqSpeed = speed;
}

void SoundController::SetSoundVolume(const std::string& soundName, float volPar)
{
	if (!soundsMap_.contains(soundName))
	{
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
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
		//‰¹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
		return;
	}

	//SE‰¹—Êæ“¾
	float volSE = GetSEVolumePar();

	//BGM‰¹—Êæ“¾
	float volBGM = GetBGMVolumePar();

	for (auto& soundInfo : soundsMap_)
	{
		if (soundInfo.second.soundType == SoundType::SE)
		{
			//SE‰¹—ÊXV
			SetSoundVolume(soundInfo.first, volSE);
			continue;
		}

		if (soundInfo.second.soundType == SoundType::BGM)
		{
			//BGM‰¹—ÊXV
			SetSoundVolume(soundInfo.first, volBGM);
		}
	}
}
