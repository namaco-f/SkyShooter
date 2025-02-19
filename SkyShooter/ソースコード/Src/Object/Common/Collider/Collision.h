#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include "ColliderTag.h"
#include "../Transform.h"

class Collider;

/// <summary>
/// 衝突判定の計算
/// </summary>
class Collision
{
public:

	//モデル以外との判定時に返す情報
	struct HitColInfo
	{
		//VECTOR hitPos;	//衝突した座標(中心)
		VECTOR hitDir;		//衝突した方向(視点終点ともに中心)
		bool isHit;			//衝突したか

		HitColInfo()
			:
			hitDir{},
			isHit(false)
		{};
	};

	Collision(void);

	virtual ~Collision(void);

	const HitColInfo GetColResultHitColInfo(const Collider& colA, const Collider& colB);

	/// <summary>
	/// 衝突判定結果を返す : モデル用
	/// </summary>
	/// <param name="objA">オブジェクト名</param>
	/// <param name="objB">オブジェクト名</param>
	/// <returns></returns>
	const MV1_COLL_RESULT_POLY GetCollResultPoly_Line(
		const Collider& colA, const Collider& colB);


	/// <summary>
	/// 球とカプセルの当たり判定
	/// </summary>
	/// <param name="colSphere">球コライダー</param>
	/// <param name="colCapsule">カプセルコライダー</param>
	/// <returns>コリジョン結果 HitColInfo</returns>
	const HitColInfo IsHitSphereCapsuleResult(
		const Collider& colA, 
		const Collider& colB
	);

	/// <summary>
	///  球同士の当たり判定
	/// </summary>
	/// <param name="colA">球</param>
	/// <param name="colB">球</param>
	/// <returns>コリジョン結果 HitColInfo</returns>
	const HitColInfo IsHitSpheresResult(
		const Collider& colA,
		const Collider& colB
	);

	/// <summary>
	///  球同士の当たり判定
	/// </summary>
	/// <param name="colA">球</param>
	/// <param name="colB">球</param>
	/// <returns>コリジョン結果 HitColInfo</returns>
	const HitColInfo IsHitCapsulesResult(
		const Collider& colA,
		const Collider& colB
	);

	//ポリゴン情報の削除
	void CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& hitPoly);

	/// <summary>
	/// 衝突判定結果を返す : モデル用
	/// </summary>
	/// <param name="objA">オブジェクト名</param>
	/// <param name="objB">オブジェクト名</param>
	const MV1_COLL_RESULT_POLY GetCollResultPoly_Line(
		const Collider& colA, const VECTOR& lineTop,const VECTOR& lineBottom);

	/// <summary>
	/// 移動制限との判定
	/// </summary>
	/// <param name="movedPos">移動後座標</param>
	void IsCollisionMoveLimit(VECTOR& movedPos);


private:

	//判定結果を格納 : 配列の解放が必要
	MV1_COLL_RESULT_POLY_DIM resultPolyDim_;

	//空の衝突情報
	const HitColInfo nullHitColInfo_ = HitColInfo();

	//判定結果を格納しているか
	bool isStoreResult_;

	/// <summary>
	/// 球とカプセルの当たり判定
	/// </summary>
	/// <param name="colModel">球</param>
	/// <param name="colSphere">カプセル</param>
	/// <returns>コリジョン結果</returns>
	const HitColInfo CollisionSphereCapsule(
		const Collider& colSphere,
		const Collider& colCapsule
	);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="colModel">球</param>
	/// <param name="colSphere">球</param>
	/// <returns>コリジョン結果</returns>
	const HitColInfo CollisionCapsules(
		const Collider& colCapsuleA,
		const Collider& colCapsuleB
	);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="colModel">球</param>
	/// <param name="colSphere">球</param>
	/// <returns>コリジョン結果</returns>
	const HitColInfo CollisionSpheres(
		const Collider& colSphereA,
		const Collider& colSphereB
	);

	///// <summary>
	///// モデルの衝突判定
	///// </summary>
	///// <param name="colModel">モデル</param>
	///// <param name="colA">衝突対象</param>
	///// <returns>コリジョン結果 : MV1_COLL_RESULT_POLY_DIM</returns>
	//const MV1_COLL_RESULT_POLY_DIM& CollisionModel(
	//	std::weak_ptr<Collider> colModel,
	//	std::weak_ptr<Collider> colA
	//);


	/// <summary>
	/// モデルの衝突判定
	/// </summary>
	/// <param name="colModel">線分</param>
	/// <param name="colA">モデル</param>
	/// <returns>コリジョン結果 : MV1_COLL_RESULT_POLY</returns>
	const MV1_COLL_RESULT_POLY CollisionLine(
		const Collider& colLine,
		const Collider& colModel
	) const;

	/// <summary>
	/// モデルの衝突用の関数を取得
	/// </summary>
	/// <param name="colModel">モデル</param>
	/// <param name="colA">衝突対象</param>
	/// <returns>衝突判定用の関数</returns>
	const std::function<MV1_COLL_RESULT_POLY_DIM(void)>GetCollModelFunc(
		Collider& colModel,
		Collider& colA
	) const;

	//移動制限の判定
	void CollisionMoveLimit(float& pos, const float minPos, const float maxPos);

	//衝突判定がカプセル型か判定
	const bool IsCapsule(const Collider& col) const;

	//衝突判定がモデル型か判定
	const bool IsModel(Collider& col) const;

	//衝突判定が球型か判定
	const bool IsSphere(const Collider& col) const;

	//衝突判定が線分型か判定
	const bool IsLine(const Collider& col) const;

	//HitColInfoが適用可能なコライダ種別か
	const bool AbleToGetHitColInfo(const Collider& col) const;

};

