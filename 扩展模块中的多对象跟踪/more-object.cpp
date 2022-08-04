#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//������Ŀ׷����
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


	//��ȡ��Ƶ�ĵ�һ֡
	capture.read(frame);

	//�ڵ�һ֡�ж�λ����
	vector <Rect> bboxes;
	bool showCrosshair = true;
	bool fromCenter = false;
	selectROIs("tracker demo", frame, bboxes, showCrosshair, fromCenter);
	
	if (bboxes.size() < 1)
	{
		return 0;
	}


	//��ʼ����Ŀ��׷����
	Ptr<MultiTracker> multiTracker = MultiTracker::create();
	

	//���add����ĵ�һ���������õ�Ŀ��׷��������bug 
	//ֻ���õ��ú�����ʽʵ�֣�����ֱ�Ӷ����ò���
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