#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/nonfree/nonfree.hpp"//SIFT���
#include "opencv2/legacy/legacy.hpp"//ƥ�������
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	//1.SIFT��������ȡ����detect()����
	Mat srcImg1 = imread("��1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat srcImg2 = imread("��2.jpg", CV_LOAD_IMAGE_COLOR);
	Mat dstImg1, dstImg2;
	SiftFeatureDetector siftDetector;//SiftFeatureDetector��SIFT��ı���  
	vector<KeyPoint> keyPoints1;
	vector<KeyPoint> keyPoints2;
	siftDetector.detect(srcImg1, keyPoints1);
	siftDetector.detect(srcImg2, keyPoints2);
	drawKeypoints(srcImg1, keyPoints1, dstImg1);
	drawKeypoints(srcImg2, keyPoints2, dstImg2);
	//namedWindow("dst");
	//imwrite("winesift_feature1.jpg", dstImg1);
	//imwrite("winesift_feature2.jpg", dstImg2);
	//2.������������(��������)��ȡ����compute()����
	SiftDescriptorExtractor descriptor;//SiftDescriptorExtractor��SIFT��ı���  
	Mat description1;
	Mat description2;
	descriptor.compute(srcImg1, keyPoints1, description1);//ע��ԭͼ��������Ҫ��Ӧ����Ҫд��
	descriptor.compute(srcImg2, keyPoints2, description2);
	//imshow("description1", description1);
	//imshow("description2", description2);
	//3.ʹ�ñ���ƥ�������б���ƥ�䡪��BruteForceMatcher���match()����
	BruteForceMatcher<L2<float>> matcher;//ʵ��������ƥ����
	vector<DMatch> matches; //����ƥ��������
	matcher.match(description1, description2, matches);//ʵ��������֮���ƥ��
	//4.��ƥ��������ɸѡ������DMatch�ṹ���е�float���ͱ���distance����ɸѡ��
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
	//5.����ƥ��������drawMatches()
	Mat dstImg3;
	drawMatches(srcImg1, keyPoints1, srcImg2, keyPoints2, goodMatches, dstImg3);
	imwrite("��sift_match.jpg", dstImg3);
	waitKey(0);
	return 0;
}
