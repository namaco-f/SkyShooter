#pragma once

#include <list>
#include <unordered_map>
#include <functional>
#include "../UI/UIBase.h"
#include "../../Singleton.h"

class UIBase;

/// <summary>
/// UI�Ǘ�
/// </summary>
class UIManager
	: public Singleton<UIManager>
{

public:
	/// <summary>
	/// �w�肵��UI���擾
	/// </summary>
	const std::unique_ptr<UIBase>& GetUI(const UIType uiType) const
	{
		//����̕����I�u�W�F�N�g�ɔ�Ή�
		for (const auto& ui : ui_)
		{
			if (ui)
			{
				if (ui->GetUIType() == uiType)
				{
					//�w���UI
					return ui;
				}
			}
		}

		//�w�肵��UI�����݂��Ȃ�
		return nullUI_;
	}

	/// <summary>
	/// UI�̒ǉ�
	/// </summary>
	/// <param name="createFunc">
	/// �Ώۂ�UI��Create�֐�
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
			//UI�����݂��Ȃ�
			return;
		}

		ui_.clear();
	}

private:

	friend class Singleton<UIManager>;

	//UI
	std::list<std::unique_ptr<UIBase>> ui_;

	//���UI
	const std::unique_ptr<UIBase> nullUI_ = nullptr;

	UIManager(void);
	virtual ~UIManager(void) override;

};

