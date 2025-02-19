
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
		//エフェクトを停止
		StopEffect3D(effectData.second.playId);
	}

	effectsData_.clear();
}

void EffectController::Update(const std::string& effectKeyName)
{
	if (IsEffectNameKeyError(effectKeyName))
	{
		//エフェクトが追加されていない
		return;
	}

	auto& effectData = effectsData_.at(effectKeyName);
	
	if (!effectData.IsPartPlayEffect())
	{
		//一部再生ではない場合は終了
		return;
	}

	EffectPlayData effectPlayData = effectData.effectPlayData;

	//エフェクシアのエフェクトデータを取得
	const auto& effekseer = GetEffekseer3DManager();
	const auto& effekObj = GetEffekseerEffect(effectData.resId);

	//カウントを進める
	effectPlayData.step += 1.0f * TimeManager::GetInstance().GetDeltaTime();

	if (effectPlayData.IsEndTime())
	{
		//一部再生の終了時間
		Stop(effectKeyName);

		if (effectPlayData.isLoop) 
		{
			//ループ再生
			PartPlay(effectData);
		}
	}

	//エフェクト情報更新
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
		//エフェクトが追加されていない
		return;
	}

	auto& effectData = effectsData_.at(effectKeyName);

	//一部再生か途中から最後まで再生かを判別
	(effectData.IsPartPlayEffect() || effectData.IsPlayInMiddle())
		? PartPlay(effectData) : NotPartPlay(effectData);
}

void EffectController::Stop(const std::string& effectKeyName)
{
	if (IsEffectNameKeyError(effectKeyName))
	{
		//エフェクトが追加されていない
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
		//存在しない
		return false;
	}
	const auto& effectData = effectsData_.at(effectKeyName);

	//一部再生とそれ以外を判別
	return effectData.IsPartPlayEffect()
		? effectData.effectPlayData.IsEndTime()
		: IsEffekseer3DEffectPlaying(effectData.playId) == -1;
}

void EffectController::FollowPos(EffectData& effectData)
{
	EffectTransform effectTransform = effectData.effectTransform;

	//回転を合成
	Quaternion qua = effectTransform.quaRot.Mult(effectTransform.localQuaRot);

	//ローカル座標を回転
	VECTOR localPos = qua.PosAxis(effectTransform.localPos);

	effectData.effectTransform.pos = VAdd(effectTransform.pos, localPos);
}

void EffectController::FollowTransformUpdate(const std::string& effectKeyName, const Transform& transform)
{
	if (IsEffectNameKeyError(effectKeyName))
	{
		//エフェクトが追加されていない
		return;
	}

	const auto& effectData = effectsData_.at(effectKeyName);

	EffectTransform effect = EffectTransform();

	VECTOR pos = transform.pos;

	Quaternion qua = transform.quaRot;

	VECTOR localPos = qua.PosAxis(effectData.effectTransform.localPos);

	effect.pos = VAdd(pos, localPos);

	//回転と座標を反映
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

	//オイラー角に変換
	const VECTOR rot = Quaternion::ToEuler(qua);

	//デグリー角に変換
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

	//座標の更新
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
	//Effekseerのインスタンスの取得
	const auto& effekseer = GetEffekseer3DManager();

	const EffectTransform& effectTransform = effectData.effectTransform;

	const EffectPlayData& effctPlayData = effectData.effectPlayData;

	//座標の更新
	FollowPos(effectData);
	const VECTOR pos = effectTransform.pos;

	//Effeckseerからエフェクト本体を取得
	auto effect = GetEffekseerEffect(effectData.resId);

	//座標と再生時間を指定して再生
	effectData.playId = effekseer->Play(
		effect, Effekseer::Vector3D(pos.x, pos.y, pos.z), effctPlayData.startTime);

	SetScalePlayEffect3D(effectData.playId, effectTransform.scale);
	SetRotatePlayEffect3D(
		effectData.playId,
		effectTransform.quaRot,
		effectTransform.localQuaRot
	);
}
