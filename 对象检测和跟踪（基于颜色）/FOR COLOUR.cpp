#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Rect ROI;
void processFrame(Mat &binary,Rect &rect);


int main() {

	VideoCapture capture;
	//һ����ɫ�ķ���
	capture.open("D:/OpenCV/picture zone/video_006.mp4");   
	if (!capture.isOpened())
	{
		printf("ERROR");
		return -1;
	}
	Mat frame, mask;

	capture.read(frame);
	
	Mat background = Mat::zeros(frame.size(),frame.type());

	Mat k1 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1)); //��̬ѧ����
	Mat k2 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1)); //��̬ѧ����
	namedWindow("input video",CV_WINDOW_AUTOSIZE);
	namedWindow("track mask",CV_WINDOW_AUTOSIZE);
	while (capture.read(frame))
	{
		inRange(frame,Scalar(0,127,0),Scalar(120,255,120),mask);  //��ɫ����	
	
		morphologyEx(mask,mask,MORPH_OPEN,k1,Point(-1,-1));   //��������ȥ����������
		dilate(mask,mask,k2,Point(-1,-1),4);   //��������Ŀ����״

		processFrame(mask,ROI);  //ʹ�� ������ȡ  ����ȡ����Ȥ����

		rectangle(frame,ROI,Scalar(0,0,255),2,8,0);
		circle(background, Point(ROI.x,ROI.y),3,Scalar(0,0,255),-1,8,0);
		imshow("�켣", background);
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

	//��������
	findContours(binary,contours,hireachy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point(0,0));
	if (contours.size()>0)  //�ж������Ƿ����
	{
		double maxArea = 0.0;  //������
		for (size_t t = 0; t < contours.size(); t++)  //��������
		{
			//Ѱ�������Ӿ���
			double area = contourArea(contours[static_cast<int>(t)]);
			if (area >maxArea) //�����ǰ������� ���� ֮ǰ�����
			{
				maxArea = area;  //����
				rect = boundingRect(contours[static_cast<int>(t)]);    //Ѱ�������Ӿ���
			}
			else//�����ǰ������� С�� ֮ǰ�����
			{
				//ȫ������ 0
				rect.x = rect.y = rect.height = rect.width = 0;
			}
		}
	

	}

}
