#pragma once

#include <list>
#include "../Singleton.h"
#include "Message.h"

/// <summary>
/// �ʒm�N���X
/// </summary>
class Observer
	: public Singleton<Observer>
{
public:

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// ���݂̒ʒm��S�ď���
	/// </summary>
	void ClearMessage(void);

	/// <summary>
	/// �ʒm��S�ď���
	/// </summary>
	void ClearAllMessage(void);

	/// <summary>
	/// �ʒm�𑗂�
	/// </summary>
	/// <param name="message">
	/// Message
	/// </param>
	void SendMessageList(const Message message);

	/// <summary>
	/// �ʒm���󂯎��
	/// </summary>
	/// <returns>
	/// nowMessage_
	/// </returns>
	const std::list<Message>& ReceiveMessageList(void) const;

	/// <summary>
	/// �w�肵���ʒm�����݂��Ă��邩
	/// </summary>
	/// <param name="message">�w�肷��ʒm</param>
	/// <returns>
	/// true : ���݂��Ă���
	/// false : ���݂��Ă��Ȃ�
	/// </returns>
	const bool ReceiveMessageList(const Message message) const;

private:

	friend class Singleton<Observer>;

	Observer(void);
	virtual ~Observer(void) override;

	//���݂̒ʒm
	std::list<Message> nowMessage_;

	//1f�O�̒ʒm
	std::list<Message> preMessage_;

	//�ʒm�����M���ꂽ��
	bool isSendMessage_;

};

