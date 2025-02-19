#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../Object/Actor/Shot/ShotBase.h"
#include "../Singleton.h"
#include "../Object/ObjectTag.h"

class ShotBase;

/// <summary>
/// �e�̊Ǘ�
/// </summary>
class ShotManager
	: public Singleton<ShotManager>
{
public:

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �e�̓o�^
	/// </summary>
	void AddShot(std::function<std::unique_ptr<ShotBase>()> createFunc);

	/// <summary>
	/// �e�z��̎擾
	/// </summary>
	/// <returns>shots_�̎Q��</returns>
	inline const std::list<std::unique_ptr<ShotBase>>& GetShots(void) const
	{
		return shots_;
	}

	/// <summary>
	/// �e�̏���
	/// </summary>
	inline void ClearShots(void)
	{
		if (shotsNums_.empty() && shots_.empty())
		{
			return;
		}

		shots_.clear();
		shotsNums_.clear();
	}

private:

	friend class Singleton<ShotManager>;

	//�e
	std::list<std::unique_ptr<ShotBase>> shots_;
	
	//�e�̐��J�E���g�p�z��
	std::unordered_map<OBJECT_TYPE, int> shotsNums_;

	//��
	const std::unique_ptr<ShotBase> nullShot_ = nullptr;
	
	ShotManager(void);
	virtual ~ShotManager(void) override;
};

