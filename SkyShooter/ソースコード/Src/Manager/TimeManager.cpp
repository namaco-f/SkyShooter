#include "TimeManager.h"

#ifdef DEBUG
deltatime_ = 1.0f / 60.0f;
#endif // DEBUG

const float MINUTE = 60.0f;
const float DEFALUT_DELTATIME = 0.016f;
const float RESET_TIME = 0.0f;

TimeManager* Singleton<TimeManager>::instance_ = nullptr;

void TimeManager::Update(void)
{
    MeasureDeltaTime();

    if (isStop_) 
    {
        //ŽžŠÔ‚ð‰ÁŽZ‚µ‚È‚¢
        return; 
    }

    time_ += deltaTime_;
}

void TimeManager::ResetDeltaTime(void)
{
    deltaTime_ = DEFALUT_DELTATIME;
    preTime_ = std::chrono::system_clock::now();
    time_ = RESET_TIME;
}

void TimeManager::SetStop(bool isStop)
{
    isStop_ = isStop;
}

const bool TimeManager::GetIsStop(void)
{
    return isStop_;
}

const float TimeManager::GetTime(void)
{
    return time_;
}

const float TimeManager::GetDeltaTime(void)
{
    return deltaTime_;
}

const Time_Unit TimeManager::GetTime_Unit(void)
{
    return GetTime_Unit(time_);
}

const Time_Unit TimeManager::GetTime_Unit(const float time)
{
    float second = 0.0f;

    //ƒ~ƒŠ•b‚ÌŽæ“¾
    float mSecond = modff(time, &second);

    //•ª‚ÌŽæ“¾
    float min = second / MINUTE;

    //•b‚ðŽæ“¾
    second = fmodf(second, MINUTE);


    return Time_Unit(min, second, mSecond);
}

TimeManager::TimeManager(void)
    :
    preTime_{},
    time_(0.0f),
    deltaTime_(0.0f),
    isStop_(false)
{
}

TimeManager::~TimeManager(void)
{
}

void TimeManager::MeasureDeltaTime(void)
{
    // ƒfƒ‹ƒ^ƒ^ƒCƒ€
    const auto& nowTime = std::chrono::system_clock::now();

    deltaTime_ = static_cast<float>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);

    preTime_ = nowTime;
}
