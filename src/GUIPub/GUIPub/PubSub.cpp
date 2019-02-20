#include "stdafx.h"
#include "pubsub.h"
#include "DummyPublisher.h"
#include "MouseController.h"

PubSubHandler::PubSubHandler() {
}

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

VOID startProcess(LPCWSTR path)
{
	std::cout << "in startup\n";
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWDEFAULT);
	std::cout << "after attempt\n";
}

DWORD WINAPI startMethodInThread(LPVOID arg)
{
	if (!arg)
		return 0;
	PubSubHandler *ptr = (PubSubHandler*)arg;
	ptr->Run();
	return 1;
}

void PubSubHandler::StartThread(void)
{
	CreateThread(NULL, 4096, startMethodInThread, this, 0, NULL);
}

DWORD PubSubHandler::Run(void)
{	
	DummyPublisher pub = DummyPublisher(this);
	MouseController sub = MouseController(this);
	this->AddSubscriber(&sub, MousePos);
	int x;
	int y;
	MousePosData mpd;
	EventMessage em;

	while (true) {
		std::cout << "Enter new x coord: ";
		OutputDebugStringW(L"Running handler\n");
		std::cin >> x;
		std::cout << "Enter new y coord: ";
		std::cin >> y;
		mpd.x = x;
		mpd.y = y;
		//Bad
		/*em.topic = MousePos;
		em.data = &mpd;
		pub.Publish(em);*/
	}
}
