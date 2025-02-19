#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "../Singleton.h"
#include "../Object/ObjectTag.h"

class ActorBase;

/// <summary>
/// Charactor��e�ȊO��Actor���Ǘ�
/// </summary>
class ActorManager
	: public Singleton<ActorManager>
{
public:

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// �L�����N�^�[�z��̎擾
	/// </summary>
	/// <returns>�L�����N�^�[�z��̎Q��</returns>
	inline const std::list<std::shared_ptr<ActorBase>>& GetActors(void) const
	{
		return actors_;
	}

	/// <summary>
	/// �w�肵���L�����N�^�[���擾
	/// </summary>
	/// <returns>�w�肵���L�����N�^�[���擾</returns>
	template<class T>
	const std::shared_ptr<ActorBase>& GetActor(const OBJECT_TYPE obj) const
	{
		//����̕����I�u�W�F�N�g�ɔ�Ή�
		for (const auto& actor : actors_)
		{
			if (actor)
			{
				if (actor->GetObjectType() == obj)
				{
					return actor;
				}
			}
		}

		return nullActor_;
	}

	/// <summary>
	/// �w�肵���L�����N�^�[���擾 : �����p
	/// </summary>
	/// <returns>�w�肵���L�����N�^�[���擾 : �����p</returns>
	template<class T>
	const std::list<std::weak_ptr<ActorBase>> GetActors(const OBJECT_TYPE obj)
	{
		std::list<std::weak_ptr<ActorBase>> ret;

		//����̕����I�u�W�F�N�g�ɑΉ�
		for (const auto& actor : actors_)
		{
			if (actor)
			{
				if (actor->GetObjectType() == obj)
				{
					ret.emplace_back(actor);
				}
			}
		}
		return ret;
	}

	/// <summary>
	/// �L�����N�^�[�̒ǉ�
	/// </summary>
	/// <param name="createFunc">
	/// �Ώۂ̃L�����N�^�[��Create�֐�
	/// </param>
	void AddActor(std::function<std::shared_ptr<ActorBase>()> createFunc);

	inline void ClearActors(void)
	{
		if (actorsNums_.empty() && actors_.empty())
		{
			return;
		}

		actors_.clear();
		actorsNums_.clear();
	}

private:

	friend class Singleton<ActorManager>;

	//�A�N�^�[��
	std::list<std::shared_ptr<ActorBase>> actors_;

	//�A�N�^�[���J�E���g�p�z��
	std::unordered_map<OBJECT_TYPE, int> actorsNums_;

	//��A�N�^�[
	const std::shared_ptr<ActorBase> nullActor_ = nullptr;

	ActorManager(void);
	virtual ~ActorManager(void) override;

};

