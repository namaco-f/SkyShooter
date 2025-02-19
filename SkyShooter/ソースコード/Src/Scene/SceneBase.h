#pragma once
#include <memory>
#include <string>
#include <future>
#include <functional>

class SceneManager;
class Camera;
class ResourceManager;
class ModelResourcesManager;
class SoundResourcesManager;
class EffectResourcesManager;
class ImageResourcesManager;
class ImagesResourcesManager;
class FontResourcesManager;
class ShaderManager;
class CharactorManager;
class ActorManager;
class UIManager;
class Collision;
class Observer;
class InputController;
class SoundController;
class Grid3d;

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase
{
public:

	SceneBase(void);
	virtual ~SceneBase(void);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void) = 0;

	/// <summary>
	/// 音量更新
	/// </summary>
	virtual void UpdateSound(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void) = 0;

	/// <summary>
	/// 非同期ロード
	/// </summary>
	virtual void ASyncLoad(void) = 0;

protected:

	//読込状態
	enum class LoadState
	{
		None,			//初期化用
		IsAsyncLoad,	//非同期読込中
		IsLoadEnd,		//読込終了
	};

	//画面状態
	enum class State
	{
		None,			//初期化用
		Load,			//読込中
		Play,			//実行中
		End,			//終了中
	};

	//メニューカーソル移動音のキー値
	const std::string SE_CURSORMOVE = "CursorMove";

	//メニュー決定音のキー値
	const std::string SE_ENTER = "Enter";

	//シーン管理
	SceneManager& scnMng_;

	//カメラ
	Camera& camera_;

	// リソース管理
	ResourceManager& resMng_;

	//リソース管理 : モデル
	ModelResourcesManager& resMngModel_;
	
	//リソース管理 : 音
	SoundResourcesManager& resMngSound_;
	
	//リソース管理 : エフェクト
	EffectResourcesManager& resMngEffect_;
	
	//リソース管理 : 画像
	ImageResourcesManager& resMngImage_;
	
	//リソース管理 : 分割画像
	ImagesResourcesManager& resMngImages_;
	
	//シェーダー管理
	ShaderManager& resMngShader_;
	
	//フォント管理
	FontResourcesManager& resMngFont_;

	//オブジェクト管理
	ActorManager& actorMng_;

	//キャラクター管理
	CharactorManager& charaMng_;

	//UI管理
	UIManager& uiMng_;

	//オブザーバー
	Observer& observer_;

	//入力制御
	std::unique_ptr<InputController> inputController_;

	//音制御
	std::unique_ptr<SoundController> soundController_;

	//グリッド線
	std::unique_ptr<Grid3d> grid_;

	//読込時間カウント
	float loadingTime_;


	//int* loadingImgsHandle_;
	//読込画像ハンドル
	std::vector<int> loadingImgsHandles_;

	//読込画像インデックスカウント用
	int loadingImgsIdxCnt_;

	//読込画像インデックス上限
	int loadingImgsIdxMax_;

	//読込時間カウント
	float loadingCntStep_;

	//読込終了画像ハンドル
	int loadingEndStartImgsHandle_;

	//操作説明画像
	std::vector<int> tutorialImgsHandle_;

	//操作説明時に表示する左右矢印
	int tutorialArrowsImgsHandle_;

	//操作説明画像インデックス
	int tutorialImgsIdxCnt_;

	//画面状態
	State state_;

	//読込状態確認
	LoadState loadState_;

	//音の初期化
	virtual void InitSound(void);

	//画面状態遷移の登録
	virtual void InitChangeState(void);

	//読込状態遷移の登録
	virtual void InitChangeLoadState(void);

	//画面状態遷移
	virtual void ChangeState(const State state);

	//画面状態遷移時の初期化
	std::unordered_map<State, std::function<void(void)>> stateChange_;
	virtual void ChangeState_Load(void);		//読込中
	virtual void ChangeState_Play(void);		//実行中
	virtual void ChangeState_End(void);			//終了中

	//画面状態ごとの更新
	std::function<void(void)> stateUpdate_;
	virtual void StateUpdate_Load(void);		//読込中
	virtual void StateUpdate_Play(void);		//実行中
	virtual void StateUpdate_End(void);			//終了中

	//画面状態ごとの描画
	std::function<void(void)> stateDraw_;
	virtual void StateDraw_Load(void);			//読込中
	virtual void StateDraw_Play(void);			//実行中
	virtual void StateDraw_End(void);			//終了中

	//読込状態遷移
	virtual void ChangeLoadState(const LoadState loadState);

	//読込状態遷移時の初期化
	std::unordered_map<LoadState, std::function<void(void)>> loadStateChange_;
	virtual void ChangeLoadState_IsAsyncLoad(void);		//非同期読込中
	virtual void ChangeLoadState_IsLoadEnd(void);		//読込終了

	//読込状態ごとの更新
	std::function<void(void)> loadStateUpdate_;
	virtual void LoadStateUpdate_IsAsyncLoad(void);		//非同期読込中
	virtual void LoadStateUpdate_IsLoadEnd(void);		//読込終了

	//読込状態ごとの描画
	std::function<void(void)> loadStateDraw_;
	virtual void LoadStateDraw_IsAsyncLoad(void);		//非同期読込中
	virtual void LoadStateDraw_IsLoadEnd(void);			//読込終了

	virtual void UpdateTutorial(void);
	virtual void DrawTutorial(void);

	virtual void UpdateIsASyncLoad(void);
	virtual void DrawIsASyncLoad(void);

	//Dxlib非対応用の非同期読込
	std::future<void> aSyncLoad_;
	std::function<void(void)> effectLoad_;
};