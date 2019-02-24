#pragma once

#ifndef EVENT_DATA_H
#define EVENT_DATA_H

enum EventTopic {
	Eye,
	AprilTag,
	MousePos
};

struct EventMessage {
	EventTopic topic;
	void* data;
};

struct MousePosData {
	int x;
	int y;
};

struct EyeData {
	int x;
	int y;
};



#endif