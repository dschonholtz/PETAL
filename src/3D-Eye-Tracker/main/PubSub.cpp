#include "stdafx.h"
#include "pubsub.h"
#include "DummyPublisher.h"
#include "MouseController.h"

void PubSubHandler::AddSubscriber(Subscriber* s, EventTopic e) {
	if (topicSubscribers.find(e) == topicSubscribers.end()) {
		topicSubscribers.insert(make_pair(e, list<Subscriber*>{ s }));
	}
	else {
		topicSubscribers[e].push_back(s);
	}
}

void PubSubHandler::RemoveSubscriber(Subscriber*s, EventTopic e) {
	topicSubscribers[e].remove(s);
}

void PubSubHandler::ReceiveMessage(EventMessage e) {
	DWORD wait;
	while (1) 
	{
		wait = WaitForSingleObject(ghmutex, INFINITE);
		switch (wait)
		{
		case WAIT_OBJECT_0:
			messageQueue.push(e);
			ReleaseMutex(ghmutex);
			Forward();
			return;
				break;
		}

	}
}

void PubSubHandler::Forward() {
	while (messageQueue.size() > 0) {
		EventMessage tmpMessage = messageQueue.front();
		EventTopic topic = tmpMessage.topic;
		messageQueue.pop();

		list<Subscriber*>::iterator it;
		if (topicSubscribers.find(topic) != topicSubscribers.end()) {
			for (it = topicSubscribers[topic].begin(); it != topicSubscribers[topic].end(); it++) {
				(*it)->receiveMessage(tmpMessage);
			}

		}
	}
}
