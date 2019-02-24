#include "stdafx.h"
#include "DummyPublisher.h"
DummyPublisher::DummyPublisher() : Publisher() {

}

DummyPublisher::DummyPublisher(PubSubHandler* p) : Publisher(p) {

}

void DummyPublisher::Publish(EventMessage e)
{
	Publisher::Publish(e);
}
