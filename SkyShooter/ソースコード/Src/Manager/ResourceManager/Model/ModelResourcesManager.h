#pragma once

#include <mutex>
#include "ModelResource.h"
#include "../ResourcesManagerTemplate.h"

/// <summary>
/// ���f���̃��\�[�X��
/// </summary>
enum class SRC_MODEL
{
	BOT_X,					//�v���C���[
	Idle,					//�Î~��ԃA�j���[�V����
	Run,					//����A�j���[�V����
	Flying,					//��ԃA�j���[�V����
	Attack,					//�U���A�j���[�V����
	WATERWAVE_CUBE,			//����
	SKYDOME,				//�X�J�C�h�[��
	SHOT,					//�e
	ENEMY,					//�G
	ENEMY_BOXATTACK_CUBE,	//�G�̔��U��	
	UltAttack,				//Ult�A�j���[�V������
	Plane,					//Ult�A�j���[�V������
	MAX,					//�ő吔�J�E���g�p
};

/// <summary>
/// ���f���̊Ǘ�
/// </summary>
class ModelResourcesManager
	: 
	public ResourcesManagerTemplate<
	ModelResourcesManager, 
	ModelResource,
	SRC_MODEL
	>
{

public:

	ModelResourcesManager(void);
	virtual ~ModelResourcesManager(void) override;

	/// <summary>
	/// �w�肵�����\�[�X�̕������[�h
	/// </summary>
	/// <returns>�������ꂽ���f���̃n���h��ID</returns>
	int LoadModelDuplicate(const SRC_MODEL src);

	/// <summary>
	/// �������
	/// </summary>
	void Release(void);

	/// <summary>
	/// �w�肵�����\�[�X�����
	/// </summary>
	/// <param name="src">���\�[�X��</param>
	void Release(const SRC_MODEL src);

private:

	std::mutex mutex_;

	//�Ǎ�
	ModelResource& _Load(const SRC_MODEL src) override;

	//���f���̓o�^
	void AddResourceMap(const SRC_MODEL src, const std::string& file);
	void AddResourceMap(
		const SRC_MODEL src, 
		const std::string& file,
		const std::vector<SCENE_ID>& useScene
	);

};

