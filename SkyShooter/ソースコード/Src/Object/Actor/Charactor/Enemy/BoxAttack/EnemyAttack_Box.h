#pragma once

#include <memory>
#include "../Common/Vector2/Vector2f.h"
#include "../../../../Common/Transform.h"
#include "../../../ActorBase.h"

class EffectController;

/// <summary>
/// 攻撃情報
/// </summary>
struct BoxAttackData
{
	float attackDelay;     //攻撃判定が有効になるまでの時間
	float totalTime;       //攻撃判定が有効な時間
	float step;            //カウンター変数
	bool attackDelayEnd;

	BoxAttackData(void)
		:
		attackDelay(0.0f),
		totalTime(0.0f),
		step(0.0f),
		attackDelayEnd(false)
	{};

	BoxAttackData(const float delay, const float total)
		:
		attackDelay(delay),
		totalTime(total),
		step(0.0f),
		attackDelayEnd(false)
	{};

	/// <summary>
	/// 攻撃開始までのディレイが終了しているか
	/// </summary>
	const bool IsEndAttackDelayTime(void) const 
	{
		return attackDelay < step;
	}

	/// <summary>
	/// 攻撃が終了しているか
	/// </summary>
	const bool IsEndAttackTotalTime(void) const
	{
		return totalTime < step && attackDelayEnd;
	}
		
	const float attackDelayEndPar(void) const
	{
		if (attackDelay == 0.0f)
		{
			//0除算させない
			return 1.0f;
		}

		return step / attackDelay;
	}
};

/// <summary>
/// 矩形の種類
/// </summary>
enum class BoxType
{
	XY,		//XY平面上
	ZY,		//ZY平面上
	XZ,		//XZ平面上
	MAX		//カウント用
};

/// <summary>
/// 矩形の情報
/// </summary>
struct BoxInfo
{
	VECTOR boxCenterPos;	//中心座標
	Vector2f boxSize;		//横縦サイズ

	Vector2f boxPosTopLeft;			//左上
	Vector2f boxPosTopRight;		//右上
	Vector2f boxPosBottomLeft;		//左下
	Vector2f boxPosBottomRight;		//右下

	BoxInfo(void) : 
		boxCenterPos{}, 
		boxSize{}
	{};

	BoxInfo(const VECTOR& pos, const Vector2f& size)
		:
		boxCenterPos(pos),
		boxSize(size)
	{};
};

/// <summary>
/// 敵の箱型の範囲攻撃の基底クラス
/// </summary>
class EnemyAttack_Box
	: 
	public ActorBase
{

public:

	EnemyAttack_Box() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="transform">モデルの制御情報</param>
	/// <param name="boxSize">縦横サイズ</param>
	/// <param name="boxAttackData">攻撃の情報</param>
	/// <param name="boxType">矩形の種別</param>
	EnemyAttack_Box(
		const Transform& transform,
		const Vector2f& boxSize,
		const BoxAttackData& boxAttackData
	);
	virtual ~EnemyAttack_Box(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void) override;

	/// <summary>
	/// コライダの状態を反転
	/// </summary>
	virtual void SwitchCollider(void);

	/// <summary>
	/// 攻撃情報を取得
	/// </summary>
	/// <returns>boxAttackData_</returns>
	virtual const BoxAttackData& GetBoxAttackData(void) const
	{
		return boxAttackData_;
	}

	/// <summary>
	/// 攻撃のディレイが終わっているか
	/// </summary>
	/// <returns>
	/// true : 終了
	/// false : ディレイ中
	/// </returns>
	virtual inline const bool IsAttackDelayEnd(void) const
	{
		return boxAttackData_.attackDelayEnd;
	}

	/// <summary>
	/// 攻撃が終わっているか
	/// </summary>
	/// <returns>
	/// true : 終了
	/// false : 攻撃継続中
	/// </returns>
	virtual inline const bool IsAttackEnd(void) const
	{
		return boxAttackData_.IsEndAttackTotalTime();
	}

protected:

	//攻撃の情報
	BoxAttackData boxAttackData_;

	//矩形の情報
	BoxInfo boxInfo_;

	//矩形の種別
	BoxType boxType_;
	
	//2Dコライダの生成
	virtual void MakeBoxCollider(void) = 0;

	//ディレイ中の更新処理
	virtual void UpdateAttackDelayTime(void);

	//攻撃中の更新処理
	virtual void UpdateAttackTotalTime(void);

	//初期化時の読込処理
	virtual void InitLoad(void) override;

	//モデル制御情報の初期化
	virtual void InitTransform(void) override;

	//レンダラーの初期化
	virtual void InitRender(void) override;

	//isHitUpdate_の登録
	virtual void InitIsHitUpdate(void) override;

	//箱の座標を取得
	virtual void CalculateBoxPos(void);

	virtual void AttackEffectDraw(void);

	//モデルのスケールを取得
	virtual const VECTOR GetBoxScale(void) = 0 ;

	virtual const VECTOR GetBoxChangeScale(void) = 0;

	//モデルのサイズの調節
	virtual const VECTOR RegulateBoxScale(const VECTOR& val);

	virtual const float LerpBoxScale(const float& max) const;

};

