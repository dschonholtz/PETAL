#pragma once
#include "Subscriber.h"
class DotTrainer :
	public Publisher
{
public:
	DotTrainer(PubSubHandler* p);
	~DotTrainer();
	void StartThread(void);
	DWORD Run();

private:
	char cPath[_MAX_PATH+1];
	static DWORD WINAPI startMethodInThread(LPVOID arg);
	int dotX;
	int dotY;
	bool trainingOn = false;
};

