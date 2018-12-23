#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2/opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;
void bilinear(Mat &src, Mat &out, int i, int j, double x, double y);
void inverse(double a[3][3], double b[3][3]);
void WarpAffine(Mat &src, Mat &out, double originarray[3][3])
{
	double inverseArray[3][3] = { 0,0,0,0,0,0,0,0,0 };
	inverse(originarray, inverseArray);
	for (int i = 0; i < out.rows; i++)
	{
		for (int j = 0; j < out.cols; j++)
		{
			double x = inverseArray[0][0] * i + inverseArray[0][1] * j + inverseArray[0][2] * 1;
			double y = inverseArray[1][0] * i + inverseArray[1][1] * j + inverseArray[1][2] * 1;
			bilinear(src, out,i,j, x, y);
		}
	}
}

void bilinear(Mat &src, Mat &out,int i ,int j,double x, double y)
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

void inverse(double a[3][3], double b[3][3])
{
	double det = a[0][0] * (a[1][1] * a[2][2] - a[2][1] * a[1][2]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
	double invdet = 1 / det;
	b[0][0] = (a[1][1] * a[2][2] - a[2][1] * a[1][2]) * invdet;
	b[0][1] = (a[0][2] * a[2][1] - a[0][1] * a[2][2]) * invdet;
	b[0][2] = (a[0][1] * a[1][2] - a[0][2] * a[1][1]) * invdet;
	b[1][0] = (a[1][2] * a[2][0] - a[1][0] * a[2][2]) * invdet;
	b[1][1] = (a[0][0] * a[2][2] - a[0][2] * a[2][0]) * invdet;
	b[1][2] = (a[1][0] * a[0][2] - a[0][0] * a[1][2]) * invdet;
	b[2][0] = (a[1][0] * a[2][1] - a[2][0] * a[1][1]) * invdet;
	b[2][1] = (a[2][0] * a[0][1] - a[0][0] * a[2][1]) * invdet;
	b[2][2] = (a[0][0] * a[1][1] - a[1][0] * a[0][1]) * invdet;
}

double matrix33(double a[3][3], double b[3][3], int i, int j)
{
	double temp = 0;
	for (int k = 0; k < 3; k++)
	{
		temp += a[i][k] * b[k][j];
	}
	return temp;
}

void Rotate(Mat &src, Mat &out, double a[3][3], int x, int y)
{
	double m[3][3] = { 1,0,x,0,1,y,0,0,1 };
	double n[3][3] = { 1,0,-x,0,1,-y,0,0,1 };

	double c[3][3] = { 0,0,0,0,0,0,0,0,0 };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			c[i][j] = matrix33(m, a, i, j);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] = matrix33(c, n, i, j);
		}
	}
	WarpAffine(src, out, m);
}

int main()
{
	int cx = 230;
	int cy = 170;
	double theta = -30;
	theta = theta * 3.1415926 / 180;
	Mat img = imread("origin.jpg");
	Mat out = Mat::zeros(2*img.rows, 2*img.cols, img.type());
	//旋转
	double mat[3][3] = 
	{
		cos(theta),-sin(theta),0,
		sin(theta),cos(theta),0,
		0,0,1
	};
	//平移
	/*double mat[3][3] = {
		1,0,100,
		0,1,100,
		0,0,1
	};*/
	//缩放
	/*double mat[3][3] = {
		1.4,0,0,
		0,1.4,0,
		0,0,1
	};*/
	//切变
	/*double mat[3][3] = {
		1,0.3,0,
		0,1,0,
		0,0,1
	};*/
	Rotate(img, out,mat, cx, cy);
	//WarpAffine(img, out, mat);
	namedWindow("origin");
	imshow("origin", img);
	imshow("transform", out);
	waitKey(0);
	return 0;
}