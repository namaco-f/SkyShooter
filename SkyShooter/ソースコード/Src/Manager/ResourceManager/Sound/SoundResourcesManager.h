#pragma once


#include "../ResourcesManagerTemplate.h"
#include "SoundResource.h"

/// <summary>
///　音のリソース名
/// </summary>
enum class SRC_SOUND
{
	BGM_TITLE,				//タイトルBGM
	BGM_TUTORIAL,			//チュートリアルBGM
	BGM_GAME,				//ゲームBGM
	BGM_CLEAR,				//クリアBGM
	BGM_FAILED,				//敗北BGM
	
	SE_PLAYER_SHOT,				//ビームガン
	SE_PLAYER_SHOT_SPECIAL,			//ビーム砲
	SE_PLAYER_ULT_CHAGE,	//ビーム砲チャージ
	SE_PLAYER_DAMAGE,				//打撃
	SE_PLAYER_DASH,				//ブースター
	SE_ENEMY_WARP,			//出現
	SE_MENU_ENTER,				//決定
	SE_MENU_CURSORMOVE,			//カーソル移動
	SE_SHIELDCOUNTER_CHARGE,				//パワーアップ
	SE_SPEEDUP,				//スピードアップ
	SE_ENEMY_COOLTIME_END,			//素振り音
	
	MAX,					//最大数カウント用
};

/// <summary>
/// 音の管理
/// </summary>
class SoundResourcesManager
	: public ResourcesManagerTemplate<
	SoundResourcesManager,
	SoundResource,
	SRC_SOUND
	>
{

public:

	SoundResourcesManager(void);
	virtual ~SoundResourcesManager(void) override;
	
	/// <summary>
	/// 音の複製 : 3分を超えると複製不可
	/// </summary>
	/// <param name="src">リソース名</param>
	/// <returns>ハンドル名</returns>
	int LoadSoundDuplicate(const SRC_SOUND src);

	/// <summary>
	/// 解放
	/// </summary>
	void Release(void);


private:

	//リソースの読込
	SoundResource& _Load(const SRC_SOUND src) override;

	//リソースの登録
	void AddResourceMap(const SRC_SOUND src, const std::string& file);
	void AddResourceMap(
		const SRC_SOUND src,
		const std::string& file,
		const std::vector<SCENE_ID>& useScene
	);
};

