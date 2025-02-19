#pragma once

#include "../ResourcesManagerTemplate.h"
#include"ImageResource.h"

/// <summary>
/// 画像リソースの名前
/// </summary>
enum class SRC_IMAGE
{
	TrailTexture,						//追尾弾の軌跡テクスチャ
	WATERWAVE_CUBE_TEX,					//水面テクスチャ
	CircleShadow,						//丸影
	Keyboard_Mouse,						//キーマウ画像
	Controller,							//コントローラー画像
	TitleLogo,							//タイトルロゴ
	ClearText,							//クリアテキスト
	LoadEnd_Start,						//スタート画像
	Cursor,								//カーソル
	Arrow,								//矢印
	OperationInstructions_Controller,	//コントローラー操作説明
	OperationInstructions_KeyBoard,		//キーボード操作説明
	HP_TEXT,							//HP
	MP_TEXT,							//MP
	SE_TEXT,							//SE
	BGM_TEXT,							//BGM
	BOSS_ENEMY_TEXT,					//ボス
	MAX,
};

/// <summary>
/// 画像の管理
/// </summary>
class ImageResourcesManager
	: 
	public ResourcesManagerTemplate<
	ImageResourcesManager,
	ImageResource,
	SRC_IMAGE
	>
{

public:

	ImageResourcesManager(void);
	virtual ~ImageResourcesManager(void) override;

	/// <summary>
	/// 解放
	/// </summary>
	void Release(void);

private:

	//読込処理
	ImageResource& _Load(const SRC_IMAGE src) override;

	//エフェクトの追加
	void AddResourceMap(const SRC_IMAGE src,const std::string& file);
	void AddResourceMap(
		const SRC_IMAGE src,
		const std::string& file,
		const std::vector<SCENE_ID>& useScenes
	);

};

