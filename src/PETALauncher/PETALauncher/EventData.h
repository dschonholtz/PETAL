#pragma once

#ifndef EVENT_DATA_H
#define EVENT_DATA_H

struct EventMessage {
	EventTopic topic;
	void* data;
};

struct MousePosData {
	int x;
	int y;
};

enum EventTopic {
	EyeData,
	AprilTagData,
	MousePos
};

#endif