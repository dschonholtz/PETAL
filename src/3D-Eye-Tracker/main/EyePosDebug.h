#pragma once
#include "Subscriber.h"

class EyePosDebug : Subscriber{
public:
	EyePosDebug(PubSubHandler* p) : Subscriber(p) {
		Subscriber::SubscribeToTopic(EventTopic::MousePos);
	}

	vector<MousePosData> recievedMousePositions;
	MousePosData eyePos;
	MousePosData GetEyePos() { return eyePos; };
	virtual void readMessages();

	// Member Functions() 
	MousePosData calculateNewMousePos(MousePosData newMousePos);

};
