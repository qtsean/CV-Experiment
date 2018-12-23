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



int main()
{
	Mat p1 = imread("32.jpg");
	Mat p2 = imread("31.jpg");
//	Mat p3 = imread("3.jpg");

	Mat dstImg1, dstImg2;

	SurfFeatureDetector Detector(2000);

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


	FlannBasedMatcher matcher;
	vector<vector<DMatch> > matchePoints;
	vector<DMatch> GoodMatchePoints;
	vector<Mat> train_desc(1, description1);
	matcher.add(train_desc);
	matcher.train();
	matcher.knnMatch(description2, matchePoints, 2);
	cout << "total match points: " << matchePoints.size() << endl;
	// Lowe's algorithm,获取优秀匹配点
	for (int i = 0; i < matchePoints.size(); i++)
	{
		if (matchePoints[i][0].distance < 0.35 * matchePoints[i][1].distance)
		{
			GoodMatchePoints.push_back(matchePoints[i][0]);
		}
	}
	cout << GoodMatchePoints.size();

	Mat dstImg3;
	drawMatches(p2, keyPoints2, p1, keyPoints1, GoodMatchePoints, dstImg3);
	imwrite("test1.jpg", dstImg3);

	vector<Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i < GoodMatchePoints.size(); i++)
	{
		imagePoints2.push_back(keyPoints2[GoodMatchePoints[i].queryIdx].pt);
		imagePoints1.push_back(keyPoints1[GoodMatchePoints[i].trainIdx].pt);
	}

	Mat homograph = findHomography(imagePoints1, imagePoints2, CV_RANSAC);

	cout << homograph << endl << endl;

	Mat t1;
	warpPerspective(p1, t1, homograph, Size(2*p2.cols, 1.2*p2.rows));
	imwrite("trans1.jpg", t1);

	int width = t1.cols;
	int height = max(p2.rows,t1.rows);

	Mat dst(height, width, CV_8UC3);
	dst.setTo(0);
	
	t1.copyTo(dst(Rect(0, 0, t1.cols, t1.rows)));
	p2.copyTo(dst(Rect(0, 0, p2.cols, p2.rows)));

	imwrite("result.jpg", dst);

	waitKey(0);
	return 0;




}