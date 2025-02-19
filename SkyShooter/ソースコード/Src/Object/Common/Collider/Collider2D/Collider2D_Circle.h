#pragma once
#include "Collider2D.h"


/// <summary>
/// �~��2D�R���C�_
/// </summary>
class Collider2D_Circle :
    public Collider2D
{

public:

	Collider2D_Circle(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="colData">�R���C�_���</param>
	/// <param name="colliderDef">�R���C�_��L�������邩</param>
	explicit Collider2D_Circle(const ColliderData_2D& colData, const bool colliderDef = true);
	virtual ~Collider2D_Circle(void);

	/// <summary>
	/// �R���C�_�`�揈��
	/// </summary>
	virtual void Draw(void)override;

private:
};

