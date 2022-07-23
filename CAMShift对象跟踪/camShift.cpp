#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <math.h>

using namespace cv;
using namespace std;

//����֡ , ɫ�ʿռ� ,hue ,mask, ֱ��ͼ,ֱ��ͼ����ӳ��
Mat frame , hsv , hue , mask , hist , backproject;
Mat drawImg = Mat::zeros(300,300,CV_8UC3);

int smin = 15;
int smax = 256;
int vmin = 40;
int vmax = 256;

int bins = 16; //ֱ��ͼ����
float hrange[] = {0,180};
const float* hranges = hrange;

int main() {
	VideoCapture capture;
	capture.open("D:/OpenCV/picture zone/quan.mkv");
	if (!capture.isOpened())
	{
		printf("video is not open");
		return -1;
	}
	
	namedWindow("video", CV_WINDOW_AUTOSIZE);
	namedWindow("Hist", CV_WINDOW_AUTOSIZE);

	bool firstTimes = true;
	Rect selection;

	while (capture.read(frame))
	{
		if (firstTimes)
		{
			//��չģ�������ѡ��ROI�����api������һ�����ڣ���������ʲôͼ��
			Rect2d first = selectROI("video",frame);
			selection.x = first.x;
			selection.y = first.y;
			selection.width = first.width;
			selection.height = first.height;
			printf("ROI.x = %d \n ROI.y = %d \n  width = %d \n  height = %d \n",selection.x, selection.y, selection.width, selection.height);
		}


		//ת��HSVɫ�ʿռ�
		/*
			hue Ϊһ�źڵ�ͼƬ����hsv����ͬ�ĳߴ��ɫ�����
			��hue��hsv��Hͨ�� ����ͨ�����
			����͹������
		*/

		cvtColor(frame,hsv,COLOR_BGR2HSV);
		inRange(hsv, Scalar(0, smin, vmin), Scalar(180, smax, vmax), mask);
		hue = Mat(hsv.size(),hsv.depth());
		int channels[] = {0,0};
		mixChannels(&hsv,1,&hue,1,channels,1);  //ͨ�����,������ͼƬ��0��ͨ��Ҳ����HSV�е�H���л��

		
		if (firstTimes)
		{
			//ROI ����ֱ��ͼ
			Mat roi(hue, selection);
			Mat maskroi(mask, selection);
				//����ͼ��ָ��  ͼ����Ŀ   ͨ����  roi����  �����ֱ��ͼ����  ά��  ֵ��Χ
			calcHist(&roi, 1, 0, maskroi, hist, 1, &bins, &hranges);
			normalize(hist, hist, 0, 255, NORM_MINMAX);

			//��ʾֱ��ͼ(����ȥ��)
			int binw = drawImg.cols / bins;  //ֱ��ͼ���
			Mat colorIndex = Mat(1, bins, CV_8UC3);
			for (int i = 0; i < bins; i++)
			{
				colorIndex.at<Vec3b>(0, i) = Vec3b(static_cast<uchar>(i * 180 / bins), 255, 255);
			}
			cvtColor(colorIndex, colorIndex, COLOR_HSV2BGR);
			for (int t = 0; t < bins; t++)
			{
				int val = static_cast<int>(hist.at<float>(t) * drawImg.rows / 255);
				rectangle(drawImg, Point(t * binw, drawImg.rows), Point((t + 1) * binw, drawImg.rows - val), Scalar(colorIndex.at<Vec3b>(0, t)), -1, 8, 0);
			}
		}
		

		//ֱ��ͼ����ӳ��
		calcBackProject(&hue,1,0,hist,backproject,&hranges);
		
		//CAMShiftλ�ø���
		backproject &= mask;
		RotatedRect trackBox = CamShift(backproject, selection, TermCriteria((TermCriteria::COUNT | TermCriteria::EPS), 10, 1));
		
		
		//����λ����ʾ
		ellipse(frame,trackBox,Scalar(0,0,255),3,8);



		if (firstTimes)
		{
			firstTimes = false;
		}

		imshow("Hist", drawImg);
		imshow("video",frame);

		char c = waitKey(50);
		if (c == 27)
		{
			break;
		}

	}

	capture.release();
	waitKey(0);
	return 0;

}