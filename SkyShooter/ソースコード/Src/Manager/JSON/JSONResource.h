#pragma once

#include<string>
#include<vector>
#include<unordered_map>
#include <Dxlib.h>
#include"../Lib/nlohmann/json.hpp"

class JSONResource
{
public:

	JSONResource(void) = default;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fileName">Data/JSON/�ȍ~�̃p�X</param>
	explicit JSONResource(const std::string& fileName);
	virtual ~JSONResource(void);

	/// <summary>
	/// �Ǎ�
	/// </summary>
	void Load(void);

	/// <summary>
	/// �Ǎ���JSON���\�[�X�̎擾
	/// </summary>
	const nlohmann::json& GetJSONData(void) const
	{
		return jsonData_;
	}

private:

	//�t�@�C���p�X
	std::string fileName_;

	//JSON���\�[�X
	nlohmann::json jsonData_;
};


