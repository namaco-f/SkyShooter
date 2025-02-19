#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "ColliderTag.h"
#include "../Transform.h"

/// <summary>
/// 衝突判定の情報を保持するクラス
/// </summary>
class Collider
{

public:

	/// <summary>
	/// コライダ情報の構造体
	/// </summary>
	struct ColliderData
	{
		Transform transform_;			//モデル制御情報
		Transform preTransform_;		//1f前のモデル制御情報
		COLLISION_TYPE colType_;		//衝突判定の種類
		COLLISION_SUBTYPE subType_;		//オブジェクトのフレーム名
		int colModelFrameIdx_;			//フレーム番号
		bool ableCol_;					//衝突判定が有効か

		//VECTOR localPos;

		float radius_;				//半径
		VECTOR localPosTop_;		//上端ローカル座標
		VECTOR localPosBottom_;		//下端ローカル座標
		VECTOR localPosCenter_;		//中心座標
		Quaternion localQuaRot_;

		ColliderData() :
			transform_(Transform()),
			preTransform_(Transform()),
			subType_(COLLISION_SUBTYPE::NONE),
			colType_(COLLISION_TYPE::NONE),
			colModelFrameIdx_(-1),
			ableCol_(false),
			radius_(0.0f),
			localPosTop_{},
			localPosBottom_{},
			localPosCenter_{},
			localQuaRot_(Quaternion())
		{};
	};

	Collider(void) = default;
	~Collider(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="colData">コライダ情報</param>
	explicit Collider(const ColliderData& colData);

	/// <summary>
	/// コライダの更新
	/// </summary>
	/// <param name="colData">コライダ情報</param>
	void UpdateCollider(const ColliderData& colData);

	/// <summary>
	/// コライダ内のモデル制御情報を更新
	/// </summary>
	/// <param name="transform">モデル制御情報</param>
	void UpdateTransform(const Transform& transform);

	/// <summary>
	/// コライダ内のモデル制御情報を更新
	/// </summary>
	/// <param name="transform">モデル制御情報</param>
	void UpdateTransformQuaRot(const Quaternion& quaternion);

	/// <summary>
	/// コライダのローカル座標を設定
	/// </summary>
	void SetLocalPos(const VECTOR& localPos);

	/// <summary>
	/// 3Dコライダ描画
	/// </summary>
	void Draw(void);

	/// <summary>
	/// モデル制御情報の座標を取得
	/// </summary>
	inline const VECTOR& GetTrasformPos(void) const
	{
		return colData_.transform_.pos;
	}

	/// <summary>
	/// 当たり判定の種類を取得
	/// </summary>
	inline const COLLISION_TYPE GetColType(void) const
	{
		return colData_.colType_;
	}

	/// <summary>
	/// 衝突判定を行っているモデルのハンドルIDを取得
	/// </summary>
	inline const int GetModelId(void) const
	{
		return colData_.transform_.modelId_;
	}

	/// <summary>
	/// 追従フレーム番号を取得
	/// </summary>
	inline const int GetFrameIdx(void) const
	{
		return colData_.colModelFrameIdx_;
	}

	/// <summary>
	/// 衝突判定を行うかのフラグを取得
	/// </summary>
	inline const bool GetAbleCol(void) const
	{
		return colData_.ableCol_;
	}

	/// <summary>
	/// コライダ情報を取得
	/// </summary>
	inline const ColliderData& GetColliderData(void) const
	{
		return colData_;
	}

	/// <summary>
	/// 追従フレーム名を取得
	/// </summary>
	inline const COLLISION_SUBTYPE GetObjSubType(void) const
	{
		return colData_.subType_;
	}

	/// <summary>
	/// 上端のワールド座標を取得
	/// </summary>
	VECTOR GetPosTop(void) const;

	/// <summary>
	/// 下端のワールド座標を取得
	/// </summary>
	VECTOR GetPosDown(void) const;
	
	//ワールド座標を取得
	const VECTOR& GetPos(void) const;

	/// <summary>
	/// 中心のワールド座標を取得
	/// </summary>
	inline const VECTOR& GetCenterPos(void) const
	{
		return colData_.localPosCenter_;
	}

	/// <summary>
	/// 半径を取得
	/// </summary>
	inline const float GetRadius(void) const
	{
		return colData_.radius_;
	}

	/// <summary>
	/// 上端のローカル座標を設定
	/// </summary>
	/// <param name="val">上端のローカル座標</param>
	void SetPosTop(const VECTOR& val);
	
	/// <summary>
	/// 下端のローカル座標を設定
	/// </summary>
	/// <param name="val">下端のローカル座標</param>
	void SetPosDown(const VECTOR& val);
	
	/// <summary>
	/// 中心座標を設定
	/// </summary>
	/// <param name="val">中心座標</param>
	void SetPosCenter(const VECTOR& val);

	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="val">半径</param>
	void SetRadius(const float& val);

	/// <summary>
	/// フレーム番号を設定
	/// </summary>
	/// <param name="val">フレーム番号</param>
	void SetFrameIndex(const int& val);

	/// <summary>
	/// 当たり判定を有効にするか設定
	/// </summary>
	/// <param name="val">
	/// 当たり判定の有無
	/// </param>
	void SetColAble(const bool val);
	
	
	/// <summary>
	/// 相対座標を回転させてワールド座標で取得する
	/// </summary>
	/// <param name="localPos">相対座標</param>
	/// <returns>回転後の相対座標のワールド座標</returns>
	VECTOR GetRotPos(const VECTOR& localPos) const;

	/// <summary>
	/// ローカル座標の取得
	/// </summary>
	inline const VECTOR& GetLocalPos(void) const
	{
		return localPos_;
	}


private:

	//ローカル座標
	VECTOR localPos_;

	//コライダ情報の構造体
	ColliderData colData_;

	//コライダの種類に合わせた描画
	void InitColTypeDraw(void);

	//コライダの種類をセット
	void SetColTypeDraw(const COLLISION_TYPE colType);

	//コライダごとの遷移状態の状態
	std::unordered_map<COLLISION_TYPE, std::function<void(void)>> colTypeChange_;
	void Set_Sphere(void);			//球体
	void Set_Capsule(void);			//カプセル
	void Set_Model(void);			//モデル
	void Set_Line(void);			//線分
	void Set_Cylinder(void);		//

	//コライダごとの描画
	std::function<void(void)> colTypeDraw_;
	void Draw_Sphere(void);			//球体
	void Draw_Capsule(void);		//カプセル
	void Draw_Model(void);			//モデル
	void Draw_Line(void);			//線
	void Draw_Cylinder(void);		//

};

