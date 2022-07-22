#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


Mat frame, gray;   //��ǰ֡
Mat prev_frame, prev_gray;   //ǰһ֡
Mat flowResult, flowData;//�洢�����Ľ��������

void drawOpticalFlowHF(Mat& flowData, Mat& image, int step);//��������  ���ͼƬ  ����


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
	
	//��ȡ��һ֡������
	capture.read(prev_frame);
	cvtColor(prev_frame,prev_gray,COLOR_BGR2GRAY);

	//�ӵڶ�֡��ʼ
	while (capture.read(frame))
	{
		cvtColor(frame,gray,COLOR_BGR2GRAY);
		if (!prev_gray.empty())
		{
			calcOpticalFlowFarneback(prev_gray, gray, flowData, 0.5, 3, 15, 3, 5, 1.2, 0);
			prev_frame.copyTo(flowResult);  //����һ֡ȡ��
			drawOpticalFlowHF(flowData,flowResult,1);  //���Ƴ��ܹ���

			
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
			//����ж���x����y�����ƶ�2���������ϣ����ж�Ϊ�ƶ�
			if (fxy.x > 2 ||fxy.y > 2)
			{
				line(image,Point(col,row),Point(cvRound(col+fxy.x),cvRound(row+fxy.y)) ,Scalar(0,255,0),2,8,0);
				circle(image, Point(col,row),2,Scalar(0,0,255),-1);

			}
		}

	}


}
