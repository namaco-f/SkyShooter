#pragma once
#include <string>
#include "Singleton.h"

/// <summary>
/// 起動時の初期化、終了時の解放、ゲームループを行うクラス
/// </summary>
class Application :
	public Singleton<Application>
{

public:

	static constexpr int SCREEN_SIZE_X = 1600;						//スクリーンサイズX
	static constexpr int SCREEN_SIZE_Y = 900;						//スクリーンサイズY
	static constexpr int SCREEN_SIZE_HALF_X = SCREEN_SIZE_X / 2;	//スクリーンサイズXの半分
	static constexpr int SCREEN_SIZE_HALF_Y = SCREEN_SIZE_Y / 2;	//スクリーンサイズYの半分

	static constexpr int DEBUG_STR_DIFF_Y_INT = 20;					//文字列の間隔 : デバッグ用
	static constexpr float DEBUG_STR_DIFF_Y_F = 20.0f;				//文字列の間隔 : デバッグ用

	static constexpr int HANDLE_NULL = -1;							//ハンドルIDエラー

	//データパス関連
	static const std::string PATH_IMAGE;			//画像
	static const std::string PATH_MODEL;			//モデル
	static const std::string PATH_EFFECT;			//エフェクト
	static const std::string PATH_SOUND;			//音
	static const std::string PATH_FONT;				//フォント
	static const std::string PATH_ANIMATION;		//アニメーション
	static const std::string PATH_SHADER;			//シェーダー
	static const std::string PATH_SHADER_PIXEL;		//ピクセルシェーダー
	static const std::string PATH_SHADER_VERTEXS;	//頂点シェーダー
	static const std::string PATH_JSON;				//JSON
	static const std::string PATH_BINARY;			//バイナリ

	/// <summary>
	/// ゲームループの開始
	/// </summary>
	void Run(void);

	/// <summary>
	/// リソースの解放
	/// </summary>
	/// <remarks>DxlibEnd()前に解放処理を書くこと</remarks>
	virtual void Destroy(void) override;

	/// <summary>
	/// Dxlibの初期化成功／失敗の判定
	/// </summary>
	/// <returns>
	/// 成功時 : true
	/// 失敗時 : false
	/// </returns>
	const bool IsInitFail(void) const;

private:

	friend class Singleton<Application>;

	//初期化失敗
	bool isInitFail_;

	//解放失敗
	bool isReleaseFail_;

	//デフォルトコンストラクタ
	Application(void);
	virtual ~Application(void);

	//Effekseerの初期化
	void InitEffekseer(void);

	void CalcFrameRate(void);

	void DrawFrameRateCnt(void);

	int currentTime_ = 0;
	int lastTime_ = 0;

	int frameCnt_ = 0;
	int updateFrameRateTime_ = 0;

	float frameRate_ = 0.0f;

		
};

