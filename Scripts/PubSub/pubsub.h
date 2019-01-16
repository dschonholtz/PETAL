#pragma once
#ifndef PUBSUB_H
#define PUBSUB_H

#include <list>
#include <map>
#include <queue>
#include "subscriber.h"
#include "publisher.h"
#include "eventData.h"

using namespace std;
class PubSubHandler {
public:
	void AddSubscriber(Subscriber* s, EventTopic e);
	void Forward();
	void ReceiveMessage(EventMessage e);
	void RemoveSubscriber(Subscriber *s, EventTopic e);

private:
	map<EventTopic, list<Subscriber*>> topicSubscribers;
	queue<EventMessage> messageQueue;
};



#endif
