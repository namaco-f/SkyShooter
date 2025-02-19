#include "UIManager.h"

UIManager* Singleton<UIManager>::instance_ = nullptr;

void UIManager::AddUI(std::function<std::unique_ptr<UIBase>()> createFunc)
{
	//�L�����N�^�[�z��ɒǉ�
	ui_.emplace_back(createFunc());
}

UIManager::UIManager(void)
	:
	ui_{}
{
}

UIManager::~UIManager(void)
{
	ClearUIs();
}
