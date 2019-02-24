#include <direct.h>
#define GetCurrentDir _getcwd
// This may cause issues on MAC????

#include "stdafx.h"
#include "DotTrainer.h"
#include "Dot.h"
#include "GUIPub.h"
#include <stdio.h>  /* defines FILENAME_MAX */


DotTrainer::DotTrainer(PubSubHandler *p) : Subscriber(p) {
	this->SubscribeToTopic(AprilTagData);
	this->SubscribeToTopic(EyeData);
	GetModuleFileName(NULL, (LPWSTR)cPath, _MAX_PATH);
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
	int x = 200;
	int y = 50;
	int GAP = 100;
	for (int i = 0; i < 5; i++) {
		y += GAP;
		x = 200;
		for (int j = 0; j < 5; j++) {
			//Create dot on GUI
			x += GAP;
			AddDotToDisplay(x, y);
			this_thread::sleep_for(1s);
			ClearDot(x, y);
		}
	}
	return 1;
}

void DotTrainer::WriteDataToFile() {

}

void DotTrainer::readMessages() {

}
