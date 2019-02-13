#pragma once
#include <windows.h>
#include "Subscriber.h"

class CentralProcessor : public Subscriber, public Publisher
{
	// https://docs.microsoft.com/en-us/windows/desktop/menurc/using-cursors

	// Access specifier 
public:
	CentralProcessor(PubSubHandler* p) : Subscriber(p) {
	}
	void receiveMessage(EventMessage e);
	virtual void readMessages();
	// Member Functions() 
	void setCursorPosition(int x, int y);
};