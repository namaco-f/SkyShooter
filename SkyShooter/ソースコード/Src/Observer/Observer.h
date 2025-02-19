#pragma once

#include <list>
#include "../Singleton.h"
#include "Message.h"

/// <summary>
/// 通知クラス
/// </summary>
class Observer
	: public Singleton<Observer>
{
public:

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 現在の通知を全て消去
	/// </summary>
	void ClearMessage(void);

	/// <summary>
	/// 通知を全て消去
	/// </summary>
	void ClearAllMessage(void);

	/// <summary>
	/// 通知を送る
	/// </summary>
	/// <param name="message">
	/// Message
	/// </param>
	void SendMessageList(const Message message);

	/// <summary>
	/// 通知を受け取る
	/// </summary>
	/// <returns>
	/// nowMessage_
	/// </returns>
	const std::list<Message>& ReceiveMessageList(void) const;

	/// <summary>
	/// 指定した通知が存在しているか
	/// </summary>
	/// <param name="message">指定する通知</param>
	/// <returns>
	/// true : 存在している
	/// false : 存在していない
	/// </returns>
	const bool ReceiveMessageList(const Message message) const;

private:

	friend class Singleton<Observer>;

	Observer(void);
	virtual ~Observer(void) override;

	//現在の通知
	std::list<Message> nowMessage_;

	//1f前の通知
	std::list<Message> preMessage_;

	//通知が送信されたか
	bool isSendMessage_;

};

