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
	int GAP = 100;
	dotX = 50;
	dotY = 25;
	bool backwards = false;
	for (int k = 0; k < 6; k++) {
		for (int i = 0; i < 7; i++) {
			if (i != 0) {
				if (backwards) {
					dotY -= GAP;
				}
				else {
					dotY += GAP;
				}
			}
			//dotX = 50;
			for (int j = 0; j < 12; j++) { 
				if (i % 2 == 0)
					if (backwards)
						dotX -= GAP;
					else
						dotX += GAP;
				else if(!backwards) {
					dotX -= GAP;
				}
				else {
					dotX += GAP;
				}
				AddDotToDisplay(dotX, dotY);
				int sleep = 1000;
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
		backwards = !backwards;
	}
	EventMessage em;
	em.topic = TurnTrainingOff;
	bool data = true;
	em.data = &data;
	Publish(em);
	trainingOn = false;
	return 1;
}
