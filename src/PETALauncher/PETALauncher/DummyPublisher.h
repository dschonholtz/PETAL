#include "Publisher.h"

class DummyPublisher : public Publisher
{
	// Access specifier 
public:
	void Publish(EventMessage e);
	DummyPublisher();
	DummyPublisher(PubSubHandler* p);
};