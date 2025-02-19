#pragma once
#include "../ResourceBase.h"

/// <summary>
/// 画像の読込、解放
/// </summary>
class ImageResource :
    public ResourceBase
{
public:

    ImageResource(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="path">
    /// Data/Image/以降のパス
    /// </param>
    explicit ImageResource(const std::string& path);
    virtual ~ImageResource(void) override;

    /// <summary>
    /// 読込
    /// </summary>
    void Load(void) override;
   
    /// <summary>
    /// 解放
    /// </summary>
    void Release(void) override;

private:

};

