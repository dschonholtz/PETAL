#pragma once
#include "Subscriber.h"

class NeuralNet : public Subscriber
{
	// https://docs.microsoft.com/en-us/windows/desktop/menurc/using-cursors

	// Access specifier 
private:
	std::vector<double> mostRecentTrainingSet;
	bool recievedEyeData;
	bool recievedAprilTagData;
	bool trainingOn = false;
	int trainingMouseX;
	int trainingMouseY;

	struct fann *ann;
	FANN::neural_net net;
	std::vector<std::vector<double>> trainingData;

	void writeMostRecentTrainingSetToFile();
	void generateFannTrainingFile();
	void trainNeuralNetwork();
	void loadNeuralNetworkFromFile();

public:

	NeuralNet::NeuralNet(PubSubHandler* p) : Subscriber(p) {
		vector<double> vect(7, 0);
		mostRecentTrainingSet = vect;
	}

	void receiveMessage(EventMessage e);
	virtual void readMessages();
	// Member Functions() 
	void setCursorPosition(int x, int y);
};