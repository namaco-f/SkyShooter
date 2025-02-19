#pragma once
#include "Collider2D_Box.h"

/// <summary>
/// ZY���ʏ�̋�`��2D�R���C�_
/// </summary>
class Collider2D_BoxZY :
    public Collider2D_Box
{

public:

	Collider2D_BoxZY(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="colData">2D�R���C�_���</param>
	/// <param name="colliderDef">�R���C�_��L�������邩</param>
	explicit Collider2D_BoxZY(const ColliderData_2D& colData, const bool colliderDef = true);
	virtual ~Collider2D_BoxZY(void);

protected:

	//Vector2f��VECTOR�ɕϊ�
	virtual const VECTOR Vector2fToVECTOR(const Vector2f& pos) override;

	//��`�̍��W���v�Z
	virtual void CalculateBoxPos(void) override;

	//�R���C�_�`��p
	virtual const VECTOR GetDirection(void) const override;

};

