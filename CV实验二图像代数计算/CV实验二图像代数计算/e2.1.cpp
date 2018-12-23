#include<iostream>
#include<stdio.h>
#include<opencv2/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<math.h>
using namespace std;
using namespace cv;

Mat originImg;
Mat changedImg;
double threshold = 127.5;


const int maxValue = 10;
int currentValue;

void recall(int, void*);

void Sigmoid(int value)
{
	double _value = value - 5;
	for (int i = 0; i < originImg.rows; i++)
	{
		for (int j = 0; j < originImg.cols; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				//changedImg.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(originImg.at<Vec3b>(i, j)[c]*2 / (1 + exp(-(_value/30.0))));
				if (_value > 0) 
				{
					changedImg.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(originImg.at<Vec3b>(i, j)[c] * 2 / (1 + exp(_value * (originImg.at<Vec3b>(i, j)[c] - threshold)))));
				}
				else
				{
					changedImg.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(originImg.at<Vec3b>(i, j)[c] * 2 / (1 + exp(pow(10,-_value)/100000.0 * (originImg.at<Vec3b>(i, j)[c] - threshold))));
				}
			}
		}
	}
	imshow("changed picture", changedImg);
}

int main()
{
	currentValue = 5;
	originImg = imread("timg.jpg");
	namedWindow("original");
	imshow("original", originImg);
	changedImg = Mat::zeros(originImg.size(), originImg.type());
	namedWindow("change", WINDOW_AUTOSIZE);
	createTrackbar("对比度调整", "change", &currentValue, maxValue, recall);
	recall(currentValue, 0);
	waitKey(0);
	return 0;

}

void recall(int, void*)
{
	Sigmoid(currentValue);
	
	imshow("changed picture", changedImg);
}


