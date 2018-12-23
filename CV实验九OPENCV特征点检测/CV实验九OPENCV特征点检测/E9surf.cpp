#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/nonfree/nonfree.hpp"//SIFT���
#include "opencv2/legacy/legacy.hpp"//ƥ�������
#include <iostream>
using namespace cv;
using namespace std;

void main()
{
	Mat srcImg1 = imread("��1.jpg");
	Mat srcImg2 = imread("��2.jpg");
	//����SURF������������
	SurfFeatureDetector surfDetector(400);  //HessianThreshold

	//����KeyPoint����
	vector<KeyPoint>keyPoints1;
	vector<KeyPoint>keyPoints2;
	//��������
	surfDetector.detect(srcImg1, keyPoints1);
	surfDetector.detect(srcImg2, keyPoints2);
	//����������(�ؼ���)
	Mat feature_pic1, feature_pic2;
	drawKeypoints(srcImg1, keyPoints1, feature_pic1, Scalar::all(-1));
	drawKeypoints(srcImg2, keyPoints2, feature_pic2, Scalar::all(-1));
	//��ʾԭͼ
	//imshow("src1", srcImg1);
	//imshow("src2", srcImg2);
	//��ʾ���
	//imwrite("surf_feature1.jpg", feature_pic1);
	//imwrite("surf_feature2.jpg", feature_pic2);

	//���������������� / ����������ȡ
	SurfDescriptorExtractor descriptor;
	Mat description1;
	descriptor.compute(srcImg1, keyPoints1, description1);
	Mat description2;
	descriptor.compute(srcImg2, keyPoints2, description2);
	cout << description1.cols << endl;
	cout << description1.rows << endl;

	//����BFMatch����ƥ��
	BruteForceMatcher<L2<float>>matcher;    //ʵ��������ƥ����
	vector<DMatch>matches;  //����ƥ��������
	matcher.match(description1, description2, matches);  //ʵ��������֮���ƥ��

	 //����������������ֵ����Сֵ
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

	//ƥ����ɸѡ  
	vector<DMatch>good_matches;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < 2 * min_dist)
			good_matches.push_back(matches[i]);
	}

	Mat result;
	//drawMatches(srcImg1, keyPoints1, srcImg2, keyPoints2, matches, result, Scalar::all(-1), Scalar::all(-1));
	drawMatches(srcImg1, keyPoints1, srcImg2, keyPoints2, good_matches, result);
	imwrite("��surf_Match_Result.jpg", result);

	waitKey(0);
}