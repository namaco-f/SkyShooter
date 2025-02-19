#pragma once

#include <unordered_map>
#include <DxLib.h>
#include <variant>
#include "../UI/UIType.h"
#include "../../Singleton.h"


/// <summary>
/// UI�p�f�[�^�Ǘ��N���X
/// </summary>
class UIDataManager
	:
	public Singleton<UIDataManager>
{
public:

	/// <summary>
	/// �w�肵��UI�f�[�^���X�V
	/// </summary>
	/// <typeparam name="T">int,float,VECTOR</typeparam>
	template<typename T>
	inline void SetUIData(const SRC_UIData dataName,const T val)
	{
		if (!uiDataMap_.contains(dataName))
		{
			//�w�肳�ꂽUI�f�[�^�����݂��Ȃ�
			return;
		}

		uiDataMap_.at(dataName) = val;
	}


	/// <summary>
	/// �w�肵��UI�f�[�^�擾
	/// </summary>
	/// <return>
	/// -1 : �f�[�^���o�^����Ă��Ȃ��A�܂��͑��݂��Ȃ�
	/// </reutrn>
	inline const std::variant<int, float, VECTOR>& GetUIData(const SRC_UIData dataName)
	{
		if (!uiDataMap_.contains(dataName))
		{
			//�w�肳�ꂽUI�f�[�^�����݂��Ȃ�
			return nullData_;
		}

		return uiDataMap_.at(dataName);
	}

private:

	friend class Singleton<UIDataManager>;

	//��̃f�[�^
	const std::variant<int, float, VECTOR> nullData_ = -1;

	//UI�p�f�[�^
	std::unordered_map<SRC_UIData, std::variant<int,float,VECTOR>> uiDataMap_;


	UIDataManager(void);
	virtual ~UIDataManager(void) override;

	//UI�p�f�[�^�������p
	void UIDataInit(void);

	template<typename T> 
	inline void AddUIData(const SRC_UIData src, const T& val)
	{
		uiDataMap_.emplace(src, val);
	}

};

