
#include "../Manager/UIManager/UIDataManager.h"
#include "UIBase.h"

UIBase::UIBase(void)
	:
	uiDataMng_(UIDataManager::GetInstance()),
	uiType_(UIType::None),
	isUIEnd_(false)
{
}

void UIBase::Update(void)
{
}

void UIBase::Draw(void)
{
}
