
#include <cassert>
#include <fstream>
#include"../Application.h"
#include "JSONResource.h"

using json = nlohmann::json;

using namespace std;

JSONResource::JSONResource(const std::string& fileName)
	:
	fileName_(Application::PATH_JSON + fileName),
	jsonData_{}
{
}

JSONResource::~JSONResource(void)
{
	fileName_.clear();
	jsonData_.clear();
}

void JSONResource::Load(void)
{
	ifstream ifs(fileName_);

	json data;


	if (ifs)
	{
		ifs >> jsonData_;
	}
	else
	{
		assert(!ifs);
		//外部ファイルの読み込み失敗
		return;
	}
}
