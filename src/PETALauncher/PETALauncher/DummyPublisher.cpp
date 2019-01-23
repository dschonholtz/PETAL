#include "stdafx.h"
#include "DummyPublisher.h"

void DummyPublisher::Publish(EventMessage e)
{
	Publisher::Publish(e);
}
