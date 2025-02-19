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
/// �I�u�W�F�N�g���N���X
/// </summary>
class ActorBase
{

public:
	ActorBase(void);
	virtual ~ActorBase(void);

	virtual void Load(void) final;

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init(void) final;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void) = 0;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// �Փ˂̒ʒm
	/// </summary>
	/// <param name="obj">�Փ˂����I�u�W�F�N�g</param>
	virtual void IsHitNotice(const OBJECT_TYPE obj);

	virtual void ReceiveCommonMessage(const Common_Message message);

	/// <summary>
	/// ���f��������̎擾
	/// </summary>
	/// <returns>transform_�̎Q��</returns>
	inline const Transform& GetTransform(void) const
	{
		return transform_;
	}

	/// <summary>
	/// �I�u�W�F�N�g�^�O���擾
	/// </summary>
	/// <returns>objType_�̎Q��</returns>
	inline const OBJECT_TYPE& GetObjectType(void) const
	{
		return objType_;
	}

	/// <summary>
	/// �Փ˔�����̎擾
	/// </summary>
	/// <returns>collider_�̎Q��</returns>
	inline const Collider& GetCollider(void) const
	{
		return *collider_;
	}	

	/// <summary>
	/// �Փ˔�����̎擾
	/// </summary>
	/// <returns>collider_�̎Q��</returns>
	inline const Collider& GetColliderFoot(void) const
	{
		return *colliderFoot_;
	}
	
	/// <summary>
	/// �Փ˔�����̎擾
	/// </summary>
	/// <returns>collider_�̎Q��</returns>
	inline const Collider2D& GetCollider2D(void) const
	{
		return *collider2D_;
	}

	virtual const bool IsAlive(void) const;


protected:

	//���\�[�X�Ǘ�
	ResourceManager& resMng_;

	ModelResourcesManager& resMngModel_;
	ImageResourcesManager& resMngImage_;
	EffectResourcesManager& resMngEffect_;
	SoundResourcesManager& resMngSound_;
	ShaderManager& resMngShader_;
	UIDataManager& uiDataMng_;
	
	Observer& observer_;

	//�Փ˔���
	std::unique_ptr<Collider> collider_;
	std::unique_ptr<Collider> colliderFoot_;
	std::unique_ptr<Collider2D> collider2D_;

	std::unique_ptr<EffectController> effectController_;

	std::unique_ptr<SoundController> soundController_;

	//���f������p�N���X
	Transform transform_;
	const Transform nullTransform_ = Transform();

	//�I�u�W�F�N�g�^�O
	OBJECT_TYPE objType_;

	//�ړ����x
	float moveSpeed_;

	//�����̈ړ����x
	float initSpeed_;

	//�L�����N�^�[�̕���
	VECTOR actorDir_;
	
	//�ړ�����
	VECTOR moveDir_;

	//�ŏI�I�Ɉړ�������W
	VECTOR movedPos_;

	//1f�O�̍��W
	VECTOR prePos_;

	bool isGround_;
	
	//���������̓Ǎ�����
	virtual void InitLoad(void) = 0;

	//���f��������̏�����
	virtual void InitTransform(void) = 0;

	//�Փ˔���̏�����
	virtual void InitCollider(void);

	//�����_�[�̏�����
	virtual void InitRender(void);

	//�S�Ă̏��������I�����ɍs���鏉��������
	virtual void InitEnd(void);

	//�G�t�F�N�g������
	virtual void InitEffect(void);

	//�T�E���h������
	virtual void InitSound(void);

	//�Փ˂����I�u�W�F�N�g�ɉ����ČĂ΂��֐�
	std::unordered_map<OBJECT_TYPE, std::function<void(void)>> isHitUpdate_;

	//isHitUpdate_�̓o�^
	virtual void InitIsHitUpdate(void) = 0;

	//�ėp���b�Z�[�W���󂯎�����ۂɍs����֐�
	std::unordered_map<Common_Message, std::function<void(void)>> commonMessageUpdate_;

	std::unordered_map<Message, std::function<void(void)>> messageUpdate_;

	//commonMessageUpdate_�̓o�^
	virtual void InitCommonMessageUpdate(void);

	virtual void Message_Landing(void);
	virtual void Message_NotLanding(void);

	virtual void UpdateObserver(void) final;

	virtual void UpdateObserver_SoundVolumeChange(void);

	virtual void InitUpdateObserver(void);
	

	/// <summary>
	/// �d�͂𖳌���
	/// </summary>
	/// <returns>
	/// true : ������
	/// false : �����Ȃ��A����������Ă��Ȃ�
	/// </returns>
	virtual const bool InvalidGravity(void) const;

private:

};

