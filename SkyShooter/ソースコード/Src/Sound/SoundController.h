#pragma once

#include <Dxlib.h>
#include <memory>
#include <string>
#include <unordered_map>

class Observer;

/// <summary>
/// 音の種別
/// </summary>
enum class SoundType
{
	BGM,	//BGM
	SE,		//SE
};

/// <summary>
/// 周波数関連
/// </summary>
struct SoundFreq
{
	int soundFreqCnt;			//周波数変化カウント
	int soundFreqMin;			//最低周波数
	int soundFreqMax;			//最高周波数
	int soundFreqSpeed;			//周波数変化スピード

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
/// 音情報
/// </summary>
struct SoundInfo
{
	int handle;				//サウンドハンドルID
	int vol;				//音量
	int playType;			//DX再生タイプ
	SoundType soundType;
	bool ableSoundFreq;		//周波数変化を使用するか
	SoundFreq soundFreq;	//周波数関連

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
/// 音の操作
/// </summary>
class SoundController
{
public:

	SoundController(void);
	virtual ~SoundController(void);
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 音の登録
	/// </summary>
	/// <param name="soundName">音の名前</param>
	/// <param name="soundInfo">音情報</param>
	void Add(const std::string& soundName, const SoundInfo& soundInfo);

	/// <summary>
	/// 音の周波数の設定
	/// </summary>
	/// <param name="soundName">サウンド種類</param>
	/// <param name="soundFreqMin">周波数下限</param>
	/// <param name="soundFreqMax">周波数上限</param>
	/// <param name="soundFreqSpeed">周波数変化速度</param>
	void SetFrequency(
		const std::string& soundName, int soundFreqMin, int soundFreqMax, int soundFreqSpeed);

	/// <summary>
	/// 音の周波数の設定
	/// </summary>
	/// <param name="soundName">サウンド種類</param>
	/// <param name="soundFrequency">周波数関連</param>
	void SetFrequency(
		const std::string& soundName, const SoundFreq& soundFrequency);

	/// <summary>
	/// 指定の音の再生
	/// </summary>
	/// <param name="soundName">音の名前</param>
	/// <param name="volPar">音量:割合(0.0f~1.0f)</param>
	void Play(const std::string& soundName, float volPar = 0.0f);

	/// <summary>
	/// 指定の音の停止
	/// </summary>
	void Stop(const std::string& soundName) const;

	/// <summary>
	/// 指定した音の周波数変化
	/// </summary>
	void FrequencySound(const std::string& soundName);

	/// <summary>
	/// 指定した音の周波数変化速度を設定
	/// </summary>
	void SetFrequencySpeed(const std::string& soundName, int speed);

	/// <summary>
	/// 指定した音の音量を設定
	/// </summary>
	/// <param name="soundName">音の名前</param>
	/// <param name="volPar">音量:割合(0.0f~1.0f)</param>
	void SetSoundVolume(const std::string& soundName, float volPar);

	/// <summary>
	/// 指定した音が再生中か
	/// </summary>
	/// <returns>
	/// true : 再生中
	/// false : 再生していない、登録されていない
	/// </returns>
	bool IsPlay(const std::string& soundName) const;


private:

	Observer& observer_;

	/// <summary>
	/// 音の情報
	/// </summary>
	std::unordered_map<std::string, SoundInfo> soundsMap_;

	/// <summary>
	/// 音量を更新
	/// </summary>
	void UpdateVolume(void);

	/// <summary>
	/// UIDataからSE音量を取得
	/// </summary>
	const float GetSEVolumePar(void);

	/// <summary>
	/// UIDataからBGM音量を取得
	/// </summary>
	const float GetBGMVolumePar(void);
};


