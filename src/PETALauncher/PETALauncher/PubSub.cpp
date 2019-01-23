#include "stdafx.h"
//#include "Dummies.h"

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
	messageQueue.push(e);
	//Unsure when to forward messages
	Forward();
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
