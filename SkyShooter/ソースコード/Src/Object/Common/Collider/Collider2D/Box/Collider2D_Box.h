#pragma once
#include "../Collider2D.h"


/// <summary>
/// ��`��2D�R���C�_�̊��N���X
/// </summary>
class Collider2D_Box :
    public Collider2D
{
public:

    /// <summary>
    /// ��`�̍��W���
    /// </summary>
    struct BoxData
    {
        Vector2f posLeftTop;
        Vector2f posLeftBottom;

        Vector2f posRightTop;
        Vector2f posRightBottom;
    };

    Collider2D_Box(void) = default;

	/// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="colData">2D�R���C�_���</param>
	explicit Collider2D_Box(const ColliderData_2D& colData, const bool colliderDef = true);
    virtual ~Collider2D_Box(void);

    /// <summary>
    /// �R���C�_�̕`��
    /// </summary>
    virtual void Draw(void)override;

    /// <summary>
    /// ������̍X�V
    /// </summary>
    /// <param name="trans">������</param>
    virtual void UpdateTransform(const Transform& trans) override;

protected:

    //��`�̍��W���
    BoxData boxData_;

    //��`�̍��W���v�Z
    virtual void CalculateBoxPos(void) = 0;

    //Vector2f��VECTOR�ɕϊ�
    virtual const VECTOR Vector2fToVECTOR(const Vector2f& pos) = 0;

    //�R���C�_�̐F���擾
    virtual const unsigned int GetColliderColor(void)const;

    virtual const VECTOR GetDirection(void) const = 0;

private:
};

