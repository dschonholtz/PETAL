#pragma once
#ifndef PUBSUB_H
#define PUBSUB_H

#include "stdafx.h"
#include <list>
#include <map>
#include <queue>
#include "subscriber.h"
#include "publisher.h"
#include "eventData.h"

using namespace std;
class PubSubHandler {
public:
	PubSubHandler();
	void AddSubscriber(Subscriber* s, EventTopic e);
	void Forward();
	void ReceiveMessage(EventMessage e);
	void RemoveSubscriber(Subscriber *s, EventTopic e);
	DWORD Run();
	void StartThread(void);


private:
	map<EventTopic, list<Subscriber*>> topicSubscribers;
	queue<EventMessage> messageQueue;
};



#endif
