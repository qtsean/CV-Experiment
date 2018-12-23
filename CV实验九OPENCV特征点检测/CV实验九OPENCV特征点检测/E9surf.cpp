#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/nonfree/nonfree.hpp"//SIFT相关
#include "opencv2/legacy/legacy.hpp"//匹配器相关
#include <iostream>
using namespace cv;
using namespace std;

void main()
{
	Mat srcImg1 = imread("酒1.jpg");
	Mat srcImg2 = imread("酒2.jpg");
	//定义SURF特征检测类对象
	SurfFeatureDetector surfDetector(400);  //HessianThreshold

	//定义KeyPoint变量
	vector<KeyPoint>keyPoints1;
	vector<KeyPoint>keyPoints2;
	//特征点检测
	surfDetector.detect(srcImg1, keyPoints1);
	surfDetector.detect(srcImg2, keyPoints2);
	//绘制特征点(关键点)
	Mat feature_pic1, feature_pic2;
	drawKeypoints(srcImg1, keyPoints1, feature_pic1, Scalar::all(-1));
	drawKeypoints(srcImg2, keyPoints2, feature_pic2, Scalar::all(-1));
	//显示原图
	//imshow("src1", srcImg1);
	//imshow("src2", srcImg2);
	//显示结果
	//imwrite("surf_feature1.jpg", feature_pic1);
	//imwrite("surf_feature2.jpg", feature_pic2);

	//计算特征点描述符 / 特征向量提取
	SurfDescriptorExtractor descriptor;
	Mat description1;
	descriptor.compute(srcImg1, keyPoints1, description1);
	Mat description2;
	descriptor.compute(srcImg2, keyPoints2, description2);
	cout << description1.cols << endl;
	cout << description1.rows << endl;

	//进行BFMatch暴力匹配
	BruteForceMatcher<L2<float>>matcher;    //实例化暴力匹配器
	vector<DMatch>matches;  //定义匹配结果变量
	matcher.match(description1, description2, matches);  //实现描述符之间的匹配

	 //计算向量距离的最大值与最小值
	double max_dist = 0, min_dist = 100;
	for (int i = 0; i < description1.rows; i++)
	{
		if (matches.at(i).distance > max_dist)
			max_dist = matches[i].distance;
		if (matches.at(i).distance < min_dist)
			min_dist = matches[i].distance;
	}
	cout << "min_distance=" << min_dist << endl;
	cout << "max_distance=" << max_dist << endl;

	//匹配结果筛选  
	vector<DMatch>good_matches;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < 2 * min_dist)
			good_matches.push_back(matches[i]);
	}

	Mat result;
	//drawMatches(srcImg1, keyPoints1, srcImg2, keyPoints2, matches, result, Scalar::all(-1), Scalar::all(-1));
	drawMatches(srcImg1, keyPoints1, srcImg2, keyPoints2, good_matches, result);
	imwrite("酒surf_Match_Result.jpg", result);

	waitKey(0);
}