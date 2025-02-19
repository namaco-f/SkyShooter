#pragma once
#include "../ResourceBase.h"

/// <summary>
/// エフェクトの読込、解放
/// </summary>
class EffectResource :
    public ResourceBase
{
public:

    EffectResource(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="path">
    /// Data/Effect/以降のパス
    /// </param>
    explicit EffectResource(const std::string& path);
    virtual ~EffectResource(void) override;

    /// <summary>
    /// リソースの読込
    /// </summary>
    void Load(void) override;


    /// <summary>
    /// リソースの解放
    /// </summary>
    void Release(void) override;

private:
};

