#pragma once

/// <summary>
/// 1�g���K�[���Ƃ̒e�̃f�[�^
/// </summary>
struct ShotData
{
    float coolTime_;        //�N�[���^�C��
    float shotDelay_;       //�e1�����Ƃ̃f�B���C
    float shotDelayStep_;   //�f�B���C�̃J�E���g�p
    int shotNumMax_;        //1�g���K�[�Ŕ��ˏo����e��
    int shotNum_;           //�e�̃J�E���g�p
    int cost_;              //�R�X�g
    int shotSoundId_;       //�T�E���h�n���h��

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
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="delayTime">�e1�����Ƃ̃f�B���C</param>
    /// <param name="shotNumMax">1�g���K�[�Ŕ��ˏo����e��</param>
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
    /// �J�E���g������
    /// </summary>
    void Init(void)
    {
        shotDelayStep_ = 0.0f;
        shotNum_ = 0;
    }

    /// <summary>
    /// �e�̃f�B���C���I����Ă��邩
    /// </summary>
    /// <returns>
    /// true : �I����Ă���
    /// false : �I����Ă��Ȃ�
    /// </returns>
    const bool IsEndShotDelay(void) const
    {
        return shotDelayStep_ > shotDelay_;
    }

    /// <summary>
    /// �e�����\��
    /// </summary>
    /// <returns>
    /// true : �\
    /// false : �e�������
    /// </returns>
    const bool IsAbleShot(void) const
    {
        return shotNumMax_ > shotNum_;
    }
};