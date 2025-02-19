

#include "RandManager.h"

using namespace std;

RandManager* Singleton<RandManager>::instance_ = nullptr;

const int RandManager::GetRand_mt(int min, int max)
{
    GenerateRand();
    //�^������
    mt19937 mt(randomDeviceResult_);

    //�����͈̔͂��w��
    uniform_int_distribution<int> uid(min, max);

    return uid(mt);
}

const float RandManager::GetRand_mt(float min, float max)
{
    //�^������
    mt19937 mt(randomDeviceResult_);

    //�����͈̔͂��w��
    uniform_real_distribution<float> uid(min, max);

    return uid(mt);
}

RandManager::RandManager(void)
    :
    randomDeviceResult_{}
{
    GenerateRand();
}

RandManager::~RandManager(void)
{
}

void RandManager::GenerateRand(void)
{
    //�����_���V�[�h�̐���
    random_device randomDevice;

    randomDeviceResult_ = randomDevice();
}
