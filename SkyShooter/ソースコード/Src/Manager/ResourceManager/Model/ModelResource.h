#pragma once
#include <vector>
#include "../ResourceBase.h"

/// <summary>
/// ���f���̓Ǎ��A���
/// </summary>
class ModelResource :
    public ResourceBase
{

public:

    ModelResource(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="path">
    /// Data/Model/�ȍ~�̃p�X
    /// </param>
    explicit ModelResource(const std::string& path);
    virtual ~ModelResource(void) override;

    /// <summary>
    /// �Ǎ�
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// ���
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// �������f���̔z��ɒǉ�
    /// </summary>
    /// <param name="duplicateHandleId">
    /// �������ꂽ���f���̃n���h��ID
    /// </param>
    void Add_DuplicateModelId(const int& duplicateHandleId);

    /// <summary>
    /// �������ꂽ���f���̔z��̉��
    /// </summary>
    void Clear_DuplicateModelIds(void);

    /// <summary>
    /// �������ꂽ���f���̔z����擾
    /// </summary>
    /// <returns>duplicateModelIds_</returns>
    const std::vector<int>& Get_DuplicateModelIds(void) const;


private:

    //���f�������p
    std::vector<int> duplicateModelIds_;

};

