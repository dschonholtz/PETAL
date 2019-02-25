#include "stdafx.h"

// Member Functions() 

void MouseController::readMessages() {
	EventMessage em = MouseController::getTopMessage();
	if (em.topic == MousePos) {
		MousePosData * mpd = static_cast<MousePosData *>(em.data);
		setCursorPosition(mpd->x, mpd->y);
	}
}

void MouseController::setCursorPosition(int x, int y)
{
	SetCursorPos(x, y);
}