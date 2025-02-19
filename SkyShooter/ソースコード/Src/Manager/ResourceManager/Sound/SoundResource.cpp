
#include <Dxlib.h>
#include "../../../Application.h"
#include "SoundResource.h"


//確保する配列サイズ
const int SIZE_MAX_DUPLICATE_SOUND_IDS = 10;

SoundResource::SoundResource(const std::string& path)
	:
	ResourceBase(Application::PATH_SOUND + path),
	duplicateSoundIds_{}
{

	if (duplicateSoundIds_.empty())
	{
		//一応10個分確保
		duplicateSoundIds_.reserve((size_t)SIZE_MAX_DUPLICATE_SOUND_IDS);
	}

}

SoundResource::~SoundResource(void)
{
	duplicateSoundIds_.clear();
}

void SoundResource::Load(void)
{
	SetUseASyncLoadFlag(true);
	handleId_ = LoadSoundMem(path_.c_str());
	SetUseASyncLoadFlag(false);
}

void SoundResource::Release(void)
{
	DeleteSoundMem(handleId_);
}

void SoundResource::Add_DuplicateSoundId(const int& duplicateHandleId)
{
	if ((int)duplicateSoundIds_.size() > SIZE_MAX_DUPLICATE_SOUND_IDS)
	{
		duplicateSoundIds_.clear();
	}

	duplicateSoundIds_.emplace_back(duplicateHandleId);
	duplicateSoundIds_.resize(duplicateSoundIds_.size());
}

void SoundResource::Clear_DuplicateSoundIds(void)
{
	duplicateSoundIds_.clear();
}

const std::vector<int>& SoundResource::Get_DuplicateSoundIds(void) const
{
	return duplicateSoundIds_;
}
