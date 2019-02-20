#include "stdafx.h"
#include "Publisher.h"

Publisher::Publisher() {
	psh = NULL;
}

Publisher::Publisher(PubSubHandler* p) {
	psh = p;
}

void Publisher::Publish(EventMessage message) {
	psh->ReceiveMessage(message);
}