#include "Observer.h"

using namespace std;

Observer* Singleton<Observer>::instance_ = nullptr;

Observer::Observer(void)
    :
    nowMessage_{},
    preMessage_{},
    isSendMessage_(false)
{
}

Observer::~Observer(void)
{
    ClearAllMessage();
}

void Observer::Update(void)
{
    if (isSendMessage_)
    {
        preMessage_ = nowMessage_;
        ClearMessage();
    }
}

void Observer::ClearMessage(void)
{
    nowMessage_.clear();
}

void Observer::ClearAllMessage(void)
{
    nowMessage_.clear();
    preMessage_.clear();
}

void Observer::SendMessageList(const Message message)
{
    isSendMessage_ = true;
    nowMessage_.emplace_back(message);
}

const std::list<Message>& Observer::ReceiveMessageList(void) const
{
    return preMessage_;
}

const bool Observer::ReceiveMessageList(const Message message) const
{
    for (const Message& mes : nowMessage_)
    {
        if (mes == message)
        {
            //’Ê’m‚ª‘¶İ‚µ‚Ä‚¢‚é
            return true;
        }
    }

    //’Ê’m‚ª‘¶İ‚µ‚Ä‚¢‚È‚¢
    return false;
}
