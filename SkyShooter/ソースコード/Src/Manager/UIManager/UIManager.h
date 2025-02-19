#pragma once

#include <list>
#include <unordered_map>
#include <functional>
#include "../UI/UIBase.h"
#include "../../Singleton.h"

class UIBase;

/// <summary>
/// UI管理
/// </summary>
class UIManager
	: public Singleton<UIManager>
{

public:
	/// <summary>
	/// 指定したUIを取得
	/// </summary>
	const std::unique_ptr<UIBase>& GetUI(const UIType uiType) const
	{
		//同一の複数オブジェクトに非対応
		for (const auto& ui : ui_)
		{
			if (ui)
			{
				if (ui->GetUIType() == uiType)
				{
					//指定のUI
					return ui;
				}
			}
		}

		//指定したUIが存在しない
		return nullUI_;
	}

	/// <summary>
	/// UIの追加
	/// </summary>
	/// <param name="createFunc">
	/// 対象のUIのCreate関数
	/// </param>
	void AddUI(std::function<std::unique_ptr<UIBase>()> createFunc);

	inline const std::list<std::unique_ptr<UIBase>>& GetUIs(void) const
	{
		return ui_;
	}

	inline void ClearUIs(void)
	{
		if (ui_.empty())
		{
			//UIが存在しない
			return;
		}

		ui_.clear();
	}

private:

	friend class Singleton<UIManager>;

	//UI
	std::list<std::unique_ptr<UIBase>> ui_;

	//空のUI
	const std::unique_ptr<UIBase> nullUI_ = nullptr;

	UIManager(void);
	virtual ~UIManager(void) override;

};

