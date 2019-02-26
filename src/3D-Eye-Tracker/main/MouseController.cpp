#include "stdafx.h"
#include <Windows.h>

// Member Functions() 

void MouseController::readMessages() {
	EventMessage em = MouseController::getTopMessage();
	if (em.topic == MousePos) {
		MousePosData * mpd = static_cast<MousePosData *>(em.data);
		MousePosData newPoint = calculateNewMousePos(mpd);
		setCursorPosition(newPoint.x, newPoint.y);
	}
}

MousePosData MouseController::calculateNewMousePos(MousePosData * mpd) {
	if (recievedMousePositions.size() > 10) {
		recievedMousePositions.front() = mpd;
	}
	else {
		recievedMousePositions.push_back(mpd);
	}
	MousePosData newPoint;
	int sumX = 0;
	int sumY = 0;
	for (int i = 0; i < recievedMousePositions.size(); i++) {
		sumX += recievedMousePositions[i]->x;
		sumY += recievedMousePositions[i]->y;
	}
	POINT currentMousePos;
	GetCursorPos(&currentMousePos);

	newPoint.x = (sumX+(currentMousePos.x*recievedMousePositions.size())) / (recievedMousePositions.size()*2);
	newPoint.y = (sumY + (currentMousePos.y*recievedMousePositions.size())) / (recievedMousePositions.size()*2);
	return newPoint;
}

void MouseController::setCursorPosition(int x, int y)
{
	SetCursorPos(x, y);
}