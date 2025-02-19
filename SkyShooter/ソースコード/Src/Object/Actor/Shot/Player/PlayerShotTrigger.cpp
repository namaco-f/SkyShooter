
#include "../Manager/TimeManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ResourceManager/Sound/SoundResourcesManager.h"
#include "../Manager/ActorsManager/Shot/ShotManager.h"
#include "../Manager/UIManager/UIDataManager.h"
#include "../Object/Actor/Shot/Player/PlayerShot_Standard.h"
#include "../Object/Actor/Shot/Player/PlayerShot_Missile.h"
#include "../Object/Actor/Shot/Player/PlayerShot_Ult.h"
#include "../Object/Actor/Shot/ShotFactory.h"
#include "../Sound/SoundController.h"
#include "../Utility/MathUtility.h"
#include "../UI/GetUIDataToVariantTemplate.h"
#include "PlayerShotTrigger.h"

using namespace std;

using Math = MathUtility;

//�ʏ�e���ˉ�
const std::string STANDARD_SOUND = "Standard";

//�~�T�C���e���ˉ�
const std::string MISSILE_SOUND = "Missile";

//�J�E���^�[�e���ˉ�
const std::string COUNTER_SOUND = "Counter";

//�ʏ�e1�����Ƃɂ��炷�p�x
const float STANDARD_SHOT_DIFF_DEG = 30.0f;

//�v���C���[����̒ʏ�e�̔��ˍ��W�ւ̃x�N�g��
const VECTOR SHOT_STANDARD_LOCALVEC = { 0.0f,200.0f,0.0f };

//�~�T�C���e1�����Ƃɂ��炷�p�x
const float SHOT_MISSILE_DIFF_DEG = 60.0f;

//�v���C���[����̃~�T�C���e�̔��ˍ��W�ւ̃x�N�g��
const VECTOR SHOT_MISSILE_LOCALVEC = { 0.0f,100.0f,-100.0f };

//�~�T�C���e�̏�����]
const VECTOR SHOT_MISSILE_INIT_SHOT_RAD = {
	Math::Deg2RadF(-90.0f),
	0.0f,
	0.0f
};

//�~�T�C���e�̔��ˈʒu����������
const float SHOT_MISSILE_LOCALPOS_DIFF_Y = 150.0f;

//�v���C���[����̃J�E���^�[�e�̔��ˍ��W�ւ̃x�N�g��
const VECTOR SHOT_COUNTER_LOCALVEC = { 0.0f,100.0f,-100.0f };

//�J�E���^�[�e�̏�����]
const VECTOR SHOT_COUNTER_INIT_SHOT_RAD = {
	Math::Deg2RadF(-90.0f),
	0.0f,
	0.0f
};

//�J�E���^�[�e�̔��ˈʒu����������
const float SHOT_COUNTER_LOCAL_POS_DIFF_Y = 150.0f;

PlayerShotTrigger::PlayerShotTrigger(const Transform& trans)
	:
	timeMng_(TimeManager::GetInstance()),
	shotMng_(ShotManager::GetInstance()),
	resMngSound_(ResourceManager::GetInstance().GetSoundResourcesInstance()),
	playerTransform_(trans),
	createShotData_(ShotData()),
	shotDataMap_{},
	soundController_(make_unique<SoundController>())
{
	AddCreateShotInit();

	shotDataMap_.emplace(PlayerShotPattern::None,ShotData());
}

PlayerShotTrigger::~PlayerShotTrigger(void)
{
}

void PlayerShotTrigger::AddShotPattern(const PlayerShotPattern& shotPattern, const ShotData& shotData)
{
	shotDataMap_.emplace(shotPattern, shotData);
}

void PlayerShotTrigger::UpdateShotPattern(const PlayerShotPattern& shotPattern, const ShotData& shotData)
{
	if (!shotDataMap_.contains(shotPattern))
	{
		//�e��ʂ��o�^����Ă��Ȃ�
		return;
	}

	shotDataMap_.at(shotPattern) = shotData;
}

void PlayerShotTrigger::Update(int& mp)
{
	if (soundController_ != nullptr)
	{
		soundController_->Update();
	}

	if (!createShot_)
	{
		return;
	}

	float deltaTime = timeMng_.GetDeltaTime();

	if (IsTriggerCoolTime())
	{
		coolTimeStep_ -= deltaTime;
	}

	createShotData_.shotDelayStep_ += deltaTime;
	if (mp < createShotData_.cost_)
	{
		CreateShotEnd();
		return;
	}

	if (createShotData_.IsEndShotDelay() && createShotData_.IsAbleShot())
	{
		mp -= createShotData_.cost_;
		createShot_();
	}
}

void PlayerShotTrigger::UpdateTransform(const Transform& transform)
{
	playerTransform_ = transform;
}

void PlayerShotTrigger::CreateShotInit_None(void)
{
	createShot_ = bind(&PlayerShotTrigger::CreateShot_None, this);
}

void PlayerShotTrigger::CreateShotInit_Standard(void)
{
	AddShotSound(STANDARD_SOUND);
	coolTimeStep_ = createShotData_.coolTime_;
	createShotData_.Init();
	createShot_ = bind(&PlayerShotTrigger::CreateShot_Standard, this);
}

void PlayerShotTrigger::CreateShotInit_Missile(void)
{
	coolTimeStep_ = createShotData_.coolTime_;
	AddShotSound(MISSILE_SOUND);
	createShotData_.Init();
	createShot_ = bind(&PlayerShotTrigger::CreateShot_Missile, this);
}

void PlayerShotTrigger::CreateShotInit_CounterShot(void)
{
	auto& uiData = UIDataManager::GetInstance();
	uiData.GetUIData(SRC_UIData::PLAYER_MP);
	AddShotSound(COUNTER_SOUND);
	createShotData_.Init();
	createShot_ = bind(&PlayerShotTrigger::CreateShot_CounterShot, this);
}

void PlayerShotTrigger::CreateShot_None(void)
{
}

void PlayerShotTrigger::CreateShot_Standard(void)
{
	Transform trans;
	trans.pos = playerTransform_.pos;

	Quaternion qua = Quaternion::Euler(
		{
			0.0f,
			0.0f,
			(float)(createShotData_.shotNum_ - 1) * Math::Deg2RadF(STANDARD_SHOT_DIFF_DEG)
		}
	);

	VECTOR localPos = qua.PosAxis(SHOT_STANDARD_LOCALVEC);
	trans.pos = VAdd(playerTransform_.pos, localPos);

	soundController_->Play(STANDARD_SOUND);
	shotMng_.AddShot([&] {return Create<PlayerShot_Standard>(trans); });

	createShotData_.shotDelayStep_ = 0.0f;

	++createShotData_.shotNum_;

	if (!createShotData_.IsAbleShot())
	{
		CreateShotEnd();
	}
}

void PlayerShotTrigger::CreateShot_Missile(void)
{
	//�e�̉�]����W���i�[
	Transform trans;

	//�e�̉�]
	//���@�̉�]�𔽉f
	trans.quaRot = playerTransform_.quaRot;

	//�e��X����90�x��]
	trans.quaRot = trans.quaRot.Mult(Quaternion::Euler(SHOT_MISSILE_INIT_SHOT_RAD));

	//�e�̔��˕�����6������
	trans.quaRot = trans.quaRot.Mult(
		Quaternion::Euler(
			{
				0.0f,
				(float)createShotData_.shotNum_ * Math::Deg2RadF(-SHOT_MISSILE_DIFF_DEG),
				0.0f
			}
		)
	);


	//�e�̔��ˈʒu�̉�]
	Quaternion qua = Quaternion::Euler(
		{
			0.0f,
			0.0f,
			(float)createShotData_.shotNum_ * Math::Deg2RadF(SHOT_MISSILE_DIFF_DEG),
		}
	);

	//���ˈʒu�̃��[�J�����W
	VECTOR localVec = qua.PosAxis(SHOT_MISSILE_LOCALVEC);

	//���ˈʒu�Ƀv���C���[�̉�]�𔽉f
	VECTOR localPos = playerTransform_.quaRot.PosAxis(localVec);

	trans.pos = VAdd(playerTransform_.pos, localPos);
	trans.pos.y += SHOT_MISSILE_LOCALPOS_DIFF_Y;

	soundController_->Play(MISSILE_SOUND);
	shotMng_.AddShot([&] {return Create<PlayerShot_Missile>(trans); });

	createShotData_.shotDelayStep_ = 0.0f;

	++createShotData_.shotNum_;

	if (!createShotData_.IsAbleShot())
	{
		CreateShotEnd();
	}
}

void PlayerShotTrigger::CreateShot_CounterShot(void)
{
	const float shotDiffDeg = 360.0f / (float)createShotData_.shotNumMax_;



	//�e�̉�]����W���i�[
	Transform trans;

	while (createShotData_.IsAbleShot())
	{
		//�e�̉�]
		//���@�̉�]�𔽉f
		trans.quaRot = playerTransform_.quaRot;

		//�e��X����90�x��]
		trans.quaRot = trans.quaRot.Mult(Quaternion::Euler(SHOT_COUNTER_INIT_SHOT_RAD));

		//�e�̔��˕�����6������
		trans.quaRot = trans.quaRot.Mult(
			Quaternion::Euler(
				{
					0.0f,
					(float)createShotData_.shotNum_ * Math::Deg2RadF(-shotDiffDeg),
					0.0f
				}
			)
		);

		//�e�̔��ˈʒu�̉�]
		Quaternion qua = Quaternion::Euler(
			{
				0.0f,
				0.0f,
				(float)createShotData_.shotNum_ * Math::Deg2RadF(shotDiffDeg),
			}
		);

		//���ˈʒu�̃��[�J�����W
		VECTOR localVec = qua.PosAxis(SHOT_COUNTER_LOCALVEC);

		//���ˈʒu�Ƀv���C���[�̉�]�𔽉f
		VECTOR localPos = playerTransform_.quaRot.PosAxis(localVec);

		trans.pos = VAdd(playerTransform_.pos, localPos);
		trans.pos.y += SHOT_COUNTER_LOCAL_POS_DIFF_Y;


		shotMng_.AddShot([&] {return Create<PlayerShot_Missile>(trans); });

		++createShotData_.shotNum_;

		if (!createShotData_.IsAbleShot())
		{
			soundController_->Play(COUNTER_SOUND);
			CreateShotEnd();
		}
	}
}

void PlayerShotTrigger::AddCreateShotInit(void)
{
	createShotInit_.emplace(
		PlayerShotPattern::None,
		bind(&PlayerShotTrigger::CreateShotInit_None, this));

	createShotInit_.emplace(
		PlayerShotPattern::Standard,
		bind(&PlayerShotTrigger::CreateShotInit_Standard, this));

	createShotInit_.emplace(
		PlayerShotPattern::Missile,
		bind(&PlayerShotTrigger::CreateShotInit_Missile, this)
	);

	createShotInit_.emplace(
		PlayerShotPattern::CounterShot,
		bind(&PlayerShotTrigger::CreateShotInit_CounterShot, this)
	);
}

void PlayerShotTrigger::AddShotSound(const std::string& soundName)
{
	SoundInfo soundInfo = SoundInfo();
	soundInfo.handle = createShotData_.shotSoundId_;
	soundInfo.playType = DX_PLAYTYPE_BACK;

	soundController_->Add(soundName, soundInfo);

}

void PlayerShotTrigger::CreateShotEnd(void)
{
	//���ˏI��
	CreateShot(PlayerShotPattern::None);
}

void PlayerShotTrigger::DrawDebug(void)
{
}

void PlayerShotTrigger::CreateShot(const PlayerShotPattern& shotPattern)
{
	if (IsTriggerCoolTime())
	{
		return;
	}

	if (!createShotInit_.contains(shotPattern) || !shotDataMap_.contains(shotPattern))
	{
		return;
	}

	createShotData_ = shotDataMap_.at(shotPattern);

	createShotInit_.at(shotPattern)();
}

const bool PlayerShotTrigger::AbleTriggerShot(void) const
{
	return !createShotData_.IsAbleShot();
}

const bool PlayerShotTrigger::IsTriggerCoolTime(void) const
{
	return coolTimeStep_ > 0.0f;
}

