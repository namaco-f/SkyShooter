
#include <EffekseerForDXLib.h>
#include "../Manager/TimeManager.h"
#include "EffectController.h"
#include "../Utility/MathUtility.h"

EffectController::EffectController(void)
	:
	effectsData_{}
{
}

EffectController::~EffectController(void)
{
	for (const auto& effectData : effectsData_)
	{
		//�G�t�F�N�g���~
		StopEffect3D(effectData.second.playId);
	}

	effectsData_.clear();
}

void EffectController::Update(const std::string& effectKeyName)
{
	if (IsEffectNameKeyError(effectKeyName))
	{
		//�G�t�F�N�g���ǉ�����Ă��Ȃ�
		return;
	}

	auto& effectData = effectsData_.at(effectKeyName);
	
	if (!effectData.IsPartPlayEffect())
	{
		//�ꕔ�Đ��ł͂Ȃ��ꍇ�͏I��
		return;
	}

	EffectPlayData effectPlayData = effectData.effectPlayData;

	//�G�t�F�N�V�A�̃G�t�F�N�g�f�[�^���擾
	const auto& effekseer = GetEffekseer3DManager();
	const auto& effekObj = GetEffekseerEffect(effectData.resId);

	//�J�E���g��i�߂�
	effectPlayData.step += 1.0f * TimeManager::GetInstance().GetDeltaTime();

	if (effectPlayData.IsEndTime())
	{
		//�ꕔ�Đ��̏I������
		Stop(effectKeyName);

		if (effectPlayData.isLoop) 
		{
			//���[�v�Đ�
			PartPlay(effectData);
		}
	}

	//�G�t�F�N�g���X�V
	effectData.effectPlayData = effectPlayData;
}

void EffectController::Add(const std::string& effectKeyName, const int handleId, const EffectTransform& effectTransform, const EffectPlayData& effectPlayData)
{
	EffectData effectData = EffectData();

	effectData.resId = handleId;
	effectData.effectTransform = effectTransform;
	effectData.effectPlayData = effectPlayData;

	effectsData_.emplace(effectKeyName, effectData);
}

void EffectController::Draw(const std::string& effectKeyName)
{
	if (IsEffectNameKeyError(effectKeyName))
	{
		//�G�t�F�N�g���ǉ�����Ă��Ȃ�
		return;
	}

	auto& effectData = effectsData_.at(effectKeyName);

	//�ꕔ�Đ����r������Ō�܂ōĐ����𔻕�
	(effectData.IsPartPlayEffect() || effectData.IsPlayInMiddle())
		? PartPlay(effectData) : NotPartPlay(effectData);
}

void EffectController::Stop(const std::string& effectKeyName)
{
	if (IsEffectNameKeyError(effectKeyName))
	{
		//�G�t�F�N�g���ǉ�����Ă��Ȃ�
		return;
	}

	StopEffect3D(effectsData_.at(effectKeyName).playId);
	effectsData_.at(effectKeyName).playId = -1;
	effectsData_.at(effectKeyName).effectPlayData.step = 0.0f;
}

const bool EffectController::IsEffectEnd(const std::string& effectKeyName)
{
	if (!effectsData_.contains(effectKeyName))
	{
		//���݂��Ȃ�
		return false;
	}
	const auto& effectData = effectsData_.at(effectKeyName);

	//�ꕔ�Đ��Ƃ���ȊO�𔻕�
	return effectData.IsPartPlayEffect()
		? effectData.effectPlayData.IsEndTime()
		: IsEffekseer3DEffectPlaying(effectData.playId) == -1;
}

void EffectController::FollowPos(EffectData& effectData)
{
	EffectTransform effectTransform = effectData.effectTransform;

	//��]������
	Quaternion qua = effectTransform.quaRot.Mult(effectTransform.localQuaRot);

	//���[�J�����W����]
	VECTOR localPos = qua.PosAxis(effectTransform.localPos);

	effectData.effectTransform.pos = VAdd(effectTransform.pos, localPos);
}

void EffectController::FollowTransformUpdate(const std::string& effectKeyName, const Transform& transform)
{
	if (IsEffectNameKeyError(effectKeyName))
	{
		//�G�t�F�N�g���ǉ�����Ă��Ȃ�
		return;
	}

	const auto& effectData = effectsData_.at(effectKeyName);

	EffectTransform effect = EffectTransform();

	VECTOR pos = transform.pos;

	Quaternion qua = transform.quaRot;

	VECTOR localPos = qua.PosAxis(effectData.effectTransform.localPos);

	effect.pos = VAdd(pos, localPos);

	//��]�ƍ��W�𔽉f
	SetRotatePlayEffect3D(effectData.playId, qua, effectData.effectTransform.localQuaRot);
	SetPosPlayEffect3D(effectData.playId,effect.pos);
}

void EffectController::StopEffect3D(const int playId)
{
	StopEffekseer3DEffect(playId);
}

void EffectController::SetPosPlayEffect3D(const int playId, const VECTOR& pos)
{
	SetPosPlayingEffekseer3DEffect(playId, pos.x, pos.y, pos.z);
}

void EffectController::SetRotatePlayEffect3D(const int playId, const Quaternion& quaRot,const Quaternion& quaRotLocal)
{
	Quaternion qua = quaRotLocal.Mult(quaRot);

	//�I�C���[�p�ɕϊ�
	const VECTOR rot = Quaternion::ToEuler(qua);

	//�f�O���[�p�ɕϊ�
	const VECTOR r = MathUtility::Rad2Deg_VECTOR(rot);

	SetRotationPlayingEffekseer3DEffect(playId, rot.x, rot.y, rot.z);
}

void EffectController::SetScalePlayEffect3D(const int playId, const VECTOR& scale)
{
	SetScalePlayingEffekseer3DEffect(playId, scale.x, scale.y, scale.z);
}

const bool EffectController::IsEffectNameKeyError(const std::string& effectKeyName) const
{
	return !effectsData_.contains(effectKeyName);
}

void EffectController::NotPartPlay(EffectData& effectData)
{
	const EffectTransform& effectTransform = effectData.effectTransform;

	//���W�̍X�V
	FollowPos(effectData);

	effectData.playId = PlayEffekseer3DEffect(effectData.resId);

	SetScalePlayEffect3D(effectData.playId, effectTransform.scale);
	
	SetPosPlayEffect3D(effectData.playId, effectTransform.pos);
	
	SetRotatePlayEffect3D(
		effectData.playId,
		effectTransform.quaRot,
		effectTransform.localQuaRot
	);
}

void EffectController::PartPlay(EffectData& effectData)
{
	//Effekseer�̃C���X�^���X�̎擾
	const auto& effekseer = GetEffekseer3DManager();

	const EffectTransform& effectTransform = effectData.effectTransform;

	const EffectPlayData& effctPlayData = effectData.effectPlayData;

	//���W�̍X�V
	FollowPos(effectData);
	const VECTOR pos = effectTransform.pos;

	//Effeckseer����G�t�F�N�g�{�̂��擾
	auto effect = GetEffekseerEffect(effectData.resId);

	//���W�ƍĐ����Ԃ��w�肵�čĐ�
	effectData.playId = effekseer->Play(
		effect, Effekseer::Vector3D(pos.x, pos.y, pos.z), effctPlayData.startTime);

	SetScalePlayEffect3D(effectData.playId, effectTransform.scale);
	SetRotatePlayEffect3D(
		effectData.playId,
		effectTransform.quaRot,
		effectTransform.localQuaRot
	);
}
