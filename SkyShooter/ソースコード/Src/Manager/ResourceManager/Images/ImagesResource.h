#pragma once
#include "../ResourceBase.h"
#include "../../../Common/Vector2/Vector2.h"

/// <summary>
/// ���G�摜�̓Ǎ��A���
/// </summary>
class ImagesResource :
    public ResourceBase
{

public:

    /// <summary>
    /// �����摜�̏��
    /// </summary>
    struct ImagesInfo
    {
        int* handleIds;     //�n���h��ID�̔z��
        Vector2 num;        //xy�������Ƃ̕�����
        Vector2 size;       //������̃T�C�Y

        ImagesInfo() :
            handleIds(nullptr),
            num(Vector2()),
            size(Vector2()) {};
        
        /// <summary>
        /// �R���X�g���N�^
        /// </summary>
        /// <param name="num">xy�������Ƃ̕�����</param>
        /// <param name="size">������̃T�C�Y</param>
        ImagesInfo(const Vector2& num, const Vector2& size) :
            handleIds(nullptr),
            num(num),
            size(size) {};
        
        /// <summary>
        /// ���������擾
        /// </summary>
        const int GetNumMax(void) const
        {
            return num.x * num.y;
        }
        
    };

    ImagesResource(void) = default;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="path">
    /// Data/Images/�ȍ~�̃p�X
    /// </param>
    /// <param name="num">xy�������Ƃ̕�����</param>
    /// <param name="size">������̃T�C�Y</param>
    ImagesResource(
        const std::string& path,
        const Vector2& num,
        const Vector2& size
    );

    virtual ~ImagesResource(void) override;

    /// <summary>
    /// �Ǎ�
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// ���
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// �����摜�̃R�s�[
    /// </summary>
    /// <param name="imgs">�摜���R�s�[���邽�߂̔z��</param>
    void CopyImages(int* imgs);

    /// <summary>
    /// �����摜�̏����擾
    /// </summary>
    /// <returns>imagesInfo_</returns>
    const ImagesInfo& GetImagesInfo(void) const;


private:

    //�����摜���
    ImagesInfo imagesInfo_;

};

