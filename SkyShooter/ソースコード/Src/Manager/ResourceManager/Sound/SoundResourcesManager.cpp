

#include <DxLib.h>
#include "SoundResourcesManager.h"

using namespace std;

SoundResourcesManager::SoundResourcesManager(void)
	:
	ResourcesManagerTemplate()
{
	//BGM
	//タイトル
	AddResourceMap(SRC_SOUND::BGM_TITLE, "BGM/bgm_title.mp3", USESCENE_TITLE_ONLY);

	//チュートリアル
	AddResourceMap(SRC_SOUND::BGM_TUTORIAL, "BGM/bgm_tutorial.mp3");

	//ゲーム
	AddResourceMap(SRC_SOUND::BGM_GAME, "BGM/bgm_game.mp3", USESCENE_GAME_ONLY);

	//クリア
	AddResourceMap(SRC_SOUND::BGM_CLEAR, "BGM/bgm_clear.mp3");

	//ゲームオーバー
	AddResourceMap(SRC_SOUND::BGM_FAILED, "BGM/bgm_failed.mp3");


	//SE
	//ウルトやカウンターの発射音
	AddResourceMap(SRC_SOUND::SE_PLAYER_SHOT_SPECIAL, "SE/se_player_shot_special.mp3", USESCENE_GAME_ONLY);
	
	//ウルトチャージ音
	AddResourceMap(SRC_SOUND::SE_PLAYER_ULT_CHAGE, "SE/se_player_ult_charge.mp3", USESCENE_GAME_ONLY);
	
	//通常弾や追尾弾発射音
	AddResourceMap(SRC_SOUND::SE_PLAYER_SHOT, "SE/se_player_shot.mp3", USESCENE_GAME_ONLY);

	//被弾音
	AddResourceMap(SRC_SOUND::SE_PLAYER_DAMAGE, "SE/se_player_damage.mp3");

	//ダッシュ移行音
	AddResourceMap(SRC_SOUND::SE_PLAYER_DASH, "SE/se_player_dash.mp3");
	
	//敵ワープ音
	AddResourceMap(SRC_SOUND::SE_ENEMY_WARP, "SE/se_enemy_warp.mp3", USESCENE_GAME_ONLY);
	
	//メニューカーソル音
	AddResourceMap(SRC_SOUND::SE_MENU_CURSORMOVE, "SE/se_menu_cursormove.mp3", USESCENE_ALL);
	
	//シールドカウンター弾チャージ音
	AddResourceMap(SRC_SOUND::SE_SHIELDCOUNTER_CHARGE, "SE/se_player_shieldcounter_charge.mp3", USESCENE_GAME_ONLY);
	
	//メニュー決定音
	AddResourceMap(SRC_SOUND::SE_MENU_ENTER, "SE/se_menu_enter.mp3", USESCENE_ALL);
	
	//敵クールタイム終了音
	AddResourceMap(SRC_SOUND::SE_ENEMY_COOLTIME_END, "SE/se_enemy_cooltime_end.mp3",USESCENE_GAME_ONLY);
}

SoundResourcesManager::~SoundResourcesManager(void)
{
}

int SoundResourcesManager::LoadSoundDuplicate(SRC_SOUND src)
{
	SoundResource& res = _Load(src);
	
	int duId = DuplicateSoundMem(res.GetHandleId());
	res.Add_DuplicateSoundId(duId);

	return duId;
}

void SoundResourcesManager::Release(void)
{
	for (auto& lM : loadedMap_)
	{
		lM.second.Release();
	}
	loadedMap_.clear();
}

SoundResource& SoundResourcesManager::_Load(SRC_SOUND src)
{
	if (loadedMap_.contains(src))
	{
		//読込済
		return loadedMap_.at(src);
	}

	if (!resourcesMap_.contains(src))
	{
		//未登録
		return unregistered;
	}

	//登録済、未読込
	auto& ret = resourcesMap_.at(src);
	ret->Load();

	loadedMap_.emplace(src, *ret);

	return *ret;

}

void SoundResourcesManager::AddResourceMap(SRC_SOUND src, const std::string& file)
{
	unique_ptr<SoundResource> res;

	res = make_unique<SoundResource>(file);
	resourcesMap_.emplace(src, move(res));
}

void SoundResourcesManager::AddResourceMap(const SRC_SOUND src, const std::string& file, const std::vector<SCENE_ID>& useScene)
{
	unique_ptr<SoundResource> res;

	res = make_unique<SoundResource>(file);
	resourcesMap_.emplace(src, move(res));
	resourcesUseMap_.emplace(src, useScene);
}
