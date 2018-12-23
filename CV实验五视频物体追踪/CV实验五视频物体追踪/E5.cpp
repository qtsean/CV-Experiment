#include<iostream>
#include<cstdio>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<math.h>
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

bool compare(double **h, double **temp);

void getVector(Mat &frame)
{
	Rect rect(370, 0, 80, 80);
	Mat image = frame(rect);
	namedWindow("test");
	imshow("test", image);
	imwrite("taxi.png", image);
}

void process(Mat &frame, double** h, int width, int length, int& count, int& currentrow, int& currentcol)
{
	currentrow -= 20;
	if (currentrow < 0)
	{
		currentrow = 0;
	}
	currentcol -= 20;
	if (currentcol < 0)
	{
		currentcol = 0;
	}
	bool complete = 0;
//	cout << width << "\t" << length << endl;
//	cout << frame.rows << "\t" << frame.cols << endl;
	
	
	
	for (int i = currentrow; i < frame.rows - width; i += 5)
	{
		for (int j = currentcol; j < frame.cols - length; j += 5)
		{
			/*cout << "i: " << i;
			cout << "   j: "<<j << endl;*/
			double **temp = new double*[3];
			for (int m = 0; m < 3; m++)
			{
				temp[m] = new double[256];
			}
			for (int m = 0; m < 3; m++)
			{
				for (int n = 0; n < 256; n++)
				{
					temp[m][n] = 0;
				}
			}
			int count[3] = { 0,0,0 };
			for (int m = 0; m < width; m++)
			{
				for (int n = 0; n < length; n++)
				{
					for (int c = 0; c < 3; c++)
					{
//						cout << "i: " << i << "\t" << "m: " << m << endl;
//						cout << "j: " << "\t" << "n: " << n << endl;
						temp[c][frame.at<Vec3b>(i + m, j + n)[c]]++;
						count[c]++;
					}
				}
			}
			for (int m = 0; m < 3; m++)
			{
				for (int n = 0; n < 256; n++)
				{
					if (count[m] != 0)
					{
						temp[m][n] /= count[m];
					}
				}
			}
			if (complete=compare(h, temp))
			{
				for (int m = 0; m < width; m++)
				{
					frame.at<Vec3b>(i + m, j)[0] = 255;
					frame.at<Vec3b>(i + m, j)[1] = 255;
					frame.at<Vec3b>(i + m, j)[2] = 255;
					frame.at<Vec3b>(i + m, j+length)[0] = 255;
					frame.at<Vec3b>(i + m, j+length)[1] = 255;
					frame.at<Vec3b>(i + m, j+length)[2] = 255;
				}
				for (int m = 0; m < length; m++)
				{
					frame.at<Vec3b>(i, j + m)[0] = 255;
					frame.at<Vec3b>(i, j + m)[1] = 255;
					frame.at<Vec3b>(i, j + m)[2] = 255;
					frame.at<Vec3b>(i + width, j + m)[0] = 255;
					frame.at<Vec3b>(i + width, j + m)[1] = 255;
					frame.at<Vec3b>(i + width, j + m)[2] = 255;
					
				}
				currentrow = i;
				currentcol = j;
			}
			for (int i = 0; i < 3; i++)
			{
				delete []temp[i];
			}
			delete []temp;
			if (complete)
			{
				/*cout << "FOUND" << endl;*/
				return;
			}
		}
	}
	if (complete == false) 
	{
		for (int i = 0; i < frame.rows - width; i += 5)
		{
			for (int j = 0; j < frame.cols - length; j += 5)
			{
				/*cout << "Second loop i: " << i;
				cout << "   j: " << j << endl;*/
				double **temp = new double*[3];
				for (int m = 0; m < 3; m++)
				{
					temp[m] = new double[256];
				}
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 256; n++)
					{
						temp[m][n] = 0;
					}
				}
				int count[3] = { 0,0,0 };
				for (int m = 0; m < width; m++)
				{
					for (int n = 0; n < length; n++)
					{
						for (int c = 0; c < 3; c++)
						{
							//						cout << "i: " << i << "\t" << "m: " << m << endl;
							//						cout << "j: " << "\t" << "n: " << n << endl;
							temp[c][frame.at<Vec3b>(i + m, j + n)[c]]++;
							count[c]++;
						}
					}
				}
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 256; n++)
					{
						if (count[m] != 0)
						{
							temp[m][n] /= count[m];
						}
					}
				}
				if (complete = compare(h, temp))
				{
					for (int m = 0; m < width; m++)
					{
						frame.at<Vec3b>(i + m, j)[0] = 255;
						frame.at<Vec3b>(i + m, j)[1] = 255;
						frame.at<Vec3b>(i + m, j)[2] = 255;
						frame.at<Vec3b>(i + m, j + length)[0] = 255;
						frame.at<Vec3b>(i + m, j + length)[1] = 255;
						frame.at<Vec3b>(i + m, j + length)[2] = 255;
					}
					for (int m = 0; m < length; m++)
					{
						frame.at<Vec3b>(i, j + m)[0] = 255;
						frame.at<Vec3b>(i, j + m)[1] = 255;
						frame.at<Vec3b>(i, j + m)[2] = 255;
						frame.at<Vec3b>(i + width, j + m)[0] = 255;
						frame.at<Vec3b>(i + width, j + m)[1] = 255;
						frame.at<Vec3b>(i + width, j + m)[2] = 255;

					}
					currentrow = i;
					currentcol = j;
				}
				for (int i = 0; i < 3; i++)
				{
					delete[]temp[i];
				}
				delete[]temp;
				if (complete)
				{
					/*cout << "FOUND" << endl;*/
					return;
				}
			}
		}
	}
}

bool compare(double **h, double **temp)
{
	/*double sum[3] = { 0,0,0 };
	for (int c = 0; c < 3; c++)
	{
		for (int i = 0; i < 256; i++)
		{
			sum[c] += sqrt(h[c][i] * temp[c][i]);
		}
	}
	if (sum[0] + sum[1] + sum[2] > 2.8)
		return true;
	else
		return false;*/
	double sumAB = 0;
	double sumA = 0;
	double sumB = 0;

	for (int c = 0; c < 3; c++)
	{
		for (int i = 0; i < 256; i++)
		{
			sumAB += h[c][i] * temp[c][i];
			sumA += h[c][i] * h[c][i];
			sumB += temp[c][i] * temp[c][i];
		}
	}

	if (sumAB / (sqrt(sumA)*sqrt(sumB)) > 0.8)
		return true;
	else
		return false;
}

double** histogram(Mat &image)
{
	double **h = new double*[3];
	for (int i = 0; i < 3; i++)
	{
		h[i] = new double[256];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			h[i][j] = 0;
		}
	}
	int count[3] = { 0, 0, 0 };
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				h[c][image.at<Vec3b>(i, j)[c]]++;
				count[c]++;
			}
		}
	}
	for (int m = 0; m < 3; m++)
	{
		for (int n = 0; n < 256; n++)
		{
			if (count[m] != 0) 
			{
				h[m][n] /= count[m];
			}
		}
	}

	return h;
}

int main()
{
	string filename = "1.avi";
	VideoCapture capture;
	capture.open(filename);

	double rate = capture.get(CV_CAP_PROP_FPS);
	int delay = cvRound(1000.000 / rate);

	Mat car = imread("taxi.png");

	int windowWidth = car.rows;
	int windowLength = car.cols;


	double **h = histogram(car);

	int count = 0;
	int &c = count;
	int currentrow = 0;
	int currentcol = 0;
	int &cr = currentrow;
	int &cc = currentcol;
	if (!capture.isOpened())
	{
		return -1;
	}
	else
	{
		int count = 1;
		while (true)
		{

			Mat frame;
			capture >> frame;
			if (frame.empty())
				break;
			if (count > 2) {
				process(frame, h, windowWidth, windowLength, c, cr, cc);
			}
			//if(count==10)
			//	getVector(frame);
			namedWindow("สำฦต");
			imshow("สำฦต", frame);
			waitKey(delay);
			count++;
			if (count == 54)
				break;
		}
	}
	
	system("pause");
	return 0;
}