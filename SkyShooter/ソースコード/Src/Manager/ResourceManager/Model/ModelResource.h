#pragma once
#include <vector>
#include "../ResourceBase.h"

/// <summary>
/// モデルの読込、解放
/// </summary>
class ModelResource :
    public ResourceBase
{

public:

    ModelResource(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="path">
    /// Data/Model/以降のパス
    /// </param>
    explicit ModelResource(const std::string& path);
    virtual ~ModelResource(void) override;

    /// <summary>
    /// 読込
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// 解放
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// 複製モデルの配列に追加
    /// </summary>
    /// <param name="duplicateHandleId">
    /// 複製されたモデルのハンドルID
    /// </param>
    void Add_DuplicateModelId(const int& duplicateHandleId);

    /// <summary>
    /// 複製されたモデルの配列の解放
    /// </summary>
    void Clear_DuplicateModelIds(void);

    /// <summary>
    /// 複製されたモデルの配列を取得
    /// </summary>
    /// <returns>duplicateModelIds_</returns>
    const std::vector<int>& Get_DuplicateModelIds(void) const;


private:

    //モデル複製用
    std::vector<int> duplicateModelIds_;

};

