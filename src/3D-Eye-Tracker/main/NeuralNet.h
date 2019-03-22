#pragma once
#include "Subscriber.h"
#include "Publisher.h"
#include <doublefann.h>
#include <fann_cpp.h>

class NeuralNet : public Subscriber, Publisher
{
	// https://docs.microsoft.com/en-us/windows/desktop/menurc/using-cursors

	// Access specifier 
private:
	std::vector<double> mostRecentData;
	bool recievedEyeData = false;
	bool recievedAprilTagData = false;
	bool trainingOn = false;

	std::vector<double> eyeData;
	std::vector<double> aprilTagData;

	bool networkDoneTraining = false;

	int trainingMouseX;
	int trainingMouseY;

	struct fann *ann;
	std::vector<std::vector<double>> trainingData;

	void writeMostRecentTrainingSetToFile();
	void generateFannTrainingFile();
	void trainNeuralNetwork();
	void loadNeuralNetworkFromFile();

public:

	NeuralNet::NeuralNet(PubSubHandler* p) : Subscriber(p), Publisher(p) {
		vector<double> vect(16, 0);
		mostRecentData = vect;
	}

	void receiveMessage(EventMessage e);
	virtual void readMessages();
	// Member Functions() 
	void setCursorPosition(int x, int y);
};