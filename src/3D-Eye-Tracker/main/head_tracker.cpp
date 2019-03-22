#include "head_tracker.h"
#include <vector> 

// Constructor
//HeadTracker::HeadTracker(bool debug, bool quiet, FamilyName family, int border, int threads, double decimate, double blur, bool refine_edges, bool refine_decode, bool refine_pose, PubSubHandler* p) : cap(0)
HeadTracker::HeadTracker(int index, bool debug, bool quiet, FamilyName family, int border, int threads, double decimate, double blur, bool refine_edges, bool refine_decode, bool refine_pose, PubSubHandler* p) : Publisher(p)
{
	//getopt_t *getopt = getopt_create();

	//getopt_add_bool(getopt, 'h', "help", 0, "Show this help");
	//getopt_add_bool(getopt, 'd', "debug", 0, "Enable debugging output (slow)");
	//getopt_add_bool(getopt, 'q', "quiet", 0, "Reduce output");
	//getopt_add_string(getopt, 'f', "family", "tag36h11", "Tag family to use");
	//getopt_add_int(getopt, '\0', "border", "1", "Set tag family border size");
	//getopt_add_int(getopt, 't', "threads", "4", "Use this many CPU threads");
	//getopt_add_double(getopt, 'x', "decimate", "1.0", "Decimate input image by this factor");
	//getopt_add_double(getopt, 'b', "blur", "0.0", "Apply low-pass blur to input");
	//getopt_add_bool(getopt, '0', "refine-edges", 1, "Spend more time trying to align edges of tags");
	//getopt_add_bool(getopt, '1', "refine-decode", 0, "Spend more time trying to decode tags");
	//getopt_add_bool(getopt, '2', "refine-pose", 0, "Spend more time trying to precisely localize tags");

	//if (!getopt_parse(getopt, argc, argv, 1) ||
	//	getopt_get_bool(getopt, "help")) {
	//	printf("Usage: %s [options]\n", argv[0]);
	//	getopt_do_usage(getopt);
	//	exit(0);
	//}
	if (!cap.open(1)) {  // open and check if succeeded
		throw "Couldn't open cap\n";
		return;
	};
	// Initialize camera
	if (!cap.isOpened()) {
		throw "Couldn't open video capture device";
		return;
	}
	// Initialize tag detector with options
	tf = NULL;
	famname = family;
	switch (famname)
	{
	case tag36h11:
		tf = tag36h11_create();
		break;
	case tag36h10:
		tf = tag36h10_create();
		break;
	case tag36artoolkit:
		tf = tag36artoolkit_create();
		break;
	case tag25h9:
		tf = tag25h9_create();
		break;
	case tag25h7:
		tf = tag25h7_create();
		break;
	default:
		throw "Unrecognized tag family name. Use e.g. \"tag36h11\".";
	}
	tf->black_border = border;

	td = apriltag_detector_create();
	apriltag_detector_add_family(td, tf);
	td->quad_decimate = decimate;
	td->quad_sigma = blur;
	td->nthreads = threads;
	td->debug = debug;
	td->refine_edges = refine_edges;
	td->refine_decode = refine_decode;
	td->refine_pose = refine_pose;

	return;
}

// Destructor
HeadTracker::~HeadTracker()
{
	apriltag_detector_destroy(td);
	switch (famname)
	{
	case tag36h11:
		tag36h11_destroy(tf);
		break;
	case tag36h10:
		tag36h10_destroy(tf);
		break;
	case tag36artoolkit:
		tag36artoolkit_destroy(tf);
		break;
	case tag25h9:
		tag25h9_destroy(tf);
		break;
	case tag25h7:
		tag25h7_destroy(tf);
		break;
	default:
		throw "Unrecognized tag family name. Use e.g. \"tag36h11\".";
	}

	return;
}

void HeadTracker::updatePosition()
{
	cap >> frame;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

	// Make an image_u8_t header for the Mat data
#ifdef _MSC_VER
	image_u8_t im{ gray.cols, gray.rows, gray.cols, gray.data };
#else
	image_u8_t im = { .width = gray.cols,
		.height = gray.rows,
		.stride = gray.cols,
		.buf = gray.data
	};
#endif

	zarray_t *detections = apriltag_detector_detect(td, &im);
	//cout << zarray_size(detections) << " tags detected" << endl;

	// Draw detection outlines
	for (int i = 0; i < zarray_size(detections); i++) {
		apriltag_detection_t *det;
		zarray_get(detections, i, &det);
		line(frame, cv::Point(det->p[0][0], det->p[0][1]),
			cv::Point(det->p[1][0], det->p[1][1]),
			cv::Scalar(0, 0xff, 0), 2);
		line(frame, cv::Point(det->p[0][0], det->p[0][1]),
			cv::Point(det->p[3][0], det->p[3][1]),
			cv::Scalar(0, 0, 0xff), 2);
		line(frame, cv::Point(det->p[1][0], det->p[1][1]),
			cv::Point(det->p[2][0], det->p[2][1]),
			cv::Scalar(0xff, 0, 0), 2);
		line(frame, cv::Point(det->p[2][0], det->p[2][1]),
			cv::Point(det->p[3][0], det->p[3][1]),
			cv::Scalar(0xff, 0, 0), 2);

		cv::Mat H(cv::Size(3, 3), CV_64F);
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				double var = det->H->data[3 * j + k];
				H.at<double>(j, k) = det->H->data[3 * j + k];
			}
		}

		// Normalization to ensure that ||c1|| = 1
		double norm = sqrt(H.at<double>(0, 0)*H.at<double>(0, 0) +
			H.at<double>(1, 0)*H.at<double>(1, 0) +
			H.at<double>(2, 0)*H.at<double>(2, 0));
		H /= norm;
		cv::Mat c1 = H.col(0);
		cv::Mat c2 = H.col(1);
		cv::Mat c3 = c1.cross(c2);
		cv::Mat tvec = H.col(2);
		cv::Mat R(3, 3, CV_64F);
		for (int i = 0; i < 3; i++)
		{
			R.at<double>(i, 0) = c1.at<double>(i, 0);
			R.at<double>(i, 1) = c2.at<double>(i, 0);
			R.at<double>(i, 2) = c3.at<double>(i, 0);
		}

		std::cout << "R (before polar decomposition):\n" << R << "\ndet(R): " << determinant(R) << std::endl;
		cv::Mat W, U, Vt;
		SVDecomp(R, W, U, Vt);
		R = U * Vt;
		std::cout << "R (after polar decomposition):\n" << R << "\ndet(R): " << determinant(R) << std::endl;
		std::cout << "tvec: \n" << tvec << std::endl;
		pose = R;

		for (int j = 0; j < 3; j++) 
		{
			vectPose.push_back(R.at<double>(i, 0));
			vectPose.push_back(R.at<double>(i, 1));
			vectPose.push_back(R.at<double>(i, 2));
		}

		EventMessage msg;
		msg.data = static_cast<void*>(&vectPose[0]);
		msg.topic = AprilTag;
		HeadTracker::Publish(msg);

		std::stringstream ss;
		ss << det->id;
		cv::String text = ss.str();
		int fontface = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
		double fontscale = 1.0;
		int baseline;
		cv::Size textsize = cv::getTextSize(text, fontface, fontscale, 2,
			&baseline);
		putText(frame, text, cv::Point(det->c[0] - textsize.width / 2,
			det->c[1] + textsize.height / 2),
			fontface, fontscale, cv::Scalar(0xff, 0x99, 0), 2);
	}
	zarray_destroy(detections);
	cv::imshow("Tag Detections", frame);
	return;
}

void HeadTracker::Publish(EventMessage e)
{
	Publisher::Publish(e);
}


