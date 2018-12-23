#include <opencv2/opencv.hpp>
#include<opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include<highgui/highgui.hpp>
#include<vector>
#include<iostream>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/core/core.hpp>

using namespace std;
using namespace cv;

void process(Mat& src, Mat& out, Mat& paste)
{
	Mat obj;
	obj = imread("obj2.jpg");
	Mat p2(src.rows, src.cols, src.type());
	Mat p1(obj.rows, obj.cols, obj.type());
	src.copyTo(p2);
	obj.copyTo(p1);
	resize(paste, paste, Size(obj.cols,obj.rows));
	Mat dstImg1, dstImg2;

	SurfFeatureDetector Detector(1000);

	vector<KeyPoint> keyPoints1;
	vector<KeyPoint> keyPoints2;

	Detector.detect(p1, keyPoints1);
	Detector.detect(p2, keyPoints2);

	drawKeypoints(p1, keyPoints1, dstImg1);
	drawKeypoints(p2, keyPoints2, dstImg2);

	SurfDescriptorExtractor descriptor;

	Mat description1;
	Mat description2;

	descriptor.compute(p1, keyPoints1, description1);
	descriptor.compute(p2, keyPoints2, description2);

	/*FlannBasedMatcher matcher;
	vector<vector<DMatch> > matchePoints;
	vector<DMatch> GoodMatchePoints;
	vector<Mat> train_desc(1, description1);
	matcher.add(train_desc);
	matcher.train();
	matcher.knnMatch(description2, matchePoints, 2);
	cout << "total match points: " << matchePoints.size() << endl;

	for (int i = 0; i < matchePoints.size(); i++)
	{
		if (matchePoints[i][0].distance < 0.5 * matchePoints[i][1].distance)
		{
			GoodMatchePoints.push_back(matchePoints[i][0]);
		}
	}
	cout << GoodMatchePoints.size();*/


	//BruteForceMatcher<L2<float>>matcher;    //实例化暴力匹配器
	//vector<DMatch>matches;  //定义匹配结果变量
	//matcher.match(description1, description2, matches);  //实现描述符之间的匹配

	// //计算向量距离的最大值与最小值
	//double max_dist = 0, min_dist = 100;
	//for (int i = 0; i < description1.rows; i++)
	//{
	//	if (matches.at(i).distance > max_dist)
	//		max_dist = matches[i].distance;
	//	if (matches.at(i).distance < min_dist)
	//		min_dist = matches[i].distance;
	//}
	//cout << "min_distance=" << min_dist << endl;
	//cout << "max_distance=" << max_dist << endl;

	////匹配结果筛选  
	//vector<DMatch>GoodMatchePoints;
	//for (int i = 0; i < matches.size(); i++)
	//{
	//	if (matches[i].distance < 3 * min_dist)
	//		GoodMatchePoints.push_back(matches[i]);
	//}

	BFMatcher matcher(NORM_L2, true);
	vector<DMatch> matches;
	matcher.match(description1, description2, matches);

	
	/*Mat dstImg3;
	drawMatches(p2, keyPoints2, p1, keyPoints1, GoodMatchePoints, dstImg3);*/
//	imwrite("test1.jpg", dstImg3);
	/*vector<Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < GoodMatchePoints.size(); i++)
	{
		imagePoints2.push_back(keyPoints2[GoodMatchePoints[i].queryIdx].pt);
		imagePoints1.push_back(keyPoints1[GoodMatchePoints[i].trainIdx].pt);
	}

	Mat homograph = findHomography(imagePoints1, imagePoints2, CV_RANSAC);*/

	vector<Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < matches.size(); i++)
	{
		imagePoints2.push_back(keyPoints2[matches[i].trainIdx].pt);
		imagePoints1.push_back(keyPoints1[matches[i].queryIdx].pt);
	}

	Mat homograph = findHomography(imagePoints1, imagePoints2, CV_RANSAC);

	cout << homograph << endl << endl;

	Mat t1;
	warpPerspective(paste, t1, homograph, Size(p2.cols, p2.rows));
//	imwrite("trans1.jpg", t1);

	int width = p2.cols;
	int height = p2.rows;

	Mat dst(height, width, CV_8UC3);
	dst.setTo(0);

	
	p2.copyTo(dst(Rect(0, 0, p2.cols, p2.rows)));
	for (int i = 0; i < t1.rows; i++)
	{
		for (int j = 0; j < t1.cols; j++)
		{
			if (t1.at<Vec3b>(i, j)[0] != 0 || t1.at<Vec3b>(i, j)[1] != 0 || t1.at<Vec3b>(i, j)[2] != 0)
			{
				dst.at<Vec3b>(i, j)[0] = t1.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = t1.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = t1.at<Vec3b>(i, j)[2];
//				cout << "changed" << endl;
			}
		}
	}

	//imwrite("result.jpg", dst);
	out = dst;
//	waitKey(0);
}

int main()
{
	Mat paste = imread("apple.jpg");
	VideoCapture capture;
	VideoCapture capture2;
	capture.open("video1.mp4");
	capture2.open("S28E03(720p).mp4");
	Size s(1920, 1080);
	VideoWriter writer("bad.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, s);

	double rate = capture.get(CV_CAP_PROP_FPS);
	double rate1 = capture2.get(CV_CAP_PROP_FPS);
	int delay = cvRound(1000.00 / rate);
	int delay2 = cvRound(1000.00 / rate1);

	for (int i = 0; i < 45; i++)
	{
		Mat f;
		capture2 >> f;
	}

	if (!capture.isOpened())
	{
		return -1;
	}
	else
	{
		while (true)
		{
			Mat frame, out;
			Mat framesimpson;
			capture >> frame;
			capture2 >> framesimpson;
			flip(frame, frame, 0);
			flip(frame, frame, 1);

			if (frame.empty())
			{
				break;
			}
//			imshow("origin", frame);
			process(frame, out, framesimpson);
			imshow("out", out);
			writer << out;
			waitKey(1);
		}
	}



	
	return 0;
}