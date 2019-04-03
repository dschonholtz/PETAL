#include <direct.h>
#include "stdafx.h"
#include "DotTrainer.h"
#include "GUIPub.h"
#include <fstream>
#include <sstream>
#include "GUIPub.h"


DotTrainer::DotTrainer(PubSubHandler *p) : Publisher(p) {
	GetModuleFileName(NULL, cPath, _MAX_PATH);
	srand(time(NULL));
	GetGapViaScreenRes(cols, rows);
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
	AddDotToDisplay(dotX, dotY, 255, 0,0);
	int sleep = 200;
	MousePosTraining mousePosTraining;
	mousePosTraining.x = dotX;
	mousePosTraining.y = dotY;
	mousePosTraining.ms = sleep;
	EventMessage eventMessage;
	eventMessage.topic = TrainingMousePos;
	eventMessage.data = &mousePosTraining;
	Publish(eventMessage);
	this_thread::sleep_for(std::chrono::milliseconds(sleep));
	AddDotToDisplay(dotX, dotY, 0,0,0);
}

void DotTrainer::HorizontalDotMovement(int& dotX, int& dotY, int x_gap, int y_gap) {
	dotY -= y_gap;
	for (int i = 0; i < rows; i++) {
		dotY += y_gap;
		DrawDot(dotX, dotY);
		for (int j = 0; j < cols - 1; j++) {
			dotX += x_gap;			
			DrawDot(dotX, dotY);
		}

		x_gap *= -1;
	}
}

void DotTrainer::VerticalDotMovement(int& dotX, int& dotY, int x_gap, int y_gap) {
	dotX -= x_gap;
	for (int i = 0; i < cols; i++) {
		dotX += x_gap;
		DrawDot(dotX, dotY);
		for (int j = 0; j < rows - 1 ; j++) {
			dotY += y_gap;
			DrawDot(dotX, dotY);
		}
		y_gap *= -1;
	}
}

DWORD DotTrainer::Run()
{	
	HideButtons();
	if (trainingOn) return 0;

	int NUMITERATIONS = 1;
	trainingOn = true;
	int GAP = 70;
	int START_X = 50;
	int START_Y = 25;
	bool backwards = false;
	int dotX = START_X;
	int dotY = START_Y;
	int a = rows;

	//Right -> Left
	for (int i = 0; i < NUMITERATIONS; i++) {
		int x_gap = GAP;
		int y_gap = GAP;
		HorizontalDotMovement(dotX, dotY, x_gap, y_gap); //Left -> Right
		DetermineGap(x_gap, y_gap, START_X, START_Y, dotX, dotY, GAP);

		HorizontalDotMovement(dotX, dotY, x_gap, y_gap); //Right -> Left
		DetermineGap(x_gap, y_gap, START_X, START_Y, dotX, dotY, GAP);

		VerticalDotMovement(dotX, dotY, x_gap, y_gap); // Up->down
		DetermineGap(x_gap, y_gap, START_X, START_Y, dotX, dotY, GAP);

		VerticalDotMovement(dotX , dotY, x_gap, y_gap); //Down-Up
	}

	ClearAllDots(dotX, dotY, GAP, GAP);
	ShowButtons();
	EventMessage em;
	em.topic = TurnTrainingOff;
	bool data = true;
	em.data = &data;
	Publish(em);
	trainingOn = false;
	return 1;
}

void DotTrainer::ClearAllDots(int dotX, int dotY, int x_gap, int y_gap) {
	dotY -= y_gap;
	for (int i = 0; i < rows; i++) {
		dotY += y_gap;
		ClearDot(dotX, dotY);
		for (int j = 0; j < cols - 1; j++) {
			dotX += x_gap;
			ClearDot(dotX, dotY);
		}
		x_gap *= -1;
	}
}

void DotTrainer::DetermineGap(int &x_gap, int& y_gap, int START_X, int START_Y, int curX, int curY, int GAP) {
	x_gap = (curX == START_X) ? GAP : -GAP;
	y_gap = (curY == START_Y) ? GAP : -GAP;
}

void DotTrainer::GetGapViaScreenRes(int& rows, int& cols) {
	int screenWidth;
	int screenHeight;
	GetDesktopResolution(screenWidth, screenHeight);

	rows = screenWidth / 70;
	cols = screenHeight / 70; // gap is small enough that the bar at the top of the application pushes this off the screen with gap of 50
}