#pragma once
#include <vector>
#include "../ResourceBase.h"

/// <summary>
/// ���̓Ǎ��A���
/// </summary>
class SoundResource :
    public ResourceBase
{

public:

    SoundResource(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="path">
    /// Data/Sound/�ȍ~�̃p�X
    /// </param>
    explicit SoundResource(const std::string& path);
    virtual ~SoundResource(void) override;

    /// <summary>
    /// �Ǎ�
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// ���
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// �������ꂽ�T�E���h�̃n���h����ǉ�
    /// </summary>
    /// <param name="duplicateHandleId">
    /// �������ꂽ�T�E���h�̃n���h��ID
    /// </param>
    void Add_DuplicateSoundId(const int& duplicateHandleId);

    /// <summary>
    /// �������ꂽ�T�E���h�̔z������
    /// </summary>
    void Clear_DuplicateSoundIds(void);

    /// <summary>
    /// �������ꂽ�T�E���h�̃n���h��ID�z����擾
    /// </summary>
    const std::vector<int>& Get_DuplicateSoundIds(void) const;


private:

    //�T�E���h�����p
    std::vector<int> duplicateSoundIds_;
    
};

