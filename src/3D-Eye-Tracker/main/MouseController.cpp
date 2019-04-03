#include "stdafx.h"
#include <Windows.h>

// Member Functions() 

void MouseController::readMessages() {
	EventMessage em = MouseController::getTopMessage();
	if (em.topic == MousePos) {
		MousePosData * mpd = static_cast<MousePosData *>(em.data);
		MousePosData deepCopy;
		deepCopy.x = mpd->x;
		deepCopy.y = mpd->y;
		MousePosData newPoint =calculateNewMousePos(deepCopy);
		//MousePosData newPoint = MoveMouseRelativeToEyePos(deepCopy, screenWidth, screenHeight);
		setCursorPosition(newPoint.x, newPoint.y);
	}
}

MousePosData MouseController::calculateNewMousePos(MousePosData mpd) {
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
	newPoint.x = (sumX+(currentMousePos.x*recievedMousePositions.size())) / (recievedMousePositions.size()*2);
	newPoint.y = (sumY + (currentMousePos.y*recievedMousePositions.size())) / (recievedMousePositions.size()*2);
	return newPoint;
}

void MouseController::setCursorPosition(int x, int y)
{
	SetCursorPos(x, y);
}

MousePosData MouseController::MoveMouseRelativeToEyePos(MousePosData eyePos, int width, int height) {
	MouseVelocity* mv = new MouseVelocity();
	int PIXELBUFFER = 100;
	double speedScaleX = (eyePos.x / (double)width) * 3.0;
	double speedScaleY = (eyePos.y / (double)height) * 3.0;

	mv->xVel = (eyePos.x > width / 2 + PIXELBUFFER) ? mouseSpeed * speedScaleX : (eyePos.x < width / 2 - PIXELBUFFER) ? -mouseSpeed * speedScaleX : 0;
	mv->yVel = (eyePos.y > height / 2 + PIXELBUFFER) ? mouseSpeed * speedScaleY : (eyePos.x < height / 2 - PIXELBUFFER) ? -mouseSpeed * speedScaleY : 0;

	POINT currentMousePos;
	GetCursorPos(&currentMousePos);
	MousePosData newMousePos;
	newMousePos.x = currentMousePos.x + mv->xVel;
	newMousePos.y = currentMousePos.y + mv->yVel;

	return newMousePos;	
}