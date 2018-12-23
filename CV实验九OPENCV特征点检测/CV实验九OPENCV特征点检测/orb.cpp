#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <iostream>  

using namespace cv;
using namespace std;


int main()
{

	Mat image01 = imread("ipad1.jpg", 1);
	Mat image02 = imread("ipad2.jpg", 1);
	//imshow("p2", image01);
	//imshow("p1", image02);

	//�Ҷ�ͼת��  
	Mat image1, image2;
	cvtColor(image01, image1, CV_RGB2GRAY);
	cvtColor(image02, image2, CV_RGB2GRAY);


	//��ȡ������    
	OrbFeatureDetector OrbDetector(1000);  // ������������ȣ�ֵԽС��Խ�٣�Խ��׼ 
	vector<KeyPoint> keyPoint1, keyPoint2;
	OrbDetector.detect(image1, keyPoint1);
	OrbDetector.detect(image2, keyPoint2);

	//������������Ϊ�±ߵ�������ƥ����׼��    
	OrbDescriptorExtractor OrbDescriptor;
	Mat imageDesc1, imageDesc2;
	OrbDescriptor.compute(image1, keyPoint1, imageDesc1);
	OrbDescriptor.compute(image2, keyPoint2, imageDesc2);

	flann::Index flannIndex(imageDesc1, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);

	vector<DMatch> GoodMatchePoints;

	Mat macthIndex(imageDesc2.rows, 2, CV_32SC1), matchDistance(imageDesc2.rows, 2, CV_32FC1);
	flannIndex.knnSearch(imageDesc2, macthIndex, matchDistance, 2, flann::SearchParams());

	// Lowe's algorithm,��ȡ����ƥ���
	for (int i = 0; i < matchDistance.rows; i++)
	{
		if (matchDistance.at<float>(i, 0) < 0.6 * matchDistance.at<float>(i, 1))
		{
			DMatch dmatches(i, macthIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
			GoodMatchePoints.push_back(dmatches);
		}
	}

	Mat first_match;
	drawMatches(image02, keyPoint2, image01, keyPoint1, GoodMatchePoints, first_match);
	//imshow("first_match ", first_match);
	imwrite("ipadfirst_match.jpg", first_match);
	waitKey();
	return 0;
}