#pragma once
#ifndef EVENT_DATA_H
#define EVENT_DATA_H

struct EventMessage {
	EventTopic topic;
	void* data;
};

enum EventTopic {
	EyeData,
	AprilTagData,
	MousePos
};

#endif
