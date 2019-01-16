#pragma once
#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "eventData.h"
#include <queue>
#include <list>

class PubSubHandler;
class Subscriber {
public:
	Subscriber(PubSubHandler* p) { psh = p; }
	virtual ~Subscriber() { }
	void receiveMessage(EventMessage e) { receivedMessageQueue.push(e); }
	void SubscribeToTopic(EventTopic t) { psh->AddSubscriber(this, t); }
	void UnSubscribeToTopic(EventTopic t) { psh->RemoveSubscriber(this, t); }

	virtual void readMessages() = 0;

private:
	PubSubHandler* psh;
	queue<EventMessage> receivedMessageQueue;
};

#endif // !SUBSCRIBER_H
