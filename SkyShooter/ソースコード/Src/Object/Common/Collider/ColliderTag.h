#pragma once
#include "../../ObjectTag.h"


/// <summary>
/// �Ǐ]����t���[����
/// </summary>
enum class COLLISION_SUBTYPE
{
	NONE,
	MAX,
};

/// <summary>
/// �Փ˔���̌`��
/// </summary>
enum class COLLISION_TYPE
{
	NONE,		//�������p
	SPHERE,		//����
	CAPSULE,	//�J�v�Z��
	MODEL,		//���f��
	LINE,		//����
	CYLINDER,	//�V�����_�[
};
