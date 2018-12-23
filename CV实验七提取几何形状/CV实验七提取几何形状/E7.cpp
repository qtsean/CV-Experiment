#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<math.h>

#define pi 3.1415926

using namespace std;
using namespace cv;

void showCircle(Mat& contour, Mat &out, int** L, int a, int b, int r);
void statistics(Mat& contour, Mat &out, int***H);
void process(Mat &contour, Mat &out);
bool edge(Mat &contour, int x, int y);
void draw(Mat &contour, Mat &out, int** L);
int findMax(int ***H, int a, int b, int r);

int findMax(int ***H, int a, int b, int r)
{
	int max = 0;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			for (int c = 0; c < r; c++)
			{
				if (H[i][j][c] > max)
				{
					max = H[i][j][c];
				}
			}
		}
	}
	return max;
}
void draw(Mat &contour, Mat &out, int** L)
{
	int row = contour.rows;
	int col = contour.cols;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (contour.at<uchar>(i, j) == 255)
			{
				if (L[i][j] == 1)
				{
					out.at<Vec3b>(i, j)[0] = 255;
					out.at<Vec3b>(i, j)[1] = 255;
					out.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
	}
}

bool edge(Mat &contour, int x, int y)
{
	if (contour.at<uchar>(x-1, y-1) == 255)
		return true;
	if (contour.at<uchar>(x, y-1) == 255)
		return true;
	if (contour.at<uchar>(x+1, y-1) == 255)
		return true;
	if (contour.at<uchar>(x-1, y) == 255)
		return true;
	if (contour.at<uchar>(x, y) == 255)
		return true;
	if (contour.at<uchar>(x+1, y) == 255)
		return true;
	if (contour.at<uchar>(x-1, y+1) == 255)
		return true;
	if (contour.at<uchar>(x, y+1) == 255)
		return true;
	if (contour.at<uchar>(x+1, y+1) == 255)
		return true;

	return false;
}

void statistics(Mat& contour, Mat &out, int***H)
{
	int row = contour.rows;
	int col = contour.cols;

	//创建一个数组，存大致圆的位置
	int **L = new int*[row];
	for (int i = 0; i < row; i++)
	{
		L[i] = new int[col];
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			L[i][j] = 0;
		}
	}
	int max = findMax(H, row, col, row);
	cout << "max: " << max << endl;
	for (int a = 0; a < row; a++)
	{
		for (int b = 0; b < col; b++)
		{
			for (int r = 0; r < row; r++)
			{
				if (H[a][b][r] > max-70)
				{
					cout << "show" << endl;
					cout << "a: " << a << "\tb: " << b << "\tr: " << r << endl;
					showCircle(contour, out,L, a, b, r);
				}
			}
		}
	}
	cout << "draw" << endl;
	draw(contour, out, L);
}

void showCircle(Mat& contour, Mat &out, int **L, int a, int b, int r)
{
	int row = contour.rows;
	int col = contour.cols;
	for (double theta = 0; theta < 2*pi; theta+=0.025)
	{
		int x = a + cos(theta)*r;
		int y = b - sin(theta)*r;
		if (x < 0 || x >= row-1 || y<0 || y>= col-1)
			continue;
		/*out.at<Vec3b>(x, y)[0] = 0;
		out.at<Vec3b>(x, y)[1] = 0;
		out.at<Vec3b>(x, y)[2] = 255;*/
		L[x][y] = 1;
		L[x + 1][y] = 1;
		L[x][y + 1] = 1;
		L[x + 1][y + 1] = 1;
		L[x - 1][y - 1] = 1;
		L[x - 1][y] = 1;
		L[x - 1][y + 1] = 1;
		L[x][y - 1] - 1;
		L[x + 1][y - 1] = 1;

	}
}

void process(Mat &contour, Mat &out)
{
	//建立3维数组，对应a，b，r，且a范围0到row，b范围0到col，r范围0到row
	int row = contour.rows;
	int col = contour.cols;
	int ***H = new int**[row];
	for (int i = 0; i < row; i++)
	{
		H[i] = new int*[col];
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j <  col; j++)
		{
			H[i][j] = new int[row];
		}
	}
	for (int a = 0; a < row; a++)
	{
		for (int b = 0; b < col; b++)
		{
			for (int r = 0; r < row; r++)
			{
				H[a][b][r] = 0;
			}
		}
	}
	//遍历contour
	for (int i = 0; i < row; i++)
	{
		cout << i << " / " << row << endl;
		for (int j = 0; j < col; j++)
		{
			if (contour.at<uchar>(i, j) == 255) 
			{
				for (int r = 15; r < row / 2; r++)//半径从1开始
				{
					for (double theta = 0; theta < 2*pi; theta+=0.025)
					{
						int a = i - r * cos(theta);
						int b = j + r * sin(theta);
						if (a < 0 || b < 0 || a >= row-1 || b >= col-1)
							continue;
						H[a][b][r]++;
						H[a + 1][b][r]++;
						H[a][b + 1][r]++;
						H[a + 1][b + 1][r]++;
						//					cout << "x: " << i << "\ty: " << j << "\tr: " << r << endl;
					}
				}
			}
		}
	}
	cout << "statistics" << endl;
	statistics(contour, out, H);
}

int main()
{
	Mat img = imread("circle.png");
	Mat imgGray;
	Mat contour;
	Mat out = Mat::zeros(img.size(), img.type());
	cvtColor(img, imgGray, CV_BGR2GRAY);
	Canny(imgGray, contour, 150 ,300, 3);
//	cout << img.channels() << endl;
//	cout << contour.channels() << endl;

	
	process(contour, out);

	namedWindow("contour");
	imshow("contour", contour);
	namedWindow("out");
	imshow("out", out);
	waitKey(0);
	return 0;
}