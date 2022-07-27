#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){

	VideoCapture capture;
	capture.open("D:/OpenCV/picture zone/video_003.avi"); //video_003.avi
	if (!capture.isOpened())
	{
		printf("ERROR");
		return -1;
	}

	//初始化BS模型
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();

	Mat k = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));

	vector<vector<Point>> contours;
	vector<Vec4i>hireachy;
	int count = 0; //计数

	Mat frame,gray,mogmask;
	namedWindow("video",CV_WINDOW_AUTOSIZE);
	while (capture.read(frame))
	{
		imshow("video",frame);

		pMOG2->apply(frame,mogmask); //mogmask出来的是一个8位灰度图像
		//二值化
		threshold(mogmask,mogmask,100,255,THRESH_BINARY);
		morphologyEx(mogmask,mogmask,MORPH_OPEN,k);
		
		//取轮廓
		findContours(mogmask,contours,hireachy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point(0,0));

		count = 0;
		char numtext[8];
		for (size_t i = 0; i < contours.size(); i++)
		{
			double area = contourArea(contours[i]);
			if (area<1000) 
			{
				continue;
			}

			Rect selection = boundingRect(contours[i]);
			if (selection.width < 30|| selection.height < 30)
			{
				continue;
			}
			
			count++;
			rectangle(frame,selection,Scalar(0,0,255),2,8,0);

			sprintf_s(numtext,"%d",count);
			putText(frame, numtext, Point(selection.x,selection.y),CV_FONT_NORMAL,FONT_HERSHEY_PLAIN,Scalar(255,0,0),1,8);
		}


		imshow("motion object", frame);

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
