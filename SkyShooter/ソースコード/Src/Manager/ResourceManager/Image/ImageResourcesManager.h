#pragma once

#include "../ResourcesManagerTemplate.h"
#include"ImageResource.h"

/// <summary>
/// �摜���\�[�X�̖��O
/// </summary>
enum class SRC_IMAGE
{
	TrailTexture,						//�ǔ��e�̋O�Ճe�N�X�`��
	WATERWAVE_CUBE_TEX,					//���ʃe�N�X�`��
	CircleShadow,						//�ۉe
	Keyboard_Mouse,						//�L�[�}�E�摜
	Controller,							//�R���g���[���[�摜
	TitleLogo,							//�^�C�g�����S
	ClearText,							//�N���A�e�L�X�g
	LoadEnd_Start,						//�X�^�[�g�摜
	Cursor,								//�J�[�\��
	Arrow,								//���
	OperationInstructions_Controller,	//�R���g���[���[�������
	OperationInstructions_KeyBoard,		//�L�[�{�[�h�������
	HP_TEXT,							//HP
	MP_TEXT,							//MP
	SE_TEXT,							//SE
	BGM_TEXT,							//BGM
	BOSS_ENEMY_TEXT,					//�{�X
	MAX,
};

/// <summary>
/// �摜�̊Ǘ�
/// </summary>
class ImageResourcesManager
	: 
	public ResourcesManagerTemplate<
	ImageResourcesManager,
	ImageResource,
	SRC_IMAGE
	>
{

public:

	ImageResourcesManager(void);
	virtual ~ImageResourcesManager(void) override;

	/// <summary>
	/// ���
	/// </summary>
	void Release(void);

private:

	//�Ǎ�����
	ImageResource& _Load(const SRC_IMAGE src) override;

	//�G�t�F�N�g�̒ǉ�
	void AddResourceMap(const SRC_IMAGE src,const std::string& file);
	void AddResourceMap(
		const SRC_IMAGE src,
		const std::string& file,
		const std::vector<SCENE_ID>& useScenes
	);

};

