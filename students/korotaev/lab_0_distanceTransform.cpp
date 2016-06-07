#include "stdafx.h"
#include <opencv2/opencv.hpp>

int main()
{
	cv::Mat image = cv::imread("house.png", 1);
	cv::Mat binaryImage = cv::imread("house.png", 0);
	cv::Mat distance;

	cv::distanceTransform(binaryImage, distance, CV_DIST_L2, 3);
	cv::normalize(distance, distance, 0.0, 1.0, cv::NORM_MINMAX);

	cv::imshow("source image", image);
	cv::imshow("source binary image", binaryImage);
	cv::imshow("distance", distance);

	cv::waitKey();

    return 0;
}

