#pragma once

//���\�[�X�Ǘ��N���X�̊Ǘ�

#include <memory>
#include <functional>
#include <list>
#include "../../Singleton.h"
#include "../SceneId.h"

class ModelResourcesManager;
class SoundResourcesManager;
class ImageResourcesManager;
class ImagesResourcesManager;
class EffectResourcesManager;
class FontResourcesManager;
class ShaderManager;

/// <summary>
/// �e���\�[�X�Ǘ��N���X�̊Ǘ�
/// </summary>
class ResourceManager
	: public Singleton<ResourceManager>
{

public:

	/// <summary>
	/// Dxlib_End�O�ɉ��
	/// </summary>
	void Destroy(void) override;

	/// <summary>
	/// �������
	/// </summary>
	void Release(void);

	void Update(void);

	void Load(const SCENE_ID scene);

	inline const bool GetIsLoadingNow(void) const
	{
		return (int)(loadingFunc_.size()) > 0;
	}

	/// <summary>
	/// ���f���Ǘ��N���X�̎擾
	/// </summary>
	inline  ModelResourcesManager& GetModelResourcesInstance(void)
	{
		return *modelResourcesManager_;
	}

	/// <summary>
	/// ���Ǘ��N���X�̎擾
	/// </summary>
	inline SoundResourcesManager& GetSoundResourcesInstance(void)
	{
		return *soundResourcesManager_;
	}

	/// <summary>
	/// �摜�Ǘ��N���X�̎擾
	/// </summary>
	inline ImageResourcesManager& GetImageResourcesInstance(void)
	{
		return *imageResourcesManager_;
	}

	/// <summary>
	/// �����摜�Ǘ��N���X�̎擾
	/// </summary>
	inline ImagesResourcesManager& GetImagesResourcesInstance(void)
	{
		return *imagesResourcesManager_;
	}

	/// <summary>
	///	�G�t�F�N�g�Ǘ��N���X�̎擾
	/// </summary>
	inline EffectResourcesManager& GetEffectResourcesInstance(void)
	{
		return *effectResourcesManager_;
	}	
	
	/// <summary>
	///	�G�t�F�N�g�Ǘ��N���X�̎擾
	/// </summary>
	inline FontResourcesManager& GetFontResourcesInstance(void)
	{
		return *fontResourcesManager_;
	}

	/// <summary>
	///	�V�F�[�_�[�Ǘ��N���X�̎擾
	/// </summary>
	inline ShaderManager& GetShaderResourcesInstance(void)
	{
		return *shaderResourcesManager_;
	}


private:

	friend class Singleton<ResourceManager>;

	//���f��
	std::unique_ptr<ModelResourcesManager> modelResourcesManager_;

	//��
	std::unique_ptr<SoundResourcesManager> soundResourcesManager_;

	//�摜
	std::unique_ptr<ImageResourcesManager> imageResourcesManager_;
	
	//�����摜
	std::unique_ptr<ImagesResourcesManager> imagesResourcesManager_;

	//�G�t�F�N�g
	std::unique_ptr<EffectResourcesManager> effectResourcesManager_;

	//�V�F�[�_�[
	std::unique_ptr<ShaderManager> shaderResourcesManager_;

	//�t�H���g
	std::unique_ptr<FontResourcesManager> fontResourcesManager_;

	std::list<std::function<void(void)>> loadingFunc_;

	float loadingCnt_;
	bool isLoadingNow_;

	SCENE_ID nowLoadingSceneResouce_;
	
	ResourceManager(void);
	virtual ~ResourceManager(void) override;

	void ChangeLoadResource(void);

};

