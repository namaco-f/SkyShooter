#pragma once
#include <unordered_map>
#include <string>
#include "../Object/Common/Transform.h"

/// <summary>
/// �G�t�F�N�g���W������
/// </summary>
struct EffectTransform
{
	VECTOR pos;					//���W
	VECTOR localPos;			//���[�J�����W
	VECTOR scale;				//�傫��
	Quaternion quaRot;			//��]
	Quaternion localQuaRot;		//���[�J����]

	EffectTransform(void)
		:
		pos{},
		localPos{},
		scale({1.0f,1.0f,1.0f}),
		quaRot{},
		localQuaRot{}
	{};
};

/// <summary>
/// �G�t�F�N�g�Đ��p���
/// </summary>
struct EffectPlayData
{

	int startTime;			//�Đ��J�n����			
	int endTime;			//�Đ��I������
	float step;				//�J�E���g�p
	bool isPartPlay;		//�ꕔ�Đ����g�p���邩
	bool isLoop;			//���[�v���邩
	bool PlayInMiddle;		//�r������Đ����A�Ō�܂ōĐ����邩

	EffectPlayData(void)
		:
		startTime(0),
		endTime(0),
		step(0.0f),
		isPartPlay(false),
		isLoop(false),
		PlayInMiddle(false)
	{};

	/// <summary>
	/// �ꕔ�Đ��p�̃R���X�g���N�^
	/// </summary>
	/// <param name="start">�J�n���鎞��</param>
	/// <param name="end">�I�����鎞��</param>
	/// <param name="isLoop">���[�v���邩</param>
	explicit EffectPlayData(const int start,const int end = -1,const bool isLoop = false)
		:
		startTime(start),
		endTime(end),
		step(0.0f),
		isPartPlay(true),
		isLoop(isLoop),
		PlayInMiddle(IsSetPlayInMiddle())
	{};

	/// <summary>
	/// �Đ����I�����Ă��邩
	/// </summary>
	/// <returns>
	/// true:�I�����Ă���
	/// false:�I�����Ă��Ȃ�
	/// </returns>
	const bool IsEndTime(void) const
	{
		return endTime < startTime + (int)step;
	}

	/// <summary>
	/// �r���Đ����A�Ō�܂ōĐ����邩	
	/// </summary>
	const bool IsSetPlayInMiddle(void) const
	{
		//�Đ��I�����Ԃ��ݒ肳��Ă��邩�m�F
		//true : �r���ōĐ��I��
		//false : �Ō�܂ōĐ�
		return endTime != -1 ? false : true;
	}
};

/// <summary>
/// �G�t�F�N�g���
/// </summary>
struct EffectData
{
	int resId;							//�G�t�F�N�g�n���h��
	int playId;							//�G�t�F�N�g�̍Đ��p�n���h��
	EffectPlayData effectPlayData;		//�G�t�F�N�g�Đ��p�̏��
	EffectTransform effectTransform;	//�G�t�F�N�g���W������

	EffectData(void)
		:
		resId(-1),
		playId(-1),
		effectPlayData(EffectPlayData()),
		effectTransform(EffectTransform())
	{};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="transform">�G�t�F�N�g���W������</param>
	/// <param name="effectPlayData">�G�t�F�N�g�Đ��p���</param>
	/// <param name="PartPlay">�ꕔ�Đ����邩</param>
	explicit EffectData(
		const EffectTransform& transform,
		const EffectPlayData& effectPlayData = EffectPlayData(),
		const bool PartPlay = false)
		:
		resId(-1),
		playId(-1),
		effectPlayData(effectPlayData),
		effectTransform(transform)
	{};

	/// <summary>
	/// ���W���擾
	/// </summary>
	const VECTOR& GetPos(void) const
	{
		return effectTransform.pos;
	}

	/// <summary>
	/// �r������Đ����A�Ō�܂ōĐ����邩
	/// </summary>
	const bool IsPlayInMiddle(void) const
	{
		return effectPlayData.PlayInMiddle;
	}

	/// <summary>
	/// �ꕔ�Đ���
	/// </summary>
	const bool IsPartPlayEffect(void) const
	{
		//�ꕔ�Đ��A�Ō�܂ōĐ����Ȃ�
		return effectPlayData.isPartPlay && !IsPlayInMiddle();
	}
};

/// <summary>
/// �G�t�F�N�g�̕`�搧��
/// </summary>
class EffectController
{

public:

	EffectController(void);
	virtual ~EffectController(void);

	/// <summary>
	/// �ꕔ�Đ����̂ݍX�V����
	/// </summary>
	virtual void Update(const std::string& effectKeyName);

	/// <summary>
	/// �G�t�F�N�g�ǉ�
	/// </summary>
	/// <param name="effectKeyName">�G�t�F�N�g�̖��O</param>
	/// <param name="handleId">�G�t�F�N�g�̃n���h��ID</param>
	/// <param name="effectTransform">�G�t�F�N�g�̐�����</param>
	virtual void Add(
		const std::string& effectKeyName, 
		const int handleId, 
		const EffectTransform& effectTransform,
		const EffectPlayData& effectPlayData = EffectPlayData()
	);

	/// <summary>
	/// �w�肵���G�t�F�N�g�̕`��
	/// </summary>
	/// <param name="effectKeyName">�G�t�F�N�g�̖��O</param>
	virtual void Draw(
		const std::string& effectKeyName);

	/// <summary>
	/// �w�肵���G�t�F�N�g���~
	/// </summary>
	/// <param name="effectKeyName">�G�t�F�N�g�̖��O</param>
	virtual void Stop(const std::string& effectKeyName);

	/// <summary>
	/// �w�肵���G�t�F�N�g���~
	/// </summary>
	/// <param name="effectKeyName">�G�t�F�N�g�̖��O</param>
	virtual const bool IsEffectEnd(const std::string& effectKeyName);

	/// <summary>
	/// �G�t�F�N�g���Ǐ]���Ă�����W�����X�V
	/// </summary>
	/// <param name="effectKeyName">�G�t�F�N�g�̖��O</param>
	/// <param name="transform">�G�t�F�N�g���Ǐ]���Ă���Ώۂ̐�����</param>
	virtual void FollowTransformUpdate(
		const std::string& effectKeyName,
		const Transform& transform);

private:

	//�G�t�F�N�g���
	std::unordered_map<std::string, EffectData> effectsData_;

	//�G�t�F�N�g���~
	virtual void StopEffect3D(const int playId);

	//���W���G�t�F�N�g�ɔ��f
	virtual void SetPosPlayEffect3D(const int playId, const VECTOR& pos);
	
	//��]���G�t�F�N�g�ɔ��f
	virtual void SetRotatePlayEffect3D(const int playId, const Quaternion& quaRot,const Quaternion& quaRotLocal);
	
	//�傫�����G�t�F�N�g�ɔ��f
	virtual void SetScalePlayEffect3D(const int playId, const VECTOR& scale);

	//�G�t�F�N�g���o�^����Ă��Ȃ�
	virtual const bool IsEffectNameKeyError(const std::string& effectKeyName) const;

	//�w��̃G�t�F�N�g�������Đ��ł͂Ȃ��ꍇ�̍Đ�����
	virtual void NotPartPlay(EffectData& effectData);

	//�w��̃G�t�F�N�g�������Đ��̏ꍇ�̍Đ�����
	virtual void PartPlay(EffectData& effectData);

	/// <summary>
	/// �G�t�F�N�g�̍��W���X�V
	/// </summary>
	/// <param name="effectData">�G�t�F�N�g���</param>
	virtual void FollowPos(EffectData& effectData);
};

