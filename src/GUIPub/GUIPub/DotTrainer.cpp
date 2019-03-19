#include <direct.h>
#include "stdafx.h"
#include "DotTrainer.h"
#include "Dot.h"
#include "GUIPub.h"
#include <fstream>
#include <sstream>


DotTrainer::DotTrainer(PubSubHandler *p) : Subscriber(p) {
	this->SubscribeToTopic(AprilTag);
	this->SubscribeToTopic(Eye);
	GetModuleFileName(NULL, (LPWSTR)cPath, _MAX_PATH);
	srand(time(NULL));
	ostringstream os;
	os << "Training data #" << rand() % 100000 << ".txt";
	fileName = os.str();
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
	dotX = 200;
	dotY = 50;
	int GAP = 100;
	for (int i = 0; i < 5; i++) {
		dotY += GAP;
		dotX = 200;
		for (int j = 0; j < 5; j++) {
			//Create dot on GUI
			dotX += GAP;
			AddDotToDisplay(dotX, dotY);
			this_thread::sleep_for(1s);
			ClearDot(dotX, dotY);
		}
	}
	trainingOn = false;
	return 1;
}

void DotTrainer::WriteDataToFile(float eyeX, float eyeY) {
	ofstream newFile;
	newFile.open(fileName);
	ostringstream line;
	line << eyeX << ',' << eyeY << ',' << dotX << ',' << dotY << '\n';
	newFile << line.str();
	newFile.close();
}

void DotTrainer::readMessages() {
	if (trainingOn) {
		EventMessage em = DotTrainer::getTopMessage();
		if (em.topic == Eye) {
			EyeData * ed = static_cast<EyeData *>(em.data);
			WriteDataToFile(ed->x, ed->y);
		}
	}
	else {
		DotTrainer::emptyQueue();
	}
}
