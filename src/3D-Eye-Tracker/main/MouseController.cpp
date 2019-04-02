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
		MousePosData newPoint = calculateNewMousePos(deepCopy);
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

	mv->xVel = (eyePos.x > width / 2 + PIXELBUFFER) ? mouseSpeed : (eyePos.x < width / 2 - PIXELBUFFER) ? -mouseSpeed : 0;
	mv->yVel = (eyePos.y > height / 2 + PIXELBUFFER) ? mouseSpeed : (eyePos.x < height / 2 - PIXELBUFFER) ? -mouseSpeed : 0;

	POINT currentMousePos;
	GetCursorPos(&currentMousePos);
	MousePosData newMousePos;
	newMousePos.x = currentMousePos.x + mv->xVel;
	newMousePos.y = currentMousePos.y + mv->yVel;

	return newMousePos;	
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}