#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;

int findIndex(int *a, int x,int length)
{
	for (int i = 0; i < length; i++)
	{
		if (a[i] == x)
		{
			return i;
		}
	}
	return -1;
}

int findMax(int a[], int length)
{
	/*for (int i = 0; i < length; i++)
	{
		cout << a[i] << endl;
	}*/
	int *temp = new int[length];
	for (int i = 0; i < length; i++)
	{
		temp[i] = a[i];
	}
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length-1; j++)
		{
			if (temp[j] < temp[j + 1])
			{
				int t = temp[j];
				temp[j] = temp[j + 1];
				temp[j + 1] = t;
			}
		}
	}
	for (int i = 0; i < length; i++)
	{
		cout << temp[i] << endl;
	}
	for (int i = 0; i < length; i++)
	{
		if (temp[1] == a[i])
		{
			return i;
		}
	}
}

void process1(Mat &img, Mat &out1, Mat &out2)
{
	//记录当前连通域个数,start from 0
	int count = 0;
	//创建一个和图片一样大的二维数组记录每个点属于的连通域
	int** L = new int *[img.rows];
	for (int i = 0; i < img.rows; i++)
	{
		L[i] = new int[img.cols];
	}
	//先初始化为0
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			L[i][j] = 0;
		}
	}
	//遍历图片,寻找连通域
	for (int i = 0; i < img.rows; i++)
	{

		for (int j = 0; j < img.cols; j++)
		{
//			cout << "[" << i << " , " << j << "]" << endl;
			if (i == 0 && j == 0)
			{
//				cout << "[" << i << " , " << j << "]" << "L[i][j]= "<<L[i][j]<<"  count: " << count << endl;
				L[i][j] = count;
				count++;
//				cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
			}
			else if (i == 0)
			{
				if (img.at<Vec3b>(i, j) == img.at<Vec3b>(i, j - 1))
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = L[i][j - 1];
				}
				else
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = count;
					count++;
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
				}
			}
			else if (j == 0)
			{
				if (img.at<Vec3b>(i, j) == img.at<Vec3b>(i - 1, j))
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = L[i - 1][j];
				}
				else
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = count;
					count++;
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
				}
			}
			else
			{
				if (img.at<Vec3b>(i, j) != img.at<Vec3b>(i, j - 1) && img.at<Vec3b>(i, j) != img.at<Vec3b>(i - 1, j))
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = count;
					count++;
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
				}
				else if (img.at<Vec3b>(i, j) == img.at<Vec3b>(i, j - 1) && img.at<Vec3b>(i, j) != img.at<Vec3b>(i - 1, j))
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = L[i][j - 1];
				}
				else if (img.at<Vec3b>(i, j) == img.at<Vec3b>(i - 1, j) && img.at<Vec3b>(i, j) != img.at<Vec3b>(i, j - 1))
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = L[i - 1][j];
				}
				else if (img.at<Vec3b>(i, j) == img.at<Vec3b>(i - 1, j) && img.at<Vec3b>(i, j) == img.at<Vec3b>(i, j - 1) && L[i - 1][j] == L[i][j - 1])
				{
	//				cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = L[i - 1][j];
				}
				else if (img.at<Vec3b>(i, j) == img.at<Vec3b>(i - 1, j) && img.at<Vec3b>(i, j) == img.at<Vec3b>(i, j - 1) && L[i - 1][j] != L[i][j - 1])
				{
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
					L[i][j] = L[i][j - 1];
					for (int m = 0; m < i + 1; m ++)
					{
						if (m != i)
						{
							for (int n = 0; n < img.cols; n++)
							{
								if (L[m][n] == L[i][j])
								{
									L[m][n] = L[i - 1][j];
								}
							}
						}
						else
						{
							for (int n = 0; n <= j; n++)
							{
								if (L[m][n] == L[i][j])
								{
									L[m][n] = L[i - 1][j];
								}
							}
						}
					}
					//count--;
//					cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << "  count: " << count << endl;
				}
				
			}
		}

	}
	cout << endl;
	int *area = new int[count];
	for (int i = 0; i < count; i++)
		area[i] = 0;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			area[L[i][j]]++;
		}
	}
	int countarea = 0;
	for (int i = 0; i < count; i++)
	{
		if (area[count] != 0)
		{
			countarea++;
		}
	}
	int *map = new int[countarea];
	for (int i = 0; i < countarea; i++)
	{
		map[i] = 0;
	}
	int rearrange = 0;
	
	for (int i = 0; i < count; i++)
	{
		if (area[i] != 0)
		{
			map[rearrange] = i;
			rearrange++;
		}
	}
	//for (int i = 0; i < rearrange; i++)
	//{
	//	cout << map[i] << endl;
	//}
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			L[i][j] = findIndex(map, L[i][j], rearrange);
		}
	}
	
	//int flag = -1;
	/*for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			
			cout << "[" << i << " , " << j << "]" << "L[i][j]= " << L[i][j] << endl;
			
		}
		cout << endl;
	}*/
	delete area;
	area = new int[rearrange];
	for (int i = 0; i < rearrange; i++)
		area[i] = 0;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			area[L[i][j]]++;
		}
	}
	/*cout << "output area:" << endl;
	for (int i = 0; i < rearrange; i++)
	{
		cout << area[i] << endl;
	}*/
	//根据连通区域输出图片
//	cout << count << endl;


	for (int i = 0; i < out1.rows; i++)
	{
		for (int j = 0; j < out1.cols; j++)
		{
			out1.at<Vec3b>(i,j)[0] = L[i][j] * 30;
			out1.at<Vec3b>(i,j)[1] = L[i][j] * 15;
			out1.at<Vec3b>(i, j)[2] = L[i][j] * 25;
		}
	}
	int index = findMax(area, rearrange);
	cout << rearrange;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (L[i][j] == index)
			{
				out2.at<Vec3b>(i, j)[0] = 255;
				out2.at<Vec3b>(i, j)[1] = 255;
				out2.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}
}

int main()
{
	Mat img = imread("D:\\CV实验\\CV实验六图像结构一\\CV实验六图像结构一\\3.png");
//	Mat img = imread("D:\\CV实验\\CV实验六图像结构一\\CV实验六图像结构一\\3.png");
//	Mat binaryimg = Mat::zeros(img.size(), img.type());
//	threshold(img, binaryimg, 240, 255, CV_THRESH_BINARY);
//	Mat test = imread("D:\\CV实验\\CV实验五视频物体追踪\\CV实验五视频物体追踪\\taxi.png");
//	cout << img.channels();
//	cout << img.rows << "\t" << img.cols << endl;
	Mat out1(img.size(), CV_8UC3);
//	cout << test.type();
	Mat out2 = Mat::zeros(img.rows, img.cols, img.type());

//	namedWindow("test");
//	imshow("test", test);
	int countfind = 0;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img.at<Vec3b>(i, j)[0] != 0 || img.at<Vec3b>(i, j)[1] != 0 || img.at<Vec3b>(i, j)[2] != 0)
			{
				img.at<Vec3b>(i, j)[0] = 255;
				img.at<Vec3b>(i, j)[1] = 255;
				img.at<Vec3b>(i, j)[2] = 255;
			}
//			cout << img.at<Vec3b>(i,j);
		}
	}
//	cout << img.at<Vec3b>(0, 0) << endl;
//	cout << img.at<Vec3b>(0, 1) << endl;
//	cout << (img.at<Vec3b>(0, 0) == img.at<Vec3b>(0, 1));
//	cout << countfind;
	process1(img, out1,out2);

	namedWindow("org");
	imshow("org", img);
	namedWindow("out1");
	imshow("out1", out1);
	imwrite("out1.png",out1);
	namedWindow("out2");
	imshow("out2", out2);
	
	
	waitKey(0);
	return 0;

}