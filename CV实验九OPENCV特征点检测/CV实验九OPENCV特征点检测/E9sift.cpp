#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/nonfree/nonfree.hpp"//SIFT相关
#include "opencv2/legacy/legacy.hpp"//匹配器相关
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	//1.SIFT特征点提取——detect()方法
	Mat srcImg1 = imread("酒1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat srcImg2 = imread("酒2.jpg", CV_LOAD_IMAGE_COLOR);
	Mat dstImg1, dstImg2;
	SiftFeatureDetector siftDetector;//SiftFeatureDetector是SIFT类的别名  
	vector<KeyPoint> keyPoints1;
	vector<KeyPoint> keyPoints2;
	siftDetector.detect(srcImg1, keyPoints1);
	siftDetector.detect(srcImg2, keyPoints2);
	drawKeypoints(srcImg1, keyPoints1, dstImg1);
	drawKeypoints(srcImg2, keyPoints2, dstImg2);
	//namedWindow("dst");
	//imwrite("winesift_feature1.jpg", dstImg1);
	//imwrite("winesift_feature2.jpg", dstImg2);
	//2.特征点描述符(特征向量)提取——compute()方法
	SiftDescriptorExtractor descriptor;//SiftDescriptorExtractor是SIFT类的别名  
	Mat description1;
	Mat description2;
	descriptor.compute(srcImg1, keyPoints1, description1);//注意原图和特征点要对应，不要写错
	descriptor.compute(srcImg2, keyPoints2, description2);
	//imshow("description1", description1);
	//imshow("description2", description2);
	//3.使用暴力匹配器进行暴力匹配——BruteForceMatcher类的match()方法
	BruteForceMatcher<L2<float>> matcher;//实例化暴力匹配器
	vector<DMatch> matches; //定义匹配结果变量
	matcher.match(description1, description2, matches);//实现描述符之间的匹配
	//4.对匹配结果进行筛选（依据DMatch结构体中的float类型变量distance进行筛选）
	float minDistance = 100;
	float maxDistance = 0;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < minDistance)
			minDistance = matches[i].distance;
		if (matches[i].distance > maxDistance)
			maxDistance = matches[i].distance;
	}
	cout << "minDistance: " << minDistance << endl;
	cout << "maxDistance: " << maxDistance << endl;
	vector<DMatch> goodMatches;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < 2 * minDistance)
		{
			goodMatches.push_back(matches[i]);
		}
	}
	//5.绘制匹配结果——drawMatches()
	Mat dstImg3;
	drawMatches(srcImg1, keyPoints1, srcImg2, keyPoints2, goodMatches, dstImg3);
	imwrite("酒sift_match.jpg", dstImg3);
	waitKey(0);
	return 0;
}
