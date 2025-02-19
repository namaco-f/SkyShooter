#pragma once

#include <functional>
#include "../Common/Transform.h"
#include "../ObjectTag.h"
#include "Message.h"
#include "../Observer/Message.h"

class ResourceManager;
class ModelResourcesManager;
class ImageResourcesManager;
class EffectResourcesManager;
class SoundResourcesManager;
class EffectController;
class SoundController;
class ShaderManager;
class UIDataManager;
class Collider;
class Collider2D;
class Observer;

/// <summary>
/// オブジェクト基底クラス
/// </summary>
class ActorBase
{

public:
	ActorBase(void);
	virtual ~ActorBase(void);

	virtual void Load(void) final;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(void) final;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// 衝突の通知
	/// </summary>
	/// <param name="obj">衝突したオブジェクト</param>
	virtual void IsHitNotice(const OBJECT_TYPE obj);

	virtual void ReceiveCommonMessage(const Common_Message message);

	/// <summary>
	/// モデル制御情報の取得
	/// </summary>
	/// <returns>transform_の参照</returns>
	inline const Transform& GetTransform(void) const
	{
		return transform_;
	}

	/// <summary>
	/// オブジェクトタグを取得
	/// </summary>
	/// <returns>objType_の参照</returns>
	inline const OBJECT_TYPE& GetObjectType(void) const
	{
		return objType_;
	}

	/// <summary>
	/// 衝突判定情報の取得
	/// </summary>
	/// <returns>collider_の参照</returns>
	inline const Collider& GetCollider(void) const
	{
		return *collider_;
	}	

	/// <summary>
	/// 衝突判定情報の取得
	/// </summary>
	/// <returns>collider_の参照</returns>
	inline const Collider& GetColliderFoot(void) const
	{
		return *colliderFoot_;
	}
	
	/// <summary>
	/// 衝突判定情報の取得
	/// </summary>
	/// <returns>collider_の参照</returns>
	inline const Collider2D& GetCollider2D(void) const
	{
		return *collider2D_;
	}

	virtual const bool IsAlive(void) const;


protected:

	//リソース管理
	ResourceManager& resMng_;

	ModelResourcesManager& resMngModel_;
	ImageResourcesManager& resMngImage_;
	EffectResourcesManager& resMngEffect_;
	SoundResourcesManager& resMngSound_;
	ShaderManager& resMngShader_;
	UIDataManager& uiDataMng_;
	
	Observer& observer_;

	//衝突判定
	std::unique_ptr<Collider> collider_;
	std::unique_ptr<Collider> colliderFoot_;
	std::unique_ptr<Collider2D> collider2D_;

	std::unique_ptr<EffectController> effectController_;

	std::unique_ptr<SoundController> soundController_;

	//モデル制御用クラス
	Transform transform_;
	const Transform nullTransform_ = Transform();

	//オブジェクトタグ
	OBJECT_TYPE objType_;

	//移動速度
	float moveSpeed_;

	//初期の移動速度
	float initSpeed_;

	//キャラクターの方向
	VECTOR actorDir_;
	
	//移動方向
	VECTOR moveDir_;

	//最終的に移動する座標
	VECTOR movedPos_;

	//1f前の座標
	VECTOR prePos_;

	bool isGround_;
	
	//初期化時の読込処理
	virtual void InitLoad(void) = 0;

	//モデル制御情報の初期化
	virtual void InitTransform(void) = 0;

	//衝突判定の初期化
	virtual void InitCollider(void);

	//レンダーの初期化
	virtual void InitRender(void);

	//全ての初期化が終了時に行われる初期化処理
	virtual void InitEnd(void);

	//エフェクト初期化
	virtual void InitEffect(void);

	//サウンド初期化
	virtual void InitSound(void);

	//衝突したオブジェクトに応じて呼ばれる関数
	std::unordered_map<OBJECT_TYPE, std::function<void(void)>> isHitUpdate_;

	//isHitUpdate_の登録
	virtual void InitIsHitUpdate(void) = 0;

	//汎用メッセージを受け取った際に行われる関数
	std::unordered_map<Common_Message, std::function<void(void)>> commonMessageUpdate_;

	std::unordered_map<Message, std::function<void(void)>> messageUpdate_;

	//commonMessageUpdate_の登録
	virtual void InitCommonMessageUpdate(void);

	virtual void Message_Landing(void);
	virtual void Message_NotLanding(void);

	virtual void UpdateObserver(void) final;

	virtual void UpdateObserver_SoundVolumeChange(void);

	virtual void InitUpdateObserver(void);
	

	/// <summary>
	/// 重力を無効化
	/// </summary>
	/// <returns>
	/// true : 無効化
	/// false : 処理なし、無効化されていない
	/// </returns>
	virtual const bool InvalidGravity(void) const;

private:

};

