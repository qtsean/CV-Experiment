#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{

	Mat img = imread("a1.png", -1);
	Mat backGroundImg = imread("e.png", -1);
	Mat pic(backGroundImg.rows, backGroundImg.cols, CV_8UC4);
	for (int i = 0; i < pic.rows; i++)
	{
		for (int j = 0; j < pic.cols; j++)
		{
			double temp;
			if (i >= img.rows||j >= img.cols)
			{
				pic.at<Vec4b>(i, j)[0] = backGroundImg.at<Vec4b>(i, j)[0];
				pic.at<Vec4b>(i, j)[1] = backGroundImg.at<Vec4b>(i, j)[1];
				pic.at<Vec4b>(i, j)[2] = backGroundImg.at<Vec4b>(i, j)[2];
				pic.at<Vec4b>(i, j)[3] = backGroundImg.at<Vec4b>(i, j)[3];
			}
			else
			{
				temp = img.at<Vec4b>(i, j)[3] / 255.00;
				pic.at<Vec4b>(i, j)[0] = (1 - temp)*backGroundImg.at<Vec4b>(i, j)[0] + temp * img.at<Vec4b>(i, j)[0];
				pic.at<Vec4b>(i, j)[1] = (1 - temp)*backGroundImg.at<Vec4b>(i, j)[1] + temp * img.at<Vec4b>(i, j)[1];
				pic.at<Vec4b>(i, j)[2] = (1 - temp)*backGroundImg.at<Vec4b>(i, j)[2] + temp * img.at<Vec4b>(i, j)[2];
				pic.at<Vec4b>(i, j)[3] = (1 - temp)*backGroundImg.at<Vec4b>(i, j)[3] + temp * img.at<Vec4b>(i, j)[3];
			}
			/*Vec3b pixel = backGroundImg.at<Vec3b>(i, j);
			if (i < img.rows&&j < img.cols)
			{
				Vec3b aPixel = img.at<Vec3b>(i, j);
				if (img.at<uchar>(i, j) == 255)
				{
					pixel = aPixel;
				}
				else
				{
					double temp = img.at<uchar>(i, j) / 255.00;
					pixel[0] = (1 - temp)*pixel[0] + temp * aPixel[0];
					pixel[1] = (1 - temp)*pixel[1] + temp * aPixel[1];
					pixel[2] = (1 - temp)*pixel[2] + temp * aPixel[2];
				}
			}
			pic.at<Vec3b>(i, j) = pixel;*/
		}
	}
	namedWindow("合成");
	imshow("合成", pic);
	waitKey(0);
	return 0;
}