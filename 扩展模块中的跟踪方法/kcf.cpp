#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	VideoCapture capture;
	capture.open("D:/OpenCV/picture zone/quan.mkv");
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}

	Mat frame;
	namedWindow("tracker demo", CV_WINDOW_AUTOSIZE);

	//<TrackerMedianFlow> 更改这里的东西  KCF  boostiong
	Ptr<TrackerMedianFlow> tracker = TrackerMedianFlow::create();   
	capture.read(frame);
	Rect2d roi = selectROI("tracker demo", frame);
	if (roi.width == 0 || roi.height == 0)
	{
		return -1;
	}
	tracker->init(frame, roi);  //学习

	while (capture.read(frame)) {
		tracker->update(frame, roi);  //跟踪
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 8, 0);  //画框框

		imshow("tracker demo", frame);
		char c = waitKey(100);
		if (c == 27) {
			break;
		}
	}


	capture.release();
	waitKey(0);
	return 0;
}