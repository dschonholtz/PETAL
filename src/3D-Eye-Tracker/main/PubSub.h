#pragma once
#ifndef PUBSUB_H
#define PUBSUB_H

#include "stdafx.h"
#include <list>
#include <map>
#include <queue>
#include "Subscriber.h"
#include "Publisher.h"
#include "eventData.h"

using namespace std;
class PubSubHandler {
public:
	PubSubHandler();
	void AddSubscriber(Subscriber* s, EventTopic e);
	void Forward();
	void ReceiveMessage(EventMessage e);
	void RemoveSubscriber(Subscriber *s, EventTopic e);

private:
	map<EventTopic, list<Subscriber*>> topicSubscribers;
	queue<EventMessage> messageQueue;
};



#endif
