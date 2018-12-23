#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("a1.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat alpha(img.rows, img.cols, CV_8UC1);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			alpha.at<uchar>(i, j) = img.at<Vec4b>(i, j)[3];
		}
	}
	namedWindow("alpha");
	imshow("alpha", alpha);
	namedWindow("a1");
	imshow("a1", img);
	waitKey(0);
}
