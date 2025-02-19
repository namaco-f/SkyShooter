#pragma once

#include "../ResourcesManagerTemplate.h"
#include"FontResource.h"

/// <summary>
/// �摜���\�[�X�̖��O
/// </summary>
enum class SRC_FONT
{
	Pgothic_s20,
	Pgothic_s32,
	Pgothic_s40,
	Pgothic_s50,
	Pgothic_s60,
	MAX,
};

/// <summary>
/// �摜�̊Ǘ�
/// </summary>
class FontResourcesManager
	:
	public ResourcesManagerTemplate<
	FontResourcesManager,
	FontResource,
	SRC_FONT
	>
{

public:

	FontResourcesManager(void);
	virtual ~FontResourcesManager(void) override;

	/// <summary>
	/// ���
	/// </summary>
	void Release(void);

private:

	//�Ǎ�����
	FontResource& _Load(const SRC_FONT src) override;

	//�G�t�F�N�g�̒ǉ�
	void AddResourceMap(const SRC_FONT src, const std::string& file,const int edgeSize = 0);

};

