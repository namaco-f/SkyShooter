#pragma once

#include <string>
#include <unordered_map>
#include "AnimationData.h"

/// <summary>
/// ���f���̃A�j���[�V��������
/// </summary>
class AnimationController
{

public:

	AnimationController(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="modelId">�A�j���[�V�����̐�������������f���̃n���h��ID</param>
	explicit AnimationController(const int& modelId);
	~AnimationController(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �A�j���[�V�����̒ǉ�
	/// </summary>
	/// <param name="animKey">�A�j���[�V�����̃L�[</param>
	/// <param name="animData">�A�j���[�V�����f�[�^</param>
	void AddAnimationData(const std::string& animKey,const AnimData& animData);

	/// <summary>
	/// �w�肵���A�j���[�V�����̍Đ��J�n
	/// </summary>
	/// <param name="animKey">�w�肷��A�j���[�V�����̃L�[</param>
	void Play(const std::string& animKey);

	/// <summary>
	/// �w�肵���A�j���[�V�����̒�~
	/// </summary>
	/// <param name="animKey">�w�肷��A�j���[�V�����̃L�[</param>
	void Stop(const std::string& animKey);

	/// <summary>
	/// �A�^�b�`����Ă���S�ẴA�j���[�V�����̍Đ�
	/// </summary>
	void AllPlay(void);

	/// <summary>
	/// �A�^�b�`����Ă���S�ẴA�j���[�V�����̍Đ�
	/// </summary>
	void AllStop(void);
	


	/// <summary>
	/// ���݂̃A�^�b�`����Ă���A�j���[�V�����̐����擾
	/// </summary>
	/// <returns>attachNum_</returns>
	inline const int GetAnimAttachNum(void) const
	{
		return attachNum_;
	}

	/// <summary>
	/// �o�^����Ă���A�j���[�V�����f�[�^��S�Ď擾
	/// </summary>
	/// <returns>animationDatasMap_</returns>
	inline const std::unordered_map<std::string, AnimData>& GetAnimationDatas(void) const
	{
		return animationDatasMap_;
	}

	/// <summary>
	/// �w�肵���A�j���[�V�����̃A�j���[�V�����f�[�^���擾
	/// </summary>
	/// <returns>animationDatasMap_</returns>
	const AnimData& GetAnimationDatas(const std::string& animName) const;

	const bool IsPlayAnim(const std::string& animName) const;

private:

	//���f���̃n���h��
	const int& modelHandle_;

	//��̃A�j���[�V�����f�[�^
	const AnimData nullAnimData_ = AnimData();

	//�A�^�b�`���Ă���A�j���[�V�����̐�
	int attachNum_;

	//�o�^���ꂽ�A�j���[�V�����f�[�^
	std::unordered_map<std::string, AnimData> animationDatasMap_;

	//1�O�̃A�j���[�V�����̖��O
	std::string preAnimName_;

	//���݂̃A�j���[�V�����̖��O
	std::string nowAnimName_;

	//�A�^�b�`����
	void Attach(AnimData& anim);

	//�f�^�b�`����
	void Detach(AnimData& anim);

	//�u�����h���[�g�̌v�Z����
	const float CalculateBlendRate(void);

	//�A�j���[�V�����̍Đ�����
	void PlayAnimation(const float blendRate);

};

