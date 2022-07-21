#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


/*
*	ȡ�õ�һ֡�������㣬����������Ȼ��Ժ�����֡ȥ����λ��
* 
	��һ�� ȡ�õ�һ֡����shi-tomasi�ǵ��⣬�������㱣������
	 
*/
vector<Point2f>Features;   //shi-tomasi  ��������
void detectFeatures(Mat &inFrame , Mat &ingray);  //�������
void klTrackFeture();  //����
void drawTrackLine(); //���Ƹ�����

vector<Point2f> inipoints;   //��ʼ���������ݣ����������Ҫ�õ�  �����Բ�����
vector<Point2f> fpts[2];	//���浱ǰ֡��ǰһ֡��������λ��   fpts[0] ǰһ֡������  fpts[1] ��ǰ������

//����׷���㷨�������
vector<uchar>status;//��������ٳɹ���־λ
vector<float>errors;//����ʱ���������� 

Mat frame, gray;   //��ǰ֡
Mat prev_frame, prev_gray;   //ǰһ֡


int main() {
	VideoCapture capture(0);
	//VideoCapture capture;
	//capture.open("D:/OpenCV/picture zone/bike.avi");
	
	if (!capture.isOpened())
	{
		printf("ERROR");
		return -1;
	}

	namedWindow("input video",CV_WINDOW_AUTOSIZE);
	while (capture.read(frame))
	{
		cvtColor(frame,gray,COLOR_BGR2GRAY);
		//��һ�ν� ����  ������ �ز���
		//�������������һ����ֵ������Ҫ���½��м��
		//��Ϊ�ڸ��ٵĹ����л���Ϊ���յ�������������㶪ʧ
		if (fpts[0].size()<40)    
		{
			detectFeatures(frame, gray);
			//��fpts[0]����ĩβ�в��룬Features(������)��ͷ��β
			fpts[0].insert(fpts[0].end(),Features.begin(),Features.end());
			inipoints.insert(inipoints.end(), Features.begin(), Features.end());
		}
		else
		{
			printf("tttttttttttttttt");
		}

		//���ǰһ֡Ϊ��  ��ѵ�ǰ֡����������Ϊǰһ֡
		if (prev_gray.empty())  
		{
			gray.copyTo(prev_gray);
		}

		klTrackFeture();   //����
		

		//���浱ǰ֡Ϊǰһ֡������ǰһ֡���ݣ�
		gray.copyTo(prev_gray);
		frame.copyTo(prev_frame);


		imshow("input video",frame);
		char c = waitKey(100);
		if (c == 27)
		{
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;

}

//������� shi-tomasi
void detectFeatures(Mat& inFrame, Mat& ingray)  
{
	double maxCorners = 5000;
	double quilitylevel = 0.01;
	double minDistance = 10;
	double blockSize = 3;
	goodFeaturesToTrack(ingray,Features,maxCorners,quilitylevel,minDistance,Mat(),blockSize,false,0.04);


}


void klTrackFeture()
{
	//����������
	calcOpticalFlowPyrLK(prev_gray,gray,fpts[0],fpts[1],status,errors);  

	//���������
	int k = 0;
	for (int i = 0; i < fpts[1].size(); i++)
	{
		//ɾ����ʧ�Ĺؼ��㣬�����µĹؼ���		
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y - fpts[1][i].y);
		if (dist > 2 && status[i])
		{
			inipoints[k] = inipoints[i];
			fpts[1][k++] = fpts[1][i];
		}
	}

	//���������㲢�һ��ƹ켣
	//���¹涨��С
	inipoints.resize(k);
	fpts[1].resize(k);

	drawTrackLine();

	//�������ں�֮ǰ��ֵ
	std::swap(fpts[1],fpts[0]);

}

//����
void drawTrackLine() {

	for (size_t t = 0; t < fpts[1].size(); t++)
	{
		line(frame,inipoints[t],fpts[1][t],	Scalar(0,255,0),2,8,0);
		circle(frame, fpts[1][t], 2, Scalar(0, 0, 255), 2, 8, 0);
	}


}