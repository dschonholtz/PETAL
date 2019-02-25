#include "stdafx.h"
#include "NeuralNet.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

void NeuralNet::readMessages() {
	EventMessage em = NeuralNet::getTopMessage();
	if (em.topic == Eye) {
		if (trainingOn) {
			std::vector<double> eyeData = *static_cast<std::vector<double> *>(em.data);
			mostRecentTrainingSet[2] = eyeData.at(0); // x
			mostRecentTrainingSet[3] = eyeData.at(1); // y
			mostRecentTrainingSet[4] = eyeData.at(2); // z
			mostRecentTrainingSet[5] = eyeData.at(3); // theta
			mostRecentTrainingSet[6] = eyeData.at(4); // phi
			recievedEyeData = true;
		}
	}
	else if (em.topic == AprilTag) {
		if (trainingOn) {
			recievedAprilTagData = true;
			// presumably will be setting values 7 - 19 with the rotational matrix.
		}
	}
	else if (em.topic == TrainingMousePos) {
		if (!trainingOn) {
			// clear the training Data file
			ofstream outfile;
			outfile.open("TrainingData.txt");
			outfile.close();
			trainingOn = true;
		}
		// set last value of training set that we are training for with this data
		MousePosTraining mpd = *static_cast<MousePosTraining *>(em.data);
		trainingMouseX = mpd.x;
		trainingMouseY = mpd.y;
	}
	else if (em.topic == TurnTrainingOff) {
		// dump the current neural net and train the neural network with what we have
		trainingOn = false;
		generateFannTrainingFile();
	}
	if (recievedEyeData && 
		//recievedAprilTagData &&
		trainingOn) {
		recievedEyeData = false;
		recievedAprilTagData = false;

		mostRecentTrainingSet[0] = trainingMouseX;
		mostRecentTrainingSet[1] = trainingMouseY;
		this->writeMostRecentTrainingSetToFile();
		trainingData.push_back(mostRecentTrainingSet);
	}
}

void NeuralNet::writeMostRecentTrainingSetToFile() {
	ofstream outfile;
	outfile.open("TrainingData.txt", std::ios_base::app);
	for (std::vector<double>::size_type i = 0; i != mostRecentTrainingSet.size(); i++) {
		double data = mostRecentTrainingSet.at(i);
		outfile << data << ',';
	}
	outfile << endl;
	outfile.close();
}

void NeuralNet::generateFannTrainingFile() {
	ofstream outfile;
	outfile.open("FannTrainingData.txt");
	outfile << trainingData.size() << ' ' << 5 << ' ' << 2 << endl;
	for (std::vector<vector<double>>::size_type i = 0; i != trainingData.size(); i++) {
		for (std::vector<double>::size_type i = 0; i != mostRecentTrainingSet.size(); i++) {
			double data = mostRecentTrainingSet.at(i);
			outfile << data << ' ';	
			if (i == 1) {
				outfile << endl;
			}
		}
		outfile << endl;
	}
	outfile.close();


	//ifstream myfile("TrainingData.txt");
	//string line;
	//if (myfile.is_open())
	//{
	//	while (getline(myfile, line))
	//	{
	//		std::vector<int> vect;
	//		// split the string on commas
	//		std::stringstream ss(line);

	//		int i;

	//		while (ss >> i)
	//		{
	//			vect.push_back(i);

	//			if (ss.peek() == ',')
	//				ss.ignore();
	//		}

	//		for (i = 0; i< vect.size(); i++)
	//			std::cout << vect.at(i) << std::endl;
	//	}
	//	myfile.close();
	//}
}