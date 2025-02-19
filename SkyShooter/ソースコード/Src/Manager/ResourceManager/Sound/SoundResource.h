#pragma once
#include <vector>
#include "../ResourceBase.h"

/// <summary>
/// 音の読込、解放
/// </summary>
class SoundResource :
    public ResourceBase
{

public:

    SoundResource(void) = default;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="path">
    /// Data/Sound/以降のパス
    /// </param>
    explicit SoundResource(const std::string& path);
    virtual ~SoundResource(void) override;

    /// <summary>
    /// 読込
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// 解放
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// 複製されたサウンドのハンドルを追加
    /// </summary>
    /// <param name="duplicateHandleId">
    /// 複製されたサウンドのハンドルID
    /// </param>
    void Add_DuplicateSoundId(const int& duplicateHandleId);

    /// <summary>
    /// 複製されたサウンドの配列を解放
    /// </summary>
    void Clear_DuplicateSoundIds(void);

    /// <summary>
    /// 複製されたサウンドのハンドルID配列を取得
    /// </summary>
    const std::vector<int>& Get_DuplicateSoundIds(void) const;


private:

    //サウンド複製用
    std::vector<int> duplicateSoundIds_;
    
};

