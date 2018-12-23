#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
int main()
{
	Mat img = imread("cat.jpg");
	namedWindow("test");
	imshow("test", img);
	waitKey(6000);
	return 0;

}