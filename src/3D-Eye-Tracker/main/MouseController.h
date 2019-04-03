#pragma once
#include <windows.h>
#include "Subscriber.h"
#include "GUIPub.h"

struct MouseVelocity {
	int xVel;
	int yVel;
};

class MouseController: public Subscriber
{
	// https://docs.microsoft.com/en-us/windows/desktop/menurc/using-cursors

	// Access specifier 
public:
	MouseController(PubSubHandler* p) : Subscriber(p){
		GetDesktopResolution(screenWidth, screenHeight);
	}
	vector<MousePosData> recievedMousePositions;
	MousePosData currentMousePosition;
	int maxMouseMovementDistance = 10;
	void receiveMessage(EventMessage e);
	virtual void readMessages();
	// Member Functions() 
	void setCursorPosition(int x, int y);
	MousePosData calculateNewMousePos(MousePosData newMousePos);
	const static int BUFFER = 100;

private:
	int mouseSpeed = 10;
	MousePosData MoveMouseRelativeToEyePos(MousePosData eyePos, int width, int height);
	int screenWidth;
	int screenHeight;

};