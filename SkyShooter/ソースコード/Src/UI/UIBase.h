#pragma once

#include <DxLib.h>
#include <variant>
#include <memory>
#include "UIType.h"

class UIDataManager;

class UIBase
{
public:

	UIBase(void);
	virtual ~UIBase(void) = default;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// UI�^�O���擾
	/// </summary>
	inline const UIType GetUIType(void) const
	{
		return uiType_;
	}

	/// <summary>
	/// UI�^�O��ݒ�
	/// </summary>
	/// <param name="type"></param>
	inline void SetUIType(const UIType type)
	{
		uiType_ = type;
	}

protected:
	
	//UI�f�[�^
	UIDataManager& uiDataMng_;

	//UI�^�O
	UIType uiType_;

	bool isUIEnd_;
};

