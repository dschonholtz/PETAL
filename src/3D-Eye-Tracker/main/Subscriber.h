#pragma once

#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H
#include "stdafx.h"
#include "EventData.h"
#include <queue>

using namespace std;

class PubSubHandler;
class Subscriber {
public:
	Subscriber(PubSubHandler* p);
	virtual ~Subscriber() { }
	void receiveMessage(EventMessage e);
	void SubscribeToTopic(EventTopic t);
	void UnSubscribeToTopic(EventTopic t);
	EventMessage getTopMessage();
	void EmptyQueue();

	virtual void readMessages() = 0;

private:
	PubSubHandler * psh;
	queue <EventMessage> receivedMessageQueue;
};

#endif // !SUBSCRIBER_H
