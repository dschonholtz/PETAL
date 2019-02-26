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

DWORD DotTrainer::Run()
{	
	trainingOn = true;
	dotX = 50;
	dotY = 50;
	int GAP = 100;
	for (int i = 0; i < 5; i++) {
		dotY += GAP;
		dotX = 200;
		for (int j = 0; j < 10; j++) {
			//Create dot on GUI
			dotX += GAP;
			AddDotToDisplay(dotX, dotY);
			int sleep = 500;
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
	}
	EventMessage em;
	em.topic = TurnTrainingOff;
	bool data = true;
	em.data = &data;
	Publish(em);
	trainingOn = false;
	return 1;
}
