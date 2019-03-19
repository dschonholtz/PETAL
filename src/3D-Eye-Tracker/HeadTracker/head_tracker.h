#ifndef HEAD_TRACKER_H
#define HEAD_TRACKER_H

#include <iostream>

#include <opencv2/opencv.hpp>

#include <apriltag.h>
#include <common/homography.h>
#include <tag36h11.h>
#include <tag36h10.h>
#include <tag36artoolkit.h>
#include <tag25h9.h>
#include <tag25h7.h>
#include <vector> 
#include "..\main\Publisher.h"

enum FamilyName
{
	tag36h11,
	tag36h10,
	tag36artoolkit,
	tag25h9,
	tag25h7
};

class HeadTracker : public Publisher
{
public:
	HeadTracker(bool, bool, FamilyName, int, int, double, double, bool, bool, bool, PubSubHandler*);
	~HeadTracker();
	void updatePosition();
	void Publish(EventMessage e);

private:
	FamilyName famname;
	cv::VideoCapture cap;
	apriltag_family_t *tf;
	apriltag_detector_t *td;
	cv::Mat frame;
	cv::Mat gray;
	cv::Mat pose;
	std::vector<double> vectPose;
};

#endif // HEAD_TRACKER_H