#pragma once

#include "ImagesResource.h"
#include "../ResourcesManagerTemplate.h"

/// <summary>
/// �����摜�̃��\�[�X��
/// </summary>
enum class SRC_IMAGES
{
	KEY_ENTER,		//�G���^�[�L�[�摜
	LOADING,		//���[�f�B���O�摜
	MAX,
};

/// <summary>
/// �����摜�̊Ǘ�
/// </summary>
class ImagesResourcesManager
	: public ResourcesManagerTemplate<
	ImagesResourcesManager,
	ImagesResource,
	SRC_IMAGES
	>
{

public:

	ImagesResourcesManager(void);
	virtual ~ImagesResourcesManager(void) override;

	/// <summary>
	/// �������
	/// </summary>
	void Release(void);


private:

	//�Ǎ�����
	ImagesResource& _Load(const SRC_IMAGES src) override;

	/// <summary>
	/// ���\�[�X�z��ɒǉ�
	/// </summary>
	/// <param name="src">���\�[�X��</param>
	/// <param name="file">�p�X</param>
	/// <param name="num">������</param>
	/// <param name="size">�ꖇ�̃T�C�Y</param>
	void AddResourceMap(
		const SRC_IMAGES src, 
		const std::string& file,
		const Vector2& num,
		const Vector2& size
	);

	/// <summary>
	/// ���\�[�X�z��ɒǉ�
	/// </summary>
	/// <param name="src">���\�[�X��</param>
	/// <param name="file">�p�X</param>
	/// <param name="num">������</param>
	/// <param name="size">�ꖇ�̃T�C�Y</param>
	void AddResourceMap(
		const SRC_IMAGES src,
		const std::string& file,
		const Vector2& num,
		const Vector2& size,
		const std::vector<SCENE_ID>& useScenes
	);

};

