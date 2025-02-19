#pragma once

/// <summary>
/// UI���
/// </summary>
enum class UIType
{
	None,			//�������p
	
	PlayerParameter,		//���@HP
	EnemyHp,		//�GHP
	MiniMap,		//�~�j�}�b�v

	MAX,			//�ő吔�J�E���g�p
};

/// <summary>
/// UI�p�f�[�^��
/// </summary>
enum class SRC_UIData
{
	PLAYER_HP,				//���@��HP
	PLAYER_HPMAX,			//���@�ő�HP
	PLAYER_MP,				//���@��MP
	PLAYER_MPMAX,			//���@�ő�MP
	PLAYER_UC,				//���@�ő�MP
	PLAYER_UCMAX,			//���@�ő�MP
	PLAYER_SHILD_TIME,		//�V�[���h�\����
	PLAYER_SHILD_LIMIT,		//�V�[���h�������E����
	PLAYER_SHILD_COOLPAR,	//�V�[���h���J�E���^�[�J�E���g����
	PLAYER_POS,				//���@���W

	ENEMY_HP,				//�G��HP
	ENEMY_HPMAX,			//�G�ő�HP
	ENEMY_POS,				//�G���W

	BGM_VOLUME,				//BGM����
	SE_VOLUME,				//SE����

	CLEAR_TIME,				//�N���A�^�C��(�b)

	MAX,					//�ő吔�J�E���g�p�A�G���[�l
};
