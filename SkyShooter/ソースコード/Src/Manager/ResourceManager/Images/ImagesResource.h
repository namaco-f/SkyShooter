#pragma once
#include "../ResourceBase.h"
#include "../../../Common/Vector2/Vector2.h"

/// <summary>
/// 複雑画像の読込、解放
/// </summary>
class ImagesResource :
    public ResourceBase
{

public:

    /// <summary>
    /// 複数画像の情報
    /// </summary>
    struct ImagesInfo
    {
        int* handleIds;     //ハンドルIDの配列
        Vector2 num;        //xy方向ごとの分割数
        Vector2 size;       //分割後のサイズ

        ImagesInfo() :
            handleIds(nullptr),
            num(Vector2()),
            size(Vector2()) {};
        
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="num">xy方向ごとの分割数</param>
        /// <param name="size">分割後のサイズ</param>
        ImagesInfo(const Vector2& num, const Vector2& size) :
            handleIds(nullptr),
            num(num),
            size(size) {};
        
        /// <summary>
        /// 分割数を取得
        /// </summary>
        const int GetNumMax(void) const
        {
            return num.x * num.y;
        }
        
    };

    ImagesResource(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="path">
    /// Data/Images/以降のパス
    /// </param>
    /// <param name="num">xy方向ごとの分割数</param>
    /// <param name="size">分割後のサイズ</param>
    ImagesResource(
        const std::string& path,
        const Vector2& num,
        const Vector2& size
    );

    virtual ~ImagesResource(void) override;

    /// <summary>
    /// 読込
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// 解放
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// 複数画像のコピー
    /// </summary>
    /// <param name="imgs">画像をコピーするための配列</param>
    void CopyImages(int* imgs);

    /// <summary>
    /// 複数画像の情報を取得
    /// </summary>
    /// <returns>imagesInfo_</returns>
    const ImagesInfo& GetImagesInfo(void) const;


private:

    //複数画像情報
    ImagesInfo imagesInfo_;

};

