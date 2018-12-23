#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;

void Harris(Mat &img, Mat &out)
{
	Mat imgGray;
	cvtColor(img, imgGray, CV_BGR2GRAY);
	Mat imgx;
	Mat imgy;
	//Mat imgxy = Mat::zeros(img.size(), img.type());
	Sobel(imgGray, imgx, img.depth(), 1, 0);
	Sobel(imgGray, imgy, img.depth(), 0, 1);
	/*namedWindow("test");
	imshow("test", imgx);
	namedWindow("test2");
	imshow("test2", imgy);
	waitKey(0);*/
	/*int ** LX = new int*[img.rows];
	int ** LY = new int*[img.rows];
	int ** LXY = new int*[img.rows];
	for (int i = 0; i < img.rows; i++)
	{
		LX[i] = new int[img.cols];
		LY[i] = new int[img.cols];
		LXY[i] = new int[img.cols];
	}
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			LX[i][j] = imgx.at<uchar>(i, j)*imgx.at<uchar>(i, j);
			LY[i][j] = imgy.at<uchar>(i, j)*imgy.at<uchar>(i, j);
			LXY[i][j] = imgx.at<uchar>(i, j)*imgy.at<uchar>(i, j);
		}
	}*/
	Mat LX(img.rows, img.cols, CV_64F);
	Mat LY(img.rows, img.cols, CV_64F);
	Mat LXY(img.rows, img.cols, CV_64F);
	Mat R(img.rows, img.cols, CV_64F);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			/*LX.at<double>(i, j) = imgx.at<uchar>(i, j)*imgx.at<uchar>(i, j)/255.0/255;
			LY.at<double>(i, j) = imgy.at<uchar>(i, j)*imgy.at<uchar>(i, j)/255.0/255;
			LXY.at<double>(i, j) = imgx.at<uchar>(i, j)*imgy.at<uchar>(i, j)/255.0/255;*/
			LX.at<double>(i, j) = imgx.at<uchar>(i, j)*imgx.at<uchar>(i, j);
			LY.at<double>(i, j) = imgy.at<uchar>(i, j)*imgy.at<uchar>(i, j);
			LXY.at<double>(i, j) = imgx.at<uchar>(i, j)*imgy.at<uchar>(i, j);
			/*if(LX.at<double>(i, j)!=0)
				cout << LX.at<double>(i, j) << endl;*/
			/*if((int)imgx.at<uchar>(i, j)!=0)
				cout << (int)imgx.at<uchar>(i, j) << endl;*/
		}
	}
	GaussianBlur(LX, LX, Size(7, 7), 0, 0);
	GaussianBlur(LY, LY, Size(7, 7), 0, 0);
	GaussianBlur(LXY, LXY, Size(7, 7), 0, 0);

	double alpha = 0.05;
	double max = 0;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			R.at<double>(i, j) = LX.at<double>(i, j)*LY.at<double>(i, j) - LXY.at<double>(i, j)*LXY.at<double>(i, j) - alpha * (LX.at<double>(i, j) + LY.at<double>(i, j))*(LX.at<double>(i, j) + LY.at<double>(i, j));
			if (R.at<double>(i, j) > max)
			{
				max = R.at<double>(i, j);
			}
		}
	}
	out = img.clone();
	for (int i = 1; i < img.rows-1; i++)
	{
		for (int j = 1; j < img.cols-1; j++)
		{
			if (R.at<double>(i, j) > max/100 && R.at<double>(i, j)> R.at<double>(i-1, j-1) && R.at<double>(i, j)> R.at<double>(i-1, j) && R.at<double>(i, j)> R.at<double>(i-1, j+1) && R.at<double>(i, j)> R.at<double>(i, j-1)&& R.at<double>(i, j)> R.at<double>(i, j+1)&& R.at<double>(i, j)> R.at<double>(i+1, j-1)&& R.at<double>(i, j)> R.at<double>(i+1, j)&& R.at<double>(i, j)> R.at<double>(i+1, j+1))
			{
				circle(out, Point(j*(1<<4), i*(1<<4)), 15*(1<<4), Scalar(0, 255, 255),4,CV_AA,4);
			}
		}
	}

}

int main()
{
	Mat img = imread("geo.jpg");
	Mat out = Mat::zeros(img.size(), img.type());
	Mat Gray;
	Mat dst;
	Mat img2 = img.clone();

	cvtColor(img, Gray, CV_BGR2GRAY);

	Harris(img, out);
	cornerHarris(Gray, dst, 2, 3, 0.01);
	threshold(dst, dst, 0.000015, 255, THRESH_BINARY);
	cout << dst.type();

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (dst.at<float>(i, j) != 0)
			{
				circle(img2, Point(j*(1 << 4), i*(1 << 4)), 15 * (1 << 4), Scalar(0, 0, 255), 4, CV_AA, 4);
			}
		}
		cout << i << " / " << img.rows << endl;
	}

	imwrite("G1.jpg", out);
	imwrite("CVG1.jpg", img2);

	//namedWindow("harris");
	//imshow("harris", out);
	//namedWindow("opencv");
	//imshow("opencv", img2);
	//waitKey(0);

	//system("pause");
	return 0;
}