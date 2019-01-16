#pragma once
#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "eventData.h"

class PubSubHandler;
class Publisher {
public:
	Publisher(PubSubHandler* p) { psh = p; }
    virtual ~Publisher() {}
	void Publish(EventMessage message) { psh->ReceiveMessage(message); }

private:
	PubSubHandler* psh;
};

#endif // !PUBLISHER_H

