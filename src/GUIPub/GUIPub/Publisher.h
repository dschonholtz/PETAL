#pragma once
#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "EventData.h"
class PubSubHandler;
class Publisher {
public:
	Publisher();
	Publisher(PubSubHandler* p);
	virtual ~Publisher() {}
	void Publish(EventMessage message);

private:
	PubSubHandler * psh;
};

#endif // !PUBLISHER_H

