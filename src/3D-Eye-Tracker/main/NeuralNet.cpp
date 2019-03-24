#include "stdafx.h"
#include "NeuralNet.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <doublefann.h>
#include <fann_cpp.h>

FANN::neural_net net;

// Callback function that simply prints the information to cout
int print_callback(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs, void *user_data)
{
	//cout << "Epochs     " << setw(8) << epochs << ". "
	//	<< "Current Error: " << left << net.get_MSE() << right << endl;
	return 0;
}

void NeuralNet::readMessages() {
	EventMessage em = NeuralNet::getTopMessage();
	if (em.topic == Eye) {
		std::vector<double> eyeData = *static_cast<std::vector<double> *>(em.data);
		mostRecentData[0] = eyeData.at(0); // x
		mostRecentData[1] = eyeData.at(1); // y
		mostRecentData[2] = eyeData.at(2); // z
		mostRecentData[3] = eyeData.at(3); // theta
		mostRecentData[4] = eyeData.at(4); // phi
		if (trainingOn) {
			if (mostRecentData[0] != 0 && mostRecentData[1] != 0 && mostRecentData[2] != 0 &&
				mostRecentData[3] != 0 && mostRecentData[4] != 0) {
				recievedEyeData = true;
			}
		}
		else if (networkDoneTraining) {
			//std::vector<double> eyeData = *static_cast<std::vector<double> *>(em.data);
			double * doubles = new double[VEC_SIZE];
			for (int i = 0; i < VEC_SIZE; i++) {
				doubles[i] = mostRecentData[i];
			}
			//double* doubles = &eyeData[0];
			net.scale_input(doubles);
			fann_type* result = net.run(doubles);
			net.descale_output(result);

			EventMessage newMousePos;
			newMousePos.topic = MousePos;
			MousePosData mpd;
			mpd.x = (int)result[0];
			mpd.y = (int)result[1];
			newMousePos.data = &mpd;
			Publish(newMousePos);
		}
	}
	else if (em.topic == AprilTag) {
		aprilTagData = *static_cast<std::vector<double> *>(em.data);
		recievedAprilTagData = true;
		for (int i = 0; i < aprilTagData.size(); i++) {
			mostRecentData[5+i] = aprilTagData.at(i);
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
		trainNeuralNetwork();
	}
	else if (em.topic == LoadNeuralNetworkFromFile) {
		loadNeuralNetworkFromFile();
	}
	if (recievedEyeData &&
		//recievedAprilTagData &&
		trainingOn) {
		recievedEyeData = false;
		recievedAprilTagData = false;

		mostRecentData[VEC_SIZE] = trainingMouseX;
		mostRecentData[VEC_SIZE + 1] = trainingMouseY;
		this->writeMostRecentTrainingSetToFile();
		trainingData.push_back(mostRecentData);
	}
}

void NeuralNet::writeMostRecentTrainingSetToFile() {
	ofstream outfile;
	outfile.open("TrainingData.txt", std::ios_base::app);
	for (std::vector<double>::size_type i = 0; i != mostRecentData.size(); i++) {
		double data = mostRecentData.at(i);
		outfile << data << ',';
	}
	outfile << endl;
	outfile.close();
}

void NeuralNet::trainNeuralNetwork() {
	//cout << endl << "Eye Tracking neuralNet started." << endl;

	const float learning_rate = 0.1f;
	const unsigned int num_layers = 5;
	const float desired_error = 0.10f;
	const unsigned int max_iterations = 300000;
	const unsigned int iterations_between_reports = 1000;

	unsigned int layers[5] = { VEC_SIZE, 20, 12, 7, 2 };
	net.create_standard_array(num_layers, layers);

	net.set_learning_rate(learning_rate);

	net.set_activation_steepness_hidden(1.0);
	net.set_activation_steepness_output(1.0);

	net.set_activation_function_hidden(FANN::LINEAR);
	net.set_activation_function_output(FANN::LINEAR);

	// Set additional properties such as the training algorithm
	//net.set_training_algorithm(FANN::TRAIN_QUICKPROP);

	// Output network type and parameters
	//cout << endl << "Network Type                         :  ";
	switch (net.get_network_type())
	{
	case FANN::LAYER:
		cout << "LAYER" << endl;
		break;
	case FANN::SHORTCUT:
		cout << "SHORTCUT" << endl;
		break;
	default:
		cout << "UNKNOWN" << endl;
		break;
	}
	net.print_parameters();

	//cout << endl << "Training network." << endl;

	FANN::training_data data;
	if (data.read_train_from_file("FannTrainingData.txt"))
	{
		net.set_scaling_params(data, -1, 1, -1, 1);
		net.scale_train(data);
		data.save_train("FannScaledTrainingData.data");
		// Initialize and train the network with the data
		net.init_weights(data);

		net.train_on_data(data, max_iterations,
			iterations_between_reports, desired_error);
		networkDoneTraining = true;

		for (unsigned int i = 0; i < data.length_train_data(); ++i)
		{
			// Run the network on the test data
			fann_type *calc_out = net.run(data.get_input()[i]);

			//cout << "XOR test (" << showpos << data.get_input()[i][0] << ", "
			//	<< data.get_input()[i][1] << ") -> " << *calc_out
			//	<< ", should be " << data.get_output()[i][0] << ", "
			//	<< "difference = " << noshowpos
			//	<< fann_abs(*calc_out - data.get_output()[i][0]) << endl;
		}

		//cout << endl << "Saving network." << endl;

		// Save the network in floating point and fixed point
		net.save("FannNetwork.net");
		//unsigned int decimal_point = net.save_to_fixed("FannTrainingFixed.net");
		//data.save_train_to_fixed("FannTrainingData_fixed.data", decimal_point);
	}
}

void NeuralNet::loadNeuralNetworkFromFile() {
	net.create_from_file("FannNetwork.net");
	networkDoneTraining = true;
}

void NeuralNet::generateFannTrainingFile() {
	ofstream outfile;
	outfile.open("FannTrainingData.txt");
	outfile << trainingData.size() << ' ' << VEC_SIZE << ' ' << 2 << endl;
	for (std::vector<vector<double>>::size_type i = 0; i != trainingData.size(); i++) {
		for (std::vector<double>::size_type j = 0; j != mostRecentData.size(); j++) {
			double data = trainingData.at(i).at(j);
			outfile << data << ' ';
			if (j == VEC_SIZE - 1) {
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