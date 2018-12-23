#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <iostream>
using namespace cv;
using namespace std;


int main()
{

	//-- ��ȡͼ��
	Mat img_1 = imread("desk1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat img_2 = imread("desk2.jpg", CV_LOAD_IMAGE_COLOR);
	//-- ��ʼ��
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	Mat descriptors_1, descriptors_2;
	Ptr<ORB> orb = cv::ORB::create();
	//Ptr<DescriptorExtractor> descriptor = cv::ORB::create();
	// Ptr<FeatureDetector> detector = FeatureDetector::create("ORB");
	 //Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create("ORB");
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
	//-- ��һ��:��� Oriented FAST �ǵ�λ��
	orb->detect(img_1, keypoints_1);
	orb->detect(img_2, keypoints_2);

	//-- �ڶ���:���ݽǵ�λ�ü��� BRIEF ������
	orb->compute(img_1, keypoints_1, descriptors_1);
	orb->compute(img_2, keypoints_2, descriptors_2);

	Mat outimg1;
	Mat outimg2;
	drawKeypoints(img_1, keypoints_1, outimg1);
	drawKeypoints(img_2, keypoints_2, outimg2);
	imwrite("ORB_feature1.jpg", outimg1);
	imwrite("ORB_feature2.jpg", outimg2);


	//-- ������:������ͼ���е�BRIEF�����ӽ���ƥ�䣬ʹ�� Hamming ����
	vector<DMatch> matches;
	//BFMatcher matcher ( NORM_HAMMING );
	matcher->match(descriptors_1, descriptors_2, matches);

	//-- ���Ĳ�:ƥ����ɸѡ
	double min_dist = 10000, max_dist = 0;

	//�ҳ�����ƥ��֮�����С�����������, ���������Ƶĺ�����Ƶ������֮��ľ���
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	// �������ֵ�д��
	min_dist = min_element(matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance < m2.distance; })->distance;
	max_dist = max_element(matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance < m2.distance; })->distance;

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//��������֮��ľ��������������С����ʱ,����Ϊƥ������.����ʱ����С�����ǳ�С,����һ������ֵ30��Ϊ����.
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i].distance <= max(2 * min_dist, 30.0))
		{
			good_matches.push_back(matches[i]);
		}
	}

	//-- ���岽:����ƥ����
	Mat img_match;
	Mat img_goodmatch;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_match);
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches, img_goodmatch);
	imwrite("org_match.jpg", img_match);
	imwrite("good_org_match.jpg", img_goodmatch);
	waitKey(0);

	return 0;
}
