#pragma once

#include <list>
#include "../../Common/Transform.h"


/// <summary>
/// �g���C��
/// </summary>
class Trail
{

public:

	Trail(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="trans">�Ǐ]�Ώۂ̐�����</param>
	Trail(const Transform& trans,const VECTOR& color);
	~Trail(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="trans">�Ǐ]�Ώۂ̐�����</param>
	void Update(const Transform& trans);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

	//void CalculateBillboard(void);

private:

	ShaderManager& shaderMng_;

	//�Ǐ]���Ă�����W
	VECTOR transformPos_;

	//�g���C���̍��W�z��
	std::list<VECTOR> trailPosList_;

	//�F
	COLOR_U8 diffuseColor_;

	//�g���C���̃e�N�X�`��
	int tex_;

	//�g���C���z��̍ő吔
	int trailNumMax_;

	//�g���C���̒���
	float trailLength_;

	//�g���C���֘A�̌v�Z
	void CalculateTrail(void);

	//�g���C���p���W�̒ǉ�
	void AddTrailPosList(const VECTOR& addTrailPos);

	void GetColorVECTOR(const VECTOR& color);

};

