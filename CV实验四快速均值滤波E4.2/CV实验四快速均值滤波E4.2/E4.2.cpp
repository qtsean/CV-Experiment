#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;

void MeanFilter(Mat org, Mat out, int window_size);
Mat changeWindowSize(Mat org, int windowSize);

int main()
{
	Mat org = imread("mountain.jpg");
	Mat out = Mat::zeros(org.rows, org.cols, org.type());
	int window_size = 5;
	Mat newOrg = changeWindowSize(org, window_size);
	MeanFilter(newOrg, out, window_size);
	namedWindow("changed");
	imshow("origin", org);
	imshow("changed", out);
	waitKey(0);
	return 0;
}

Mat changeWindowSize(Mat org, int windowSize)
{
	windowSize += 2;
	int addLength = 0.5*windowSize;
	Mat newOrg = Mat::zeros(org.rows + 2 * addLength, org.cols + 2 * addLength, org.type());
	for (int i = 0; i < newOrg.rows; i++)
	{
		for (int j = 0; j < newOrg.cols; j++)
		{
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

void MeanFilter(Mat org, Mat out, int window_size)
{
	double Z = window_size * window_size;
	int w = window_size / 2;
	double ***S = new double**[org.rows];
	for (int i = 0; i < org.rows; i++)
	{
		S[i] = new double*[org.cols];
	}
	for (int i = 0; i < org.rows; i++)
	{
		for (int j = 0; j < org.cols; j++)
		{
			S[i][j] = new double[org.channels()];
		}
	}
	for (int i = 0; i < org.rows; i++)
	{
		for (int j = 0; j < org.cols; j++)
		{
			if (i == 0 && j == 0)
			{
				for (int c = 0; c < org.channels(); c++)
				{
					S[i][j][c] = org.at<Vec3b>(i, j)[c];
				}
			}
			if (i == 0 && j != 0)
			{
				for (int c = 0; c < org.channels(); c++)
				{
					S[i][j][c] = S[i][j - 1][c] + org.at<Vec3b>(i, j)[c];
				}
			}
			if (i != 0)
			{
				for (int c = 0; c < org.channels(); c++)
				{
					S[i][j][c] = S[i - 1][j][c];
					for (int k = 0; k <= j; k++)
					{
						S[i][j][c] += org.at<Vec3b>(i, k)[c];
					}
				}
			}
		}
	}
	for (int i = w+1; i < org.rows-w-1; i++)
	{
		for (int j = w+1; j < org.cols-w-1; j++)
		{
			for (int c = 0; c < org.channels(); c++)
			{
				org.at<Vec3b>(i, j)[c] = 1.0 / Z * (S[i + w][j + w][c] + S[i - w - 1][j - w - 1][c] - S[i + w][j - w - 1][c] - S[i - w - 1][j + w][c]);
//				org.at<Vec3b>(i, j)[c] = 1.0 / Z * (S[i + w][j + w][c] + S[i - w ][j - w ][c] - S[i + w][j - w ][c] - S[i - w ][j + w][c]);
			}
		}
	}
	for (int i = 0; i < out.rows; i++)
	{
		for (int j = 0; j < out.cols; j++)
		{
			for (int c = 0; c < out.channels(); c++)
			{
				out.at<Vec3b>(i, j)[c] = org.at<Vec3b>(i + w, j + w)[c];
			}
		}
	}
}