#pragma once

#ifndef EVENT_DATA_H
#define EVENT_DATA_H

enum EventTopic {
	Eye,
	AprilTag,
	MousePos,
	TrainingMousePos,
	TurnTrainingOff,
	LoadNeuralNetworkFromFile,
	LoadTrainingDataFromFile,
};

struct EventMessage {
	EventTopic topic;
	void* data;
};

struct MousePosData {
	int x = 125;
	int y = 125;
};

struct MousePosTraining {
	int x;
	int y;
	int ms;
};

struct EyeData {
	int x;
	int y;
};


#endif