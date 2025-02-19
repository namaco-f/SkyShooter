#pragma once

#include "../ResourcesManagerTemplate.h"
#include "EffectResource.h"

//�G�t�F�N�g���\�[�X��
enum class SRC_EFFECT
{
	HIT,		//�q�b�g�G�t�F�N�g
	ULT,		//Ult�G�t�F�N�g
	ULT_CHARGE,	//�V�[���h
	WARP,		//���[�v�G�t�F�N�g
	SLASTER,	//�X���X�^�[�G�t�F�N�g
	WATER,		//���򖗃G�t�F�N�g
	SHIELD,		//�V�[���h
	MAX,		//�ő吔�J�E���g�p
};

/// <summary>
/// �G�t�F�N�g�̊Ǘ�
/// </summary>
class EffectResourcesManager
	: 
	public ResourcesManagerTemplate<
	EffectResourcesManager,
	EffectResource,
	SRC_EFFECT
	>
{

public:

	EffectResourcesManager(void);
	virtual ~EffectResourcesManager(void) override;

	/// <summary>
	/// �������
	/// </summary>
	void Release(void);


private:

	//�Ǎ�
	EffectResource& _Load(const SRC_EFFECT src) override;

	/// <summary>
	/// �G�t�F�N�g�̒ǉ�
	/// </summary>
	/// <param name="src">���\�[�X��</param>
	/// <param name="file">�t�@�C����</param>
	void AddResourceMap(const SRC_EFFECT src, const std::string& file);
	void AddResourceMap(
		const SRC_EFFECT src, 
		const std::string& file,
		const std::vector<SCENE_ID>& useSceneId);

};

