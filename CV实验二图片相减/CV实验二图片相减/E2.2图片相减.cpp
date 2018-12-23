#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;


const int distance = 6000;

void Mask(Mat &org, Mat &back)
{
	int count = 0;
	int _distance = 0;
	Mat mask(org.rows, org.cols, CV_8UC1);
	int rows = org.rows;
	int cols = org.cols*org.channels();
	for (int i = 0; i < rows ; i++)
	{
		uchar* dataOrg = org.ptr<uchar>(i);
		uchar* dataBack = back.ptr<uchar>(i);
		uchar* dataMask = mask.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			count++;
			if (count != 4) 
			{
				_distance += (dataOrg[j] - dataBack[j])*(dataOrg[j] - dataBack[j]);
			}
			if (count == 4)
			{
				if ((_distance) > ::distance)
				{
					dataMask[j / 4] = 255;
				}
				_distance = 0;
				count = 0;
			}
		}
	}
	namedWindow("output");
	imshow("output", mask);
	imshow("origin", org);
	imshow("background", back);
}

int main()
{
	Mat origin=imread("I.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat background = imread("B.png",CV_LOAD_IMAGE_UNCHANGED);
	Mat mask(origin.rows, origin.cols, CV_8UC1);
	Mat Box, Blur, gaussian, median, bilateral;
	//Mat mask = Mat::zeros(origin.size(), origin.type());
	namedWindow("origin");
	namedWindow("background");
	imshow("origin", origin);
	imshow("background", background);
	for (int i = 0; i < origin.rows; i++)
	{
		for (int j = 0; j < origin.cols; j++)
		{
			int _distance = 0;
			for (int c = 0; c < 3; c++)
			{
				//mask.at<Vec3b>(i, j)[c] = abs(origin.at<Vec3b>(i, j)[c] - background.at<Vec3b>(i, j)[c]);
				_distance += (origin.at<Vec4b>(i, j)[c] - background.at<Vec4b>(i, j)[c])*(origin.at<Vec4b>(i, j)[c] - background.at<Vec4b>(i, j)[c]);
			}
			if (_distance > ::distance)
			{
				mask.at<uchar>(i, j) = origin.at<Vec4b>(i, j)[3];
				//origin.at<Vec4b>(i, j)[3] = 255;
			}
			else
			{
				mask.at<uchar>(i, j) = 0;
				//origin.at<Vec4b>(i, j)[3] = 0;
			}
			//mask.at<uchar>(i, j) = origin.at<Vec4b>(i, j)[3];
		}
	}
	//cvtColor(mask, mask, CV_BGR2GRAY);
	//threshold(mask, mask, 32, 255, THRESH_BINARY);
//	Mat mask(origin.rows, origin.cols, -1);
	/*for (int i = 0; i < origin.rows; i++)
	{
		for (int j = 0; j < origin.cols; j++)
		{
			mask.at<uchar>(i, j) = origin.at<Vec4b>(i, j)[3];
		}
	}*/
	namedWindow("output");
	imshow("output", mask);
	Box = mask.clone();
	Blur = mask.clone();
	gaussian = mask.clone();
	median = mask.clone();
	bilateral = mask.clone();
	boxFilter(mask, Box, -1, Size(3, 3));
	imshow("BoxFilter", Box);
	blur(mask, Blur, Size(3, 3));
	imshow("Blur", Blur);
	GaussianBlur(mask, gaussian, Size(3, 3),0,0);
	imshow("GuassianBlur", gaussian);
	medianBlur(mask, median, 3);
	imshow("MedianBlur", median);
	bilateralFilter(mask, bilateral, 4, 8, 2);
	imshow("bilateral", bilateral);

//	Mask(origin, background);
	waitKey(0);
	//imwrite("mask.png", mask);
	return 0;


}