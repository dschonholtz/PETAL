#include "stdafx.h"
#include "EyePosDebug.h"

// Member Functions() 

void EyePosDebug::readMessages() {
	EventMessage em = EyePosDebug::getTopMessage();
	if (em.topic == MousePos) {
		MousePosData * mpd = static_cast<MousePosData *>(em.data);
		MousePosData deepCopy;
		deepCopy.x = mpd->x;
		deepCopy.y = mpd->y;
		eyePos = calculateNewMousePos(deepCopy);
	}
}

MousePosData EyePosDebug::calculateNewMousePos(MousePosData mpd) {
	if (recievedMousePositions.size() > 3) {
		recievedMousePositions.erase(recievedMousePositions.begin() + 3);
	}
	recievedMousePositions.insert(recievedMousePositions.begin(), mpd);
	MousePosData newPoint;
	int sumX = 0;
	int sumY = 0;
	for (int i = 0; i < recievedMousePositions.size(); i++) {
		sumX += recievedMousePositions[i].x;
		sumY += recievedMousePositions[i].y;
	}
	POINT currentMousePos;
	GetCursorPos(&currentMousePos);
	newPoint.x = (sumX + (currentMousePos.x*recievedMousePositions.size())) / (recievedMousePositions.size() * 2);
	newPoint.y = (sumY + (currentMousePos.y*recievedMousePositions.size())) / (recievedMousePositions.size() * 2);
	return newPoint;
}
