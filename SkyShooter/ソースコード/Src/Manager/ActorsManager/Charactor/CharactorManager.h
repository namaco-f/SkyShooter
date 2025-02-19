#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../Singleton.h"
#include "../Object/ObjectTag.h"
//#include "../Object/Actor/Charactor/CharactorBase.h"

class CharactorBase;

/// <summary>
/// �L�����N�^�[�Ǘ��N���X
/// </summary>
class CharactorManager
	:public Singleton<CharactorManager>
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
	inline const std::vector<std::shared_ptr<CharactorBase>>& GetCharactors(void) const
	{
		return charactors_;
	}

	/// <summary>
	/// �w�肵���L�����N�^�[�̐����擾
	/// </summary>
	/// <param name="objTag">�w�肷��L�����N�^�[�̃^�O</param>
	/// <returns>�w�肵���L�����N�^�[�̐�</returns>
	const int GetCharactorNum(const OBJECT_TYPE objTag) const;

	/// <summary>
	/// �w�肵���L�����N�^�[���擾
	/// </summary>
	/// <returns>�w�肵���L�����N�^�[���擾</returns>
	template<class T>
	const std::shared_ptr<CharactorBase>& GetCharactor(const OBJECT_TYPE obj) const
	{
		//����̕����I�u�W�F�N�g�ɔ�Ή�
		for (const auto& charactor : charactors_)
		{
			if (charactor)
			{
				if (charactor->GetObjectType() == obj)
				{
					return charactor;
				}
			}
		}

		return nullCharactor_;
	}

	/// <summary>
	/// �w�肵���L�����N�^�[���擾 : �����p
	/// </summary>
	/// <returns>�w�肵���L�����N�^�[���擾 : �����p</returns>
	template<class T>
	const std::list<std::weak_ptr<CharactorBase>> GetCharactors(const OBJECT_TYPE obj)
	{
		std::list<std::weak_ptr<CharactorBase>> ret;
		
		//����̕����I�u�W�F�N�g�ɑΉ�
		for (const auto& charactor : charactors_)
		{
			if (charactor)
			{
				if (charactor->GetObjectType() == obj)
				{
					ret.emplace_back(charactor);
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
	void AddCharactor(std::function<std::shared_ptr<CharactorBase>()> createFunc);

	/// <summary>
	/// �L�����N�^�[�̍폜
	/// </summary>
	inline void ClearCharactors(void)
	{
		if (charactorsNums_.empty() && charactors_.empty())
		{
			return;
		}

		charactors_.clear();
		charactorsNums_.clear();
	}

private:

	friend class Singleton<CharactorManager>;

	//�L�����N�^�[ : ���ǑS���񂷈ׁAlist�ɂ��ׂ�
	std::vector<std::shared_ptr<CharactorBase>> charactors_;

	//�L�����N�^�[���J�E���g�p�z��
	std::unordered_map<OBJECT_TYPE,int> charactorsNums_;

	//��L�����N�^�[
	const std::shared_ptr<CharactorBase> nullCharactor_ = nullptr;

	CharactorManager(void);
	virtual ~CharactorManager(void) override;
};

