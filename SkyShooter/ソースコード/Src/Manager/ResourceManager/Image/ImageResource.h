#pragma once
#include "../ResourceBase.h"

/// <summary>
/// �摜�̓Ǎ��A���
/// </summary>
class ImageResource :
    public ResourceBase
{
public:

    ImageResource(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="path">
    /// Data/Image/�ȍ~�̃p�X
    /// </param>
    explicit ImageResource(const std::string& path);
    virtual ~ImageResource(void) override;

    /// <summary>
    /// �Ǎ�
    /// </summary>
    void Load(void) override;
   
    /// <summary>
    /// ���
    /// </summary>
    void Release(void) override;

private:

};

