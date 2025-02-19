#pragma once

#include <unordered_map>
#include <DxLib.h>
#include <variant>
#include "../UI/UIType.h"
#include "../../Singleton.h"


/// <summary>
/// UI用データ管理クラス
/// </summary>
class UIDataManager
	:
	public Singleton<UIDataManager>
{
public:

	/// <summary>
	/// 指定したUIデータを更新
	/// </summary>
	/// <typeparam name="T">int,float,VECTOR</typeparam>
	template<typename T>
	inline void SetUIData(const SRC_UIData dataName,const T val)
	{
		if (!uiDataMap_.contains(dataName))
		{
			//指定されたUIデータが存在しない
			return;
		}

		uiDataMap_.at(dataName) = val;
	}


	/// <summary>
	/// 指定したUIデータ取得
	/// </summary>
	/// <return>
	/// -1 : データが登録されていない、または存在しない
	/// </reutrn>
	inline const std::variant<int, float, VECTOR>& GetUIData(const SRC_UIData dataName)
	{
		if (!uiDataMap_.contains(dataName))
		{
			//指定されたUIデータが存在しない
			return nullData_;
		}

		return uiDataMap_.at(dataName);
	}

private:

	friend class Singleton<UIDataManager>;

	//空のデータ
	const std::variant<int, float, VECTOR> nullData_ = -1;

	//UI用データ
	std::unordered_map<SRC_UIData, std::variant<int,float,VECTOR>> uiDataMap_;


	UIDataManager(void);
	virtual ~UIDataManager(void) override;

	//UI用データ初期化用
	void UIDataInit(void);

	template<typename T> 
	inline void AddUIData(const SRC_UIData src, const T& val)
	{
		uiDataMap_.emplace(src, val);
	}

};

