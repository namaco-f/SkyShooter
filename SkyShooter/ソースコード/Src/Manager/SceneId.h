#pragma once
#include <vector>

/// <summary>
/// �V�[���Ǘ��p
/// </summary>
enum class SCENE_ID
{
	NONE,		//�������p
	TITLE,		//�^�C�g���V�[��
	GAME,		//�Q�[���V�[��
	RESULT,		//���U���g�V�[��
	CLEAR,		//�N���A�V�[��
	FAILED,		//�s�k�V�[��
	MAX,		//�V�[�����J�E���g�p
};

const std::vector<SCENE_ID> USESCENE_GAME_ONLY = {
		SCENE_ID::GAME,
};

const std::vector<SCENE_ID> USESCENE_TITLE_ONLY = {
		SCENE_ID::TITLE,
};

const std::vector<SCENE_ID> USESCENE_ALL = {
		SCENE_ID::TITLE,
		SCENE_ID::GAME,
		SCENE_ID::CLEAR,
		SCENE_ID::FAILED,
};
