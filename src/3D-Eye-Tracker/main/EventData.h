#pragma once

#ifndef EVENT_DATA_H
#define EVENT_DATA_H

enum EventTopic {
	Eye,
	AprilTag,
	MousePos,
	TrainingMousePos,
	TurnTrainingOff
};

struct EventMessage {
	EventTopic topic;
	void* data;
};

struct MousePosData {
	int x;
	int y;
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