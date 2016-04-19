#include "stdafx.h"
#include <opencv2\opencv.hpp>

int main()
{
	cv::Mat image = cv::imread("house.png", 1), grayImage, cannyImage;

	cv::cvtColor(image, grayImage, CV_BGR2GRAY);

	cv::Canny(grayImage, cannyImage, 50, 200, 3);

	cv::namedWindow("source", 1);
	cv::namedWindow("canny", 1);

	cv::imshow("source", image);
	cv::imshow("canny", cannyImage);

	cv::waitKey();
	return 0;
}

