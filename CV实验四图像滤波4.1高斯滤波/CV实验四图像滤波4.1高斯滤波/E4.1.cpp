#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;

void Gaussian(Mat org, Mat out, double sigma);
Mat changeWindowSize(Mat org, int windowSize);

int main()
{
	Mat org = imread("test.jpg");
	Mat out = Mat::zeros(org.rows, org.cols, org.type());
	double sigma = 5;
	namedWindow("origin");
	imshow("origin", org);
	Gaussian(org, out, sigma);
	imshow("changed", out);
	waitKey(0);
	return 0;
}

void Gaussian(Mat org, Mat out, double sigma)
{
	int windowSize = 6 * sigma - 1;
	if (windowSize % 2 == 0)
	{
		windowSize += 1;
	}
	int addLength = 0.5*windowSize;
	Mat changedOrg = changeWindowSize(org, windowSize);
//	imshow("changedwindow", changedOrg);
	double sum = 0;
	double *weight = new double[windowSize];
	for (int i = 0; i < windowSize; i++)
	{
		weight[i] = exp((i - windowSize / 2)*(i - windowSize / 2) / (-2 * sigma*sigma));
		sum += weight[i];
	}
	for (int i = 0; i < windowSize; i++)
	{
		weight[i] /= sum;
	}
	for (int i = 0; i < org.rows ; i++)
	{
		for (int j = 0; j < org.cols ; j++)
		{
			double sumb = 0;
			double sumg = 0;
			double sumr = 0;
			for (int k = 0; k < windowSize; k++)
			{
				sumb += weight[k] * changedOrg.at<Vec3b>(i + addLength, j + k + addLength - windowSize / 2)[0];
				sumg += weight[k] * changedOrg.at<Vec3b>(i + addLength, j + k + addLength - windowSize / 2)[1];
				sumr += weight[k] * changedOrg.at<Vec3b>(i + addLength, j + k + addLength - windowSize / 2)[2];
			}
			changedOrg.at<Vec3b>(i, j)[0] = sumb;
			changedOrg.at<Vec3b>(i, j)[1] = sumg;
			changedOrg.at<Vec3b>(i, j)[2] = sumr;
		}
	}
	for (int i = 0; i < org.rows; i++)
	{
		for (int j = 0; j < org.cols; j++)
		{
			double sumb = 0;
			double sumg = 0;
			double sumr = 0;
			for (int k = 0; k < windowSize; k++)
			{
				sumb += weight[k] * changedOrg.at<Vec3b>(i + addLength + k - windowSize / 2, j + addLength)[0];
				sumg += weight[k] * changedOrg.at<Vec3b>(i + addLength + k - windowSize / 2, j + addLength)[1];
				sumr += weight[k] * changedOrg.at<Vec3b>(i + addLength + k - windowSize / 2, j + addLength)[2];
			}
			out.at<Vec3b>(i, j)[0] = sumb;
			out.at<Vec3b>(i, j)[1] = sumg;
			out.at<Vec3b>(i, j)[2] = sumr;
		}
	}
}

Mat changeWindowSize(Mat org, int windowSize)
{
	int addLength = 0.5*windowSize;
	Mat newOrg = Mat::zeros(org.rows + 2 * addLength, org.cols + 2 * addLength, org.type());
	for (int i = 0; i < newOrg.rows; i++)
	{
		for (int j = 0; j < newOrg.cols; j++)
		{
			//cout << i << "\t" << j << endl;
			/*if (i >= addLength && i < org.rows + addLength && j >= addLength && j < org.cols + addLength)
			{
				newOrg.at<Vec3b>(i, j)[0] = org.at<Vec3b>(i - addLength, j - addLength)[0];
				newOrg.at<Vec3b>(i, j)[1] = org.at<Vec3b>(i - addLength, j - addLength)[1];
				newOrg.at<Vec3b>(i, j)[2] = org.at<Vec3b>(i - addLength, j - addLength)[2];
			}
			else if (i < addLength && j >= addLength && j < org.cols + addLength)
			{
				int length = addLength - i;
				newOrg.at<Vec3b>(i, j)[0] = org.at<Vec3b>(length, j-addLength)[0];
				newOrg.at<Vec3b>(i, j)[1] = org.at<Vec3b>(length, j-addLength)[1];
				newOrg.at<Vec3b>(i, j)[2] = org.at<Vec3b>(length, j-addLength)[2];
			}
			else if (i >= org.rows + addLength && j >= addLength && j < org.cols + addLength)
			{
				int length = i - addLength + 1;
				newOrg.at<Vec3b>(i, j)[0] = org.at<Vec3b>(org.rows  - length - 1, j-addLength)[0];
				newOrg.at<Vec3b>(i, j)[1] = org.at<Vec3b>(org.rows  - length - 1, j-addLength)[1];
				newOrg.at<Vec3b>(i, j)[2] = org.at<Vec3b>(org.rows  - length - 1, j-addLength)[2];
			}
			else if (i >= addLength && i < org.rows + addLength && j < addLength)
			{
				int length = addLength - j;
				newOrg.at<Vec3b>(i, j)[0] = org.at<Vec3b>(i - addLength, length)[0];
				newOrg.at<Vec3b>(i, j)[1] = org.at<Vec3b>(i - addLength, length)[1];
				newOrg.at<Vec3b>(i, j)[2] = org.at<Vec3b>(i - addLength, length)[2];
			}
			else if (i >= addLength && i < org.rows + addLength && j >= org.cols + addLength)
			{
				int length = j - addLength + 1;
				newOrg.at<Vec3b>(i, j)[0] = org.at<Vec3b>(i - addLength, org.cols - length - 1)[0];
				newOrg.at<Vec3b>(i, j)[1] = org.at<Vec3b>(i - addLength, org.cols - length - 1)[1];
				newOrg.at<Vec3b>(i, j)[2] = org.at<Vec3b>(i - addLength, org.cols - length - 1)[2];
			}
			else if()*/
			int x;
			int y;
			if (i < addLength) 
			{
				int length = addLength - i;
				x = length;
			}
			if (i >= org.rows + addLength) 
			{
				int length = i - addLength - org.rows + 1;
				x = org.rows - length - 1;
			}
			if (i >= addLength && i < org.rows + addLength)
			{
				x = i - addLength;
			}
			if (j < addLength)
			{
				int length = addLength - j;
				y = length;
			}
			if (j >= org.cols + addLength)
			{
				int length = j - addLength - org.cols + 1;
				y = org.cols - length - 1;
			}
			if (j >= addLength && j < org.cols + addLength)
			{
				y = j - addLength;
			}
			newOrg.at<Vec3b>(i, j)[0] = org.at<Vec3b>(x, y)[0];
			newOrg.at<Vec3b>(i, j)[1] = org.at<Vec3b>(x, y)[1];
			newOrg.at<Vec3b>(i, j)[2] = org.at<Vec3b>(x, y)[2];
		}
	}
	return newOrg;
}
