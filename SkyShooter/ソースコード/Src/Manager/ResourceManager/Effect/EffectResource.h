#pragma once
#include "../ResourceBase.h"

/// <summary>
/// �G�t�F�N�g�̓Ǎ��A���
/// </summary>
class EffectResource :
    public ResourceBase
{
public:

    EffectResource(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="path">
    /// Data/Effect/�ȍ~�̃p�X
    /// </param>
    explicit EffectResource(const std::string& path);
    virtual ~EffectResource(void) override;

    /// <summary>
    /// ���\�[�X�̓Ǎ�
    /// </summary>
    void Load(void) override;


    /// <summary>
    /// ���\�[�X�̉��
    /// </summary>
    void Release(void) override;

private:
};

