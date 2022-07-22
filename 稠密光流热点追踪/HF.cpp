#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


Mat frame, gray;   //当前帧
Mat prev_frame, prev_gray;   //前一帧
Mat flowResult, flowData;//存储光流的结果和数据

void drawOpticalFlowHF(Mat& flowData, Mat& image, int step);//光流数据  输出图片  步长


int main() {
	VideoCapture capture;
	capture.open("D:/OpenCV/picture zone/video_003.avi");
	if (!capture.isOpened())
	{
		printf("ERROR");
		return -1;
	}
	namedWindow("input video", CV_WINDOW_AUTOSIZE);
	namedWindow("flow", CV_WINDOW_AUTOSIZE);
	
	//读取第一帧的数据
	capture.read(prev_frame);
	cvtColor(prev_frame,prev_gray,COLOR_BGR2GRAY);

	//从第二帧开始
	while (capture.read(frame))
	{
		cvtColor(frame,gray,COLOR_BGR2GRAY);
		if (!prev_gray.empty())
		{
			calcOpticalFlowFarneback(prev_gray, gray, flowData, 0.5, 3, 15, 3, 5, 1.2, 0);
			prev_frame.copyTo(flowResult);  //将第一帧取出
			drawOpticalFlowHF(flowData,flowResult,1);  //绘制稠密光流

			
			imshow("flow", flowResult);
			imshow("input video", frame);
		}

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

void drawOpticalFlowHF(Mat& flowData, Mat& image, int step)
{
	for (int row = 0; row < image.rows; row++)
	{
		for (int col = 0; col < image.cols; col++)
		{
			Point2f fxy = flowData.at<Point2f>(row,col); 
			//如果判断在x或者y方向移动2个像素以上，则判断为移动
			if (fxy.x > 2 ||fxy.y > 2)
			{
				line(image,Point(col,row),Point(cvRound(col+fxy.x),cvRound(row+fxy.y)) ,Scalar(0,255,0),2,8,0);
				circle(image, Point(col,row),2,Scalar(0,0,255),-1);

			}
		}

	}


}
