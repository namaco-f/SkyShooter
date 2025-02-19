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
	/// 更新処理
	/// </summary>
	virtual void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// UIタグを取得
	/// </summary>
	inline const UIType GetUIType(void) const
	{
		return uiType_;
	}

	/// <summary>
	/// UIタグを設定
	/// </summary>
	/// <param name="type"></param>
	inline void SetUIType(const UIType type)
	{
		uiType_ = type;
	}

protected:
	
	//UIデータ
	UIDataManager& uiDataMng_;

	//UIタグ
	UIType uiType_;

	bool isUIEnd_;
};

