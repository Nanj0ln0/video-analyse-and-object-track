#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	VideoCapture capture;
	capture.open("D:/OpenCV/picture zone/video_004.avi");
	if (!capture.isOpened())
	{
		printf("ERROR");
		return -1;
	}

	Mat frame;
	Mat bsmaskMOG2 , bsmaskKNN;
	namedWindow("video",CV_WINDOW_AUTOSIZE);
	namedWindow("MOG2",CV_WINDOW_AUTOSIZE);
	namedWindow("KNN",CV_WINDOW_AUTOSIZE);

	Mat k = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));

	//��ʼ�������㷨
	Ptr<BackgroundSubtractor>pMOG2 = createBackgroundSubtractorMOG2();  //GMM  ͼ��ָ�ķ���
	Ptr<BackgroundSubtractor>pKNN = createBackgroundSubtractorKNN();  //KNN  ����ѧϰ�ķ���




	while (capture.read(frame))
	{
		imshow("video",frame);
		
		//GMM
		pMOG2->apply(frame,bsmaskMOG2);
		morphologyEx(bsmaskMOG2,bsmaskMOG2,MORPH_OPEN,k,Point(-1,-1));
		imshow("MOG2", bsmaskMOG2);
		
		//KNN
		pKNN->apply(frame,bsmaskKNN,-1); //���һ������ -1:�Զ�ѧϰ   0:���䶯ģ��  1:ÿһ֡����ʼ��һ��ģ��
		imshow("KNN", bsmaskKNN);


		char c = waitKey(40);
		if (c == 27)
		{
			break;
		}

	}

	capture.release();
	waitKey(0);
	return 0;
}