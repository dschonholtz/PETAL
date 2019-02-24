#include "stdafx.h"
#include "Dot.h"


Dot::Dot()
{
}

Dot::Dot(int x, int y) {
	xCord = x;
	yCord = y;
}

void Dot::UpdatePosition(int x, int y) {
	xCord = x;
	yCord = y;
}


Dot::~Dot()
{
}
