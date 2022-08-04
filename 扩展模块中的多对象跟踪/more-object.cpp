#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//创建单目追踪器
Ptr<Tracker> createTracker()
{
	Ptr<Tracker> tracker = TrackerCSRT::create();

	return tracker;
}


int main(int argc, char** argv) {
	VideoCapture capture;
	capture.open("D:/OpenCV/picture zone/mouse.mp4");  //quan.mkv   video_003.avi  mouse.mp4
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}

	Mat frame;
	namedWindow("tracker demo", CV_WINDOW_AUTOSIZE);


	//读取视频的第一帧
	capture.read(frame);

	//在第一帧中定位物体
	vector <Rect> bboxes;
	bool showCrosshair = true;
	bool fromCenter = false;
	selectROIs("tracker demo", frame, bboxes, showCrosshair, fromCenter);
	
	if (bboxes.size() < 1)
	{
		return 0;
	}


	//初始化多目标追踪器
	Ptr<MultiTracker> multiTracker = MultiTracker::create();
	

	//这边add后面的第一个参数调用单目标追踪器，有bug 
	//只能用调用函数方式实现，其他直接定义用不了
	for (int i = 0; i < bboxes.size(); i++)
	{
		multiTracker->add(createTracker(), frame, Rect2d(bboxes[i]));
	}


	while (capture.isOpened())
	{
		capture >> frame;
		multiTracker->update(frame);
		
		for (unsigned t = 0; t < multiTracker->getObjects().size(); t++) 
		{
			rectangle(frame, multiTracker->getObjects()[t],Scalar(0,0,255), 2, 1);
		}

		imshow("tracker demo", frame);
		char c = waitKey(30);
		if (c == 27) 
		{
			break;
		}
	}


	capture.release();
	waitKey(0);
	return 0;
}