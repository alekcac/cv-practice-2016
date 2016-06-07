#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <string>

int main()
{
	std::string winNameSrc = "source";
	std::string winNameBlur = "blur";

	cv::namedWindow(winNameSrc, 1);
	cv::namedWindow(winNameBlur, 1);

	cv::Mat source = cv::imread("house.png", 1);
	cv::Mat blur;

	cv::blur(source, blur, cv::Size(5, 5));

	cv::imshow(winNameSrc, source);
	cv::imshow(winNameBlur, blur);

	cv::waitKey();

	return 0;
}

