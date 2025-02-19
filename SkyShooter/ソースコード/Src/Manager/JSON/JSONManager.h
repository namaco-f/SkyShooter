#pragma once
#include <unordered_map>
#include <memory>
#include "JSONResource.h" 
#include "../../Singleton.h" 

/// <summary>
/// JSON���\�[�X��
/// </summary>
enum class SRC_JSON
{
	INPUT_INIT_DATA,
	BOX_ATTACK_POS_DATA,
};

/// <summary>
/// JSON�̓o�^�A�Ǘ�
/// </summary>
class JSONManager
	: public Singleton<JSONManager>
{

public:

	/// <summary>
	/// �w�肵��JSON�f�[�^�ǂݍ���
	/// </summary>
	/// <param name="src">�w�肷��f�[�^��</param>
	/// <returns>JSONResource�̎Q��</returns>
	const JSONResource& Load(const SRC_JSON src);

private:

	friend class Singleton<JSONManager>;

	//���o�^
	JSONResource unregistered;

	JSONManager(void);
	virtual ~JSONManager(void) override;

	//�o�^��
	std::unordered_map<SRC_JSON, std::unique_ptr<JSONResource>> jsonDatas_;

	//�Ǎ���
	std::unordered_map<SRC_JSON, JSONResource&> jsonLoadedDatas_;

	//�Ǎ�����
	JSONResource& _Load(const SRC_JSON src);

	void AddJSONData(const SRC_JSON src, const std::string& path);

};

