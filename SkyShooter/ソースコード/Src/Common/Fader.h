#pragma once

#include <functional>
#include <unordered_map>

/// <summary>
/// フェードイン,フェードアウトを行うクラス
/// </summary>
class Fader
{
public:

	/// <summary>
	/// フェードの状態
	/// </summary>
	enum class State
	{
		NONE, 
		FADE_OUT,	//フェードアウト
		FADE_IN		//フェードイン
	};

	Fader(void);
	virtual ~Fader(void);

	/// <summary>
	/// フェード状態の取得
	/// </summary>
	/// <returns>state_</returns>
	State GetState(void) const;

	/// <summary>
	/// フェード処理が終了しているか
	/// </summary>
	bool IsEnd(void) const;

	/// <summary>
	/// 指定したフェードを開始
	/// </summary>
	void ChangeState(const State state);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

private:

	//状態
	State state_;

	//透明度
	int alpha_;

	//状態(State)を保ったまま終了判定を行うため、
	//Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	//フェード処理の終了判定
	bool isEnd_;

	//状態遷移
	std::unordered_map<State, std::function<void(void)>> changeStates_;
	void ChangeState_None(void);		//フェードなし
	void ChangeState_FadeOut(void);		//フェードアウト
	void ChangeState_FadeIn(void);		//フェードイン

	//状態ごとの更新処理
	std::function<void(void)> stateUpdate_;
	void StateUpdate_None(void);		//フェードなし
	void StateUpdate_FadeOut(void);		//フェードアウト
	void StateUpdate_FadeIn(void);		//フェードイン

	//状態ごとの描画処理
	std::function<void(void)> stateDraw_;
	void StateDraw_None(void);			//フェードなし
	void StateDraw_FadeOut(void);		//フェードアウト
	void StateDraw_FadeIn(void);		//フェードイン

	//状態遷移の登録
	void InitChangeState(void);
};
