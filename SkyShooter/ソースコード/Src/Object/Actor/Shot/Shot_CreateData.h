#pragma once

/// <summary>
/// 1トリガーごとの弾のデータ
/// </summary>
struct ShotData
{
    float coolTime_;        //クールタイム
    float shotDelay_;       //弾1発ごとのディレイ
    float shotDelayStep_;   //ディレイのカウント用
    int shotNumMax_;        //1トリガーで発射出来る弾数
    int shotNum_;           //弾のカウント用
    int cost_;              //コスト
    int shotSoundId_;       //サウンドハンドル

    ShotData(void)
        :
        coolTime_(0.0f),
        shotDelay_(0.0f),
        shotDelayStep_(0.0f),
        shotNumMax_(0),
        shotNum_(0) ,
        cost_(0),
        shotSoundId_(-1)
    {};

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="delayTime">弾1発ごとのディレイ</param>
    /// <param name="shotNumMax">1トリガーで発射出来る弾数</param>
    ShotData(const float delayTime, const int shotNumMax,const float coolTime,const int cost = 0)
        :
        coolTime_(coolTime),
        shotDelay_(delayTime),
        shotDelayStep_(delayTime),
        shotNumMax_(shotNumMax),
        shotNum_(0),
        cost_(cost),
        shotSoundId_(-1)
    {};

    /// <summary>
    /// カウント初期化
    /// </summary>
    void Init(void)
    {
        shotDelayStep_ = 0.0f;
        shotNum_ = 0;
    }

    /// <summary>
    /// 弾のディレイが終わっているか
    /// </summary>
    /// <returns>
    /// true : 終わっている
    /// false : 終わっていない
    /// </returns>
    const bool IsEndShotDelay(void) const
    {
        return shotDelayStep_ > shotDelay_;
    }

    /// <summary>
    /// 弾生成可能か
    /// </summary>
    /// <returns>
    /// true : 可能
    /// false : 弾数が上限
    /// </returns>
    const bool IsAbleShot(void) const
    {
        return shotNumMax_ > shotNum_;
    }
};