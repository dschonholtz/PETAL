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
	int sleep = 100;
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
		HorizontalDotMovement(dotX, dotY, GAP, GAP, ROWS, COLS); //Left -> Right
		HorizontalDotMovement(dotX, dotY, -GAP, -GAP, ROWS, COLS); //Right -> Left
		VerticalDotMovement(dotX, dotY, GAP, GAP, ROWS, COLS); // Up->down
		VerticalDotMovement(dotX , dotY, -GAP, GAP, ROWS, COLS); //Down-Up
	}

	EventMessage em;
	em.topic = TurnTrainingOff;
	bool data = true;
	em.data = &data;
	Publish(em);
	trainingOn = false;
	return 1;
}
