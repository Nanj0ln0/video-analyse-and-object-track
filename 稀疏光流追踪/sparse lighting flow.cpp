#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


/*
*	取得第一帧的特征点，保存起来，然后对后续的帧去计算位移
* 
	第一步 取得第一帧，做shi-tomasi角点检测，将特征点保存起来
	 
*/
vector<Point2f>Features;   //shi-tomasi  特征数据
void detectFeatures(Mat &inFrame , Mat &ingray);  //特征检测
void klTrackFeture();  //跟踪
void drawTrackLine(); //绘制跟踪线

vector<Point2f> inipoints;   //初始化特征数据，后面光流法要用的  （可以不做）
vector<Point2f> fpts[2];	//保存当前帧和前一帧的特征点位置   fpts[0] 前一帧的特征  fpts[1] 当前的特征

//光流追踪算法输出变量
vector<uchar>status;//特征点跟踪成功标志位
vector<float>errors;//跟踪时候区域误差和 

Mat frame, gray;   //当前帧
Mat prev_frame, prev_gray;   //前一帧


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
		//第一次叫 采样  后续叫 重采样
		//如果特征点少于一个阈值，则需要重新进行检测
		//因为在跟踪的过程中会因为光照等情况导致特征点丢失
		if (fpts[0].size()<40)    
		{
			detectFeatures(frame, gray);
			//在fpts[0]数组末尾中插入，Features(特征点)从头到尾
			fpts[0].insert(fpts[0].end(),Features.begin(),Features.end());
			inipoints.insert(inipoints.end(), Features.begin(), Features.end());
		}
		else
		{
			printf("tttttttttttttttt");
		}

		//如果前一帧为空  则把当前帧拷贝进来作为前一帧
		if (prev_gray.empty())  
		{
			gray.copyTo(prev_gray);
		}

		klTrackFeture();   //跟踪
		

		//保存当前帧为前一帧（更新前一帧数据）
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

//特征检测 shi-tomasi
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
	//光流法跟踪
	calcOpticalFlowPyrLK(prev_gray,gray,fpts[0],fpts[1],status,errors);  

	//特征点过滤
	int k = 0;
	for (int i = 0; i < fpts[1].size(); i++)
	{
		//删除损失的关键点，保存新的关键点		
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y - fpts[1][i].y);
		if (dist > 2 && status[i])
		{
			inipoints[k] = inipoints[i];
			fpts[1][k++] = fpts[1][i];
		}
	}

	//保存特征点并且绘制轨迹
	//重新规定大小
	inipoints.resize(k);
	fpts[1].resize(k);

	drawTrackLine();

	//交换现在和之前的值
	std::swap(fpts[1],fpts[0]);

}

//画线
void drawTrackLine() {

	for (size_t t = 0; t < fpts[1].size(); t++)
	{
		line(frame,inipoints[t],fpts[1][t],	Scalar(0,255,0),2,8,0);
		circle(frame, fpts[1][t], 2, Scalar(0, 0, 255), 2, 8, 0);
	}


}