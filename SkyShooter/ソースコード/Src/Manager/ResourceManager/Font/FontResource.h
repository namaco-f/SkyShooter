#pragma once
#include "../ResourceBase.h"

/// <summary>
/// フォントの読込、解放
/// </summary>
class FontResource :
    public ResourceBase
{
public:

    FontResource(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="path">
    /// Data/Fonts/以降のパス
    /// </param>
    explicit FontResource(const std::string& path);
    
    virtual ~FontResource(void) override;

    /// <summary>
    /// 読込
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// 解放
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// 縁の大きさ設定
    /// </summary>
    /// <param name="edgeSize">縁の大きさ</param>
    void SetEdgeSize(const int edgeSize);

private:

    //縁の大きさ
    int edgeSize_;
};



