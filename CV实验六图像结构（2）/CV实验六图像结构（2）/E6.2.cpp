#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;

int main() 
{
	Mat img = imread("snow.jpg");
	Mat out = Mat::zeros(img.size(), img.type());
	Mat gray;
	cvtColor(img, gray, CV_RGB2GRAY);
	threshold(gray, out, 127, 255, CV_THRESH_BINARY);
	Mat output = Mat::zeros(img.size(), img.type());
	distanceTransform(out, output, CV_DIST_L2, 3);
	normalize(output, output, 0, 1., NORM_MINMAX);
	namedWindow("snow");
	imshow("snow", img);
	namedWindow("out");
	imshow("out", output);
	waitKey();
	return 0;
}