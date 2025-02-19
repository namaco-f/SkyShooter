
#include <Dxlib.h>
#include <vector>
#include "JSONManager.h"

using namespace std;

JSONManager* Singleton<JSONManager>::instance_ = nullptr;

JSONManager::JSONManager(void)
{
	AddJSONData(SRC_JSON::BOX_ATTACK_POS_DATA, "BoxAttackPosOutPut.json");
}

JSONManager::~JSONManager(void)
{
	jsonDatas_.clear();
	jsonLoadedDatas_.clear();
}

const JSONResource& JSONManager::Load(const SRC_JSON src)
{
	const JSONResource& ret = _Load(src);

	if (jsonDatas_.find(src) == jsonDatas_.end())
	{
		return unregistered;
	}

	return ret;
}

JSONResource& JSONManager::_Load(const SRC_JSON src)
{
	//���[�h�ς݃`�F�b�N
	const auto& lPair = jsonLoadedDatas_.find(src);
	if (lPair != jsonLoadedDatas_.end())
	{
		return jsonLoadedDatas_.find(src)->second;
	}

	//���\�[�X�o�^�`�F�b�N
	const auto& rPair = jsonDatas_.find(src);
	if (rPair == jsonDatas_.end())
	{
		//�o�^����Ă��Ȃ�
		return unregistered;
	}

	//���[�h����
	rPair->second->Load();

	//�O�̂��߃R�s�[�R���X�g���N�^
	jsonLoadedDatas_.emplace(src, *rPair->second);

	return *rPair->second;
}

void JSONManager::AddJSONData(const SRC_JSON src, const std::string& path)
{
	using JSONRes = JSONResource;

	unique_ptr<JSONRes> loader;
	loader = make_unique<JSONRes>(path);
	jsonDatas_.emplace(src, move(loader));
}










