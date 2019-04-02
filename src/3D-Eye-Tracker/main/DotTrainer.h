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
	void VerticalDotMovement(int &dotX, int &dotY, int x_gap, int y_gap, int rows, int cols);
	void HorizontalDotMovement(int &dotX, int &dotY, int x_gap, int y_gap, int rows, int cols);
	void DrawDot(int dotX, int dotY);
	void DetermineGap(int &x_gap, int& y_gap, int START_X, int START_Y, int curX, int curY, int GAP);

private:
	char cPath[_MAX_PATH+1];
	static DWORD WINAPI startMethodInThread(LPVOID arg);
	int dotX;
	int dotY;
	bool trainingOn = false;
};

