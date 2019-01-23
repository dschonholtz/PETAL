#pragma once


#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "EventData.h"
#include <queue>
#include <list>

class PubSubHandler;
class Subscriber {
private:
	PubSubHandler * psh;
	queue<EventMessage> receivedMessageQueue;

public:
	Subscriber(PubSubHandler* p) { psh = p; }
	virtual ~Subscriber() { }
	void receiveMessage(EventMessage e) { receivedMessageQueue.push(e); }
	void SubscribeToTopic(EventTopic t) { psh->AddSubscriber(this, t); }
	void UnSubscribeToTopic(EventTopic t) { psh->RemoveSubscriber(this, t); }
	EventMessage getTopMessage() { return receivedMessageQueue.front(); }

	virtual void readMessages() = 0;
};

#endif // !SUBSCRIBER_H
