#pragma once
#include "../ResourceBase.h"

/// <summary>
/// �t�H���g�̓Ǎ��A���
/// </summary>
class FontResource :
    public ResourceBase
{
public:

    FontResource(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="path">
    /// Data/Fonts/�ȍ~�̃p�X
    /// </param>
    explicit FontResource(const std::string& path);
    
    virtual ~FontResource(void) override;

    /// <summary>
    /// �Ǎ�
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// ���
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// ���̑傫���ݒ�
    /// </summary>
    /// <param name="edgeSize">���̑傫��</param>
    void SetEdgeSize(const int edgeSize);

private:

    //���̑傫��
    int edgeSize_;
};



