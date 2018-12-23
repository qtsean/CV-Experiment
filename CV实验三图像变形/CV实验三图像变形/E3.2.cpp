#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2/opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;


void bilinear(Mat &src, Mat &out, int i, int j, double x, double y)
{
	if (x < 0 || y < 0 || x >= src.rows - 1 || y >= src.cols - 1)
	{
		return;
	}
	else
	{
		for (int c = 0; c < 3; c++)
		{
			double u = x - int(x);
			double v = y - int(y);
			out.at<Vec3b>(i, j)[c] = (u * src.at<Vec3b>(int(x) + 1, int(y))[c] + (1 - u)*src.at<Vec3b>(int(x), int(y))[c])*(1 - v) + (u*src.at<Vec3b>(int(x) + 1, int(y) + 1)[c] + (1 - u)*src.at<Vec3b>(int(x), int(y) + 1)[c])*v;
		}
	}

}


int main()
{
	Mat img = imread("origin.jpg");
	Mat out = Mat::zeros( img.rows,  img.cols, img.type());
	double x = 0;
	double y = 0;
	for (int i = 0; i < out.rows; i++)
	{
		for (int j = 0; j < out.cols; j++)
		{
			x = (i - 0.5*out.rows) / (0.5*out.rows);
			y = (j - 0.5*out.cols) / (0.5*out.cols);
			if ((x * x + y * y) >= 1)
			{
				out.at<Vec3b>(i, j)[0] = img.at<Vec3b>(i, j)[0];
				out.at<Vec3b>(i, j)[1] = img.at<Vec3b>(i, j)[1];
				out.at<Vec3b>(i, j)[2] = img.at<Vec3b>(i, j)[2];
			}
			else
			{
				double r = sqrt(x * x + y * y);
				double theta = (1 - r) * (1 - r);
				double x1 = cos(theta)*x - sin(theta)*y;
				double y1 = sin(theta)*x + cos(theta)*y;
				x = x1 * 0.5*out.rows + 0.5*out.rows;
				y = y1 * 0.5*out.cols + 0.5*out.cols;
				bilinear(img, out, i, j, x, y);
			}
		}
	}
	namedWindow("origin");
	imshow("origin", img);
	imshow("changed", out);
	waitKey(0);
	return 0;
}