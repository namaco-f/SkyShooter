#pragma once


#include "../ResourcesManagerTemplate.h"
#include "SoundResource.h"

/// <summary>
///�@���̃��\�[�X��
/// </summary>
enum class SRC_SOUND
{
	BGM_TITLE,				//�^�C�g��BGM
	BGM_TUTORIAL,			//�`���[�g���A��BGM
	BGM_GAME,				//�Q�[��BGM
	BGM_CLEAR,				//�N���ABGM
	BGM_FAILED,				//�s�kBGM
	
	SE_PLAYER_SHOT,				//�r�[���K��
	SE_PLAYER_SHOT_SPECIAL,			//�r�[���C
	SE_PLAYER_ULT_CHAGE,	//�r�[���C�`���[�W
	SE_PLAYER_DAMAGE,				//�Ō�
	SE_PLAYER_DASH,				//�u�[�X�^�[
	SE_ENEMY_WARP,			//�o��
	SE_MENU_ENTER,				//����
	SE_MENU_CURSORMOVE,			//�J�[�\���ړ�
	SE_SHIELDCOUNTER_CHARGE,				//�p���[�A�b�v
	SE_SPEEDUP,				//�X�s�[�h�A�b�v
	SE_ENEMY_COOLTIME_END,			//�f�U�艹
	
	MAX,					//�ő吔�J�E���g�p
};

/// <summary>
/// ���̊Ǘ�
/// </summary>
class SoundResourcesManager
	: public ResourcesManagerTemplate<
	SoundResourcesManager,
	SoundResource,
	SRC_SOUND
	>
{

public:

	SoundResourcesManager(void);
	virtual ~SoundResourcesManager(void) override;
	
	/// <summary>
	/// ���̕��� : 3���𒴂���ƕ����s��
	/// </summary>
	/// <param name="src">���\�[�X��</param>
	/// <returns>�n���h����</returns>
	int LoadSoundDuplicate(const SRC_SOUND src);

	/// <summary>
	/// ���
	/// </summary>
	void Release(void);


private:

	//���\�[�X�̓Ǎ�
	SoundResource& _Load(const SRC_SOUND src) override;

	//���\�[�X�̓o�^
	void AddResourceMap(const SRC_SOUND src, const std::string& file);
	void AddResourceMap(
		const SRC_SOUND src,
		const std::string& file,
		const std::vector<SCENE_ID>& useScene
	);
};

