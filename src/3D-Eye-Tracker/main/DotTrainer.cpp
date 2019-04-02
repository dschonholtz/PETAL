#include <direct.h>
#include "stdafx.h"
#include "DotTrainer.h"
#include "GUIPub.h"
#include <fstream>
#include <sstream>


DotTrainer::DotTrainer(PubSubHandler *p) : Publisher(p) {
	GetModuleFileName(NULL, cPath, _MAX_PATH);
	srand(time(NULL));
}

DotTrainer::~DotTrainer()
{
}

DWORD WINAPI DotTrainer::startMethodInThread(LPVOID arg)
{
	if (!arg)
		return 0;
	DotTrainer *ptr = (DotTrainer*)arg;
	ptr->Run();
	return 1;
}

void DotTrainer::StartThread(void)
{
	CreateThread(NULL, 4096, startMethodInThread, this, 0, NULL);
}

void DotTrainer::DrawDot(int dotX, int dotY) {
	AddDotToDisplay(dotX, dotY);
	int sleep = 75;
	MousePosTraining mousePosTraining;
	mousePosTraining.x = dotX;
	mousePosTraining.y = dotY;
	mousePosTraining.ms = sleep;
	EventMessage eventMessage;
	eventMessage.topic = TrainingMousePos;
	eventMessage.data = &mousePosTraining;
	Publish(eventMessage);
	this_thread::sleep_for(std::chrono::milliseconds(sleep));
	ClearDot(dotX, dotY);
}

void DotTrainer::HorizontalDotMovement(int& dotX, int& dotY, int x_gap, int y_gap, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			dotX += x_gap;			
			DrawDot(dotX, dotY);
		}
		dotY += y_gap;
		DrawDot(dotX, dotY);
		x_gap *= -1;
	}
}

void DotTrainer::VerticalDotMovement(int& dotX, int& dotY, int x_gap, int y_gap, int rows, int cols) {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			dotY += y_gap;
			DrawDot(dotX, dotY);
		}
		dotX += x_gap;
		DrawDot(dotX, dotY);
		y_gap *= -1;
	}
}

DWORD DotTrainer::Run()
{	
	if (trainingOn) return 0;

	int NUMITERATIONS = 2;
	trainingOn = true;
	int GAP = 100;
	int START_X = 50;
	int START_Y = 25;
	bool backwards = false;
	int ROWS = 7;
	int COLS = 12;
	int dotX = START_X;
	int dotY = START_Y;

	//Right -> Left
	for (int i = 0; i < NUMITERATIONS; i++) {
		int x_gap = GAP;
		int y_gap = GAP;
		HorizontalDotMovement(dotX, dotY, x_gap, y_gap, ROWS, COLS); //Left -> Right
		DetermineGap(x_gap, y_gap, START_X, START_Y, dotX, dotY, GAP);

		HorizontalDotMovement(dotX, dotY, x_gap, y_gap, ROWS, COLS); //Right -> Left
		DetermineGap(x_gap, y_gap, START_X, START_Y, dotX, dotY, GAP);

		VerticalDotMovement(dotX, dotY, x_gap, y_gap, ROWS, COLS); // Up->down
		DetermineGap(x_gap, y_gap, START_X, START_Y, dotX, dotY, GAP);

		VerticalDotMovement(dotX , dotY, x_gap, y_gap, ROWS, COLS); //Down-Up
	}

	EventMessage em;
	em.topic = TurnTrainingOff;
	bool data = true;
	em.data = &data;
	Publish(em);
	trainingOn = false;
	return 1;
}

void DotTrainer::DetermineGap(int &x_gap, int& y_gap, int START_X, int START_Y, int curX, int curY, int GAP) {
	x_gap = (curX == START_X) ? GAP : -GAP;
	y_gap = (curY == START_Y) ? GAP : -GAP;
}
