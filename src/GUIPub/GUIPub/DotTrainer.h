#pragma once
#include "Subscriber.h"
class DotTrainer :
	public Subscriber
{
public:
	DotTrainer(PubSubHandler* p);
	~DotTrainer();
	void StartThread(void);
	DWORD Run();

private:
	string fileName;
	char cPath[_MAX_PATH+1];
	void WriteDataToFile(float, float);
	void receiveMessage(EventMessage e);
	virtual void readMessages();
	static DWORD WINAPI startMethodInThread(LPVOID arg);
	int dotX;
	int dotY;
	bool trainingOn = false;

};

