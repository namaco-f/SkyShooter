#pragma once


#include <random>
#include <vector>
#include "../Singleton.h"

/// <summary>
/// 乱数系クラス
/// </summary>
class RandManager
	: public Singleton<RandManager>
{
public:

	/// <summary>
	/// 疑似乱数を使用し、int型の乱数を生成
	/// </summary>
	/// <param name="min">最低値</param>
	/// <param name="max">最高値</param>
	/// <returns>int型の数値</returns>
	const int GetRand_mt(int min, int max);

	/// <summary>
	/// 疑似乱数を使用し、float型の乱数を生成
	/// </summary>
	/// <param name="min">最低値</param>
	/// <param name="max">最高値</param>
	/// <returns>float型の数値</returns>
	const float GetRand_mt(float min,float max);

private:

	friend class Singleton<RandManager>;

	RandManager(void);
	virtual ~RandManager(void) override;

	//ランダムシードを生成
	void GenerateRand(void);

	//ランダムシード格納用
	std::random_device::result_type randomDeviceResult_;

};
