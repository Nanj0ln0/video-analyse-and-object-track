#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Rect ROI;
void processFrame(Mat &binary,Rect &rect);


int main() {

	VideoCapture capture;
	//一个绿色的飞盘
	capture.open("D:/OpenCV/picture zone/video_006.mp4");   
	if (!capture.isOpened())
	{
		printf("ERROR");
		return -1;
	}
	Mat frame, mask;

	capture.read(frame);
	
	Mat background = Mat::zeros(frame.size(),frame.type());

	Mat k1 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1)); //形态学定义
	Mat k2 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1)); //形态学定义
	namedWindow("input video",CV_WINDOW_AUTOSIZE);
	namedWindow("track mask",CV_WINDOW_AUTOSIZE);
	while (capture.read(frame))
	{
		inRange(frame,Scalar(0,127,0),Scalar(120,255,120),mask);  //颜色过滤	
	
		morphologyEx(mask,mask,MORPH_OPEN,k1,Point(-1,-1));   //开操作，去除椒盐噪声
		dilate(mask,mask,k2,Point(-1,-1),4);   //膨胀扩大目标形状

		processFrame(mask,ROI);  //使用 轮廓获取  ，获取感兴趣区域

		rectangle(frame,ROI,Scalar(0,0,255),2,8,0);
		circle(background, Point(ROI.x,ROI.y),3,Scalar(0,0,255),-1,8,0);
		imshow("轨迹", background);
		imshow("track mask", frame);



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

void processFrame(Mat& binary, Rect& rect)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;

	//查找轮廓
	findContours(binary,contours,hireachy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point(0,0));
	if (contours.size()>0)  //判断轮廓是否存在
	{
		double maxArea = 0.0;  //最大面积
		for (size_t t = 0; t < contours.size(); t++)  //轮廓计数
		{
			//寻找最大外接矩形
			double area = contourArea(contours[static_cast<int>(t)]);
			if (area >maxArea) //如果当前轮廓面积 大于 之前的面积
			{
				maxArea = area;  //存下
				rect = boundingRect(contours[static_cast<int>(t)]);    //寻找最大外接矩形
			}
			else//如果当前轮廓面积 小于 之前的面积
			{
				//全部返回 0
				rect.x = rect.y = rect.height = rect.width = 0;
			}
		}
	

	}

}
