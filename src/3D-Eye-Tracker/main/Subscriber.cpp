#include "stdafx.h"
#include "Subscriber.h"

Subscriber::Subscriber(PubSubHandler* p) {
	psh = p;
}

void Subscriber::receiveMessage(EventMessage e) {
	receivedMessageQueue.push(e);
	readMessages();
}

void Subscriber::SubscribeToTopic(EventTopic t) {
	psh->AddSubscriber(this, t);
}

void Subscriber::UnSubscribeToTopic(EventTopic t) {
	psh->RemoveSubscriber(this, t);
}

EventMessage Subscriber::getTopMessage() {
	EventMessage em = receivedMessageQueue.front();
	receivedMessageQueue.pop();
	return em;
}

void Subscriber::EmptyQueue() {
	receivedMessageQueue.empty();
}