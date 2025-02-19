#pragma once

#include <list>
#include <unordered_map>
#include <functional>
#include "../../../Common/Transform.h"
#include "../../Shot/Shot_CreateData.h"

class TimeManager;
class ShotManager;
class SoundResourcesManager;
class SoundController;

enum class PlayerShotPattern
{
	None,
	Standard,
	Missile,
	Ult,
	CounterShot,
	MAX
};

class PlayerShotTrigger
{
public:

	PlayerShotTrigger(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="trans">
	/// �v���C���[�̃��f��������
	/// </param>
	explicit PlayerShotTrigger(const Transform& trans);
	virtual ~PlayerShotTrigger(void);

	/// <summary>
	/// �e�̎�ʂ�o�^
	/// </summary>
	/// <param name="shotPattern">�e�̎��</param>
	/// <param name="shotData">�e�g���K�[�̏��</param>
	void AddShotPattern(const PlayerShotPattern& shotPattern,const ShotData& shotData);
	
	/// <summary>
	/// �w�肵���e�̃g���K�[�����X�V
	/// </summary>
	/// <param name="shotPattern">�e�̎��</param>
	/// <param name="shotData">�e�g���K�[�̏��</param>
	void UpdateShotPattern(const PlayerShotPattern& shotPattern, const ShotData& shotData);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(int& mp);

	/// <summary>
	/// �v���C���[�̃��f����������X�V
	/// </summary>
	void UpdateTransform(const Transform& transform);

	/// <summary>
	/// �w�肵����ނ̒e�𐶐��J�n
	/// </summary>
	void CreateShot(const PlayerShotPattern& shotPattern);

	/// <summary>
	/// �e�𔭎ˉ\��
	/// </summary>
	const bool AbleTriggerShot(void) const;

	/// <summary>
	/// �g���K�[���N�[���^�C����
	/// </summary>
	const bool IsTriggerCoolTime(void) const;

	const ShotData& GetShotData(const PlayerShotPattern& shotPattern) const
	{
		if (!shotDataMap_.contains(shotPattern))
		{
			return shotDataNull_;
		}

		return shotDataMap_.at(shotPattern);
	}

private:

	TimeManager& timeMng_;
	ShotManager& shotMng_;
	SoundResourcesManager& resMngSound_;

	Transform playerTransform_;
	ShotData createShotData_;

	const ShotData shotDataNull_ = ShotData();

	std::unordered_map<PlayerShotPattern, ShotData> shotDataMap_;
	std::unique_ptr<SoundController> soundController_;

	//�N�[���^�C���J�E���g�p
	float coolTimeStep_;

	void DrawDebug(void);

	//�e�������ɍs���鏉����
	std::unordered_map<PlayerShotPattern, std::function<void(void)>> createShotInit_;
	void CreateShotInit_None(void);
	void CreateShotInit_Standard(void);
	void CreateShotInit_Missile(void);
	void CreateShotInit_CounterShot(void);
	
	//�e�������̊֐��I�u�W�F�N�g
	std::function<void(void)> createShot_;
	void CreateShot_None(void);
	void CreateShot_Standard(void);
	void CreateShot_Missile(void);
	void CreateShot_CounterShot(void);
	//void CreateShot_Ult(void);

	////�e�������ɍs���鏉������o�^
	void AddCreateShotInit(void);

	//���ˉ��ǉ�
	void AddShotSound(const std::string& soundName);

	//���ˏI��
	void CreateShotEnd(void);

};

