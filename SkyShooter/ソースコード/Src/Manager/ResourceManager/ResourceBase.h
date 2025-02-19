#pragma once
#include <string>
#include <vector>
#include "../SceneId.h"

/// <summary>
/// ���\�[�X�̓Ǎ��A����̊��N���X
/// </summary>
class ResourceBase
{

public:

	ResourceBase(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	explicit ResourceBase(const std::string& path);
	virtual ~ResourceBase(void);

	/// <summary>
	/// �Ǎ�
	/// </summary>
	virtual void Load(void) = 0;

	/// <summary>
	/// ���
	/// </summary>
	virtual void Release(void) = 0;

	/// <summary>
	/// �n���h��ID�̎擾
	/// </summary>
	virtual const int& GetHandleId(void) const;

	/// <summary>
	/// �w�肵���V�[���Ń��\�[�X���g�p���邩
	/// </summary>
	/// <param name="scnId">SCENE_ID</param>
	/// <returns>
	/// true : �g�p����
	/// false : �g�p���Ȃ�
	/// </returns>
	virtual const bool IsUseScene(const SCENE_ID scnId) const;
	
	/// <summary>
	/// �g�p����V�[�����擾
	/// </summary>
	/// <returns>useSceneIds_</returns>
	inline virtual const std::vector<SCENE_ID>& GetUseScenes(void) const
	{
		return useSceneIds_;
	}

protected:

	//�n���h��ID
	int handleId_;

	//�p�X
	std::string path_;

	//�g�p����V�[��
	std::vector<SCENE_ID> useSceneIds_;

private:

};

