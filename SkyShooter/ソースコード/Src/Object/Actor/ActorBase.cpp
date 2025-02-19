
#include "../../Manager/ResourceManager/ResourceManager.h"
#include "../../Manager/UIManager/UIDataManager.h"
#include "../../Effect/EffectController.h"
#include "../../Sound/SoundController.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Collider2D.h"
#include "../../Utility/MathUtility.h"
#include "../Observer/Observer.h"
#include "ActorBase.h"

using namespace std;

using ResMng = ResourceManager;
using ObjTag = OBJECT_TYPE;
using Math = MathUtility;

const float INIT_MOVESPEED = Math::FLOAT_ZERO;

ActorBase::ActorBase(void)
	:
	resMng_(ResMng::GetInstance()),
	resMngModel_(resMng_.GetModelResourcesInstance()),
	resMngImage_(resMng_.GetImageResourcesInstance()),
	resMngEffect_(resMng_.GetEffectResourcesInstance()),
	resMngSound_(resMng_.GetSoundResourcesInstance()),
	resMngShader_(resMng_.GetShaderResourcesInstance()),
	observer_(Observer::GetInstance()),
	uiDataMng_(UIDataManager::GetInstance()),
	collider_(nullptr),
	colliderFoot_(nullptr),
	collider2D_(nullptr),
	effectController_(make_unique<EffectController>()),
	soundController_(make_unique<SoundController>()),
	transform_(Transform()),
	objType_(ObjTag::NONE),
	moveSpeed_(INIT_MOVESPEED),
	actorDir_{},
	moveDir_{},
	movedPos_{},
	prePos_{},
	isGround_(true)
{
	InitCommonMessageUpdate();
}

ActorBase::~ActorBase(void)
{
}

void ActorBase::Load(void)
{
	InitLoad();
}

void ActorBase::Init(void)
{
	InitUpdateObserver();

	InitLoad();
	
	InitTransform();

	InitCollider();
	
	InitRender();
	
	InitEnd();
}

void ActorBase::Draw(void)
{
	if (collider_ != nullptr)
	{
		collider_->Draw();
	}
}

void ActorBase::IsHitNotice(const OBJECT_TYPE obj)
{
	//オブジェクトが登録されているか確認
	if (isHitUpdate_.contains(obj))
	{
		if (!isHitUpdate_.at(obj))
		{
			//関数ポインタが使用不可
			return;
		}
		isHitUpdate_.at(obj)();
	}
}

void ActorBase::ReceiveCommonMessage(const Common_Message message)
{
	if (!commonMessageUpdate_.contains(message))
	{
		return;
	}

	commonMessageUpdate_.at(message)();
}

const bool ActorBase::IsAlive(void) const
{
	return true;
}

void ActorBase::InitCollider(void)
{
	//処理なし
}

void ActorBase::InitRender(void)
{
	//処理なし
}

void ActorBase::InitEnd(void)
{
	//処理なし
}

void ActorBase::InitEffect(void)
{
	//処理なし
}

void ActorBase::InitSound(void)
{
	//処理なし
}

void ActorBase::InitCommonMessageUpdate(void)
{
	commonMessageUpdate_.emplace(
		Common_Message::LANDING,
		bind(&ActorBase::Message_Landing,this)
	);
	commonMessageUpdate_.emplace(
		Common_Message::NOT_LANDING,
		bind(&ActorBase::Message_NotLanding, this)
	);
}

void ActorBase::Message_Landing(void)
{
	isGround_ = true;
}

void ActorBase::Message_NotLanding(void)
{
	isGround_ = false;
}

void ActorBase::UpdateObserver(void)
{
	if (messageUpdate_.empty())
	{
		//配列が空
		return;
	}

	std::function<void(void)> update;

	for (const auto& observer : observer_.ReceiveMessageList())
	{
		if (!messageUpdate_.contains(observer))
		{
			//登録されていない
			continue;
		}

		update = messageUpdate_.at(observer);

		if (!update)
		{
			//関数オブジェクトがリンク切れ
			continue;
		}
		update();
	}
}

void ActorBase::InitUpdateObserver(void)
{
}

void ActorBase::UpdateObserver_SoundVolumeChange(void)
{
}

const bool ActorBase::InvalidGravity(void) const
{
	return false;
}
