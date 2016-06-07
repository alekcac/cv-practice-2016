#include "stdafx.h"
#include <opencv2\opencv.hpp>
using namespace cv;

int main()
{
	Mat source = imread("road.png", 1), canny, distance, *splitSource = new Mat[3], intR, intG, intB, *result = new Mat[3], global;
	imshow("source", source);

	Canny(source, canny, 50, 200, 3);
	imshow("canny", canny);

	bitwise_not(canny, canny);
	distanceTransform(canny, distance, CV_DIST_L1, 3);
	imshow("distance", distance);

	split(source, splitSource);

	integral(splitSource[0], intR, CV_8UC1);
	integral(splitSource[1], intG, CV_8UC1);
	integral(splitSource[2], intB, CV_8UC1);

	result[0].create(source.size(), CV_8UC1);
	result[1].create(source.size(), CV_8UC1);
	result[2].create(source.size(), CV_8UC1);

	float Dij = 0.0f;
	int bot, right, top, left;
	int r, g, b;
	int k = 2;

	for (int i = 1; i < source.rows; i++)
		for (int j = 1; j < source.cols; j++)
		{
			Dij = distance.at<float>(i - 1, j - 1);

			if (Dij > 0)
			{
				bot = ( (i + Dij / k ) >= source.rows ) ? source.rows : (i + Dij / k);
				right = ( (j + Dij / k) >= source.cols ) ? source.cols : (j + Dij / k);
				top = ( (i - Dij / k) <= 0 ) ? 0 : (i - Dij / k);
				left = ( (j - Dij / k) <= 0 ) ? 0 : (j - Dij / k);

				r = intR.at<int>(bot, right) + intR.at<int>(top, left) - intR.at<int>(bot, left) - intR.at<int>(top, right);
				g = intG.at<int>(bot, right) + intG.at<int>(top, left) - intG.at<int>(bot, left) - intG.at<int>(top, right);
				b = intB.at<int>(bot, right) + intB.at<int>(top, left) - intB.at<int>(bot, left) - intB.at<int>(top, right);

				result[0].at<uchar>(i, j) = r / ((bot - top)*(right - left));
				result[1].at<uchar>(i, j) = g / ((bot - top)*(right - left));
				result[2].at<uchar>(i, j) = b / ((bot - top)*(right - left));
			}
			else
			{
				result[0].at<uchar>(i, j) = splitSource[0].at<uchar>(i, j);
				result[1].at<uchar>(i, j) = splitSource[1].at<uchar>(i, j);
				result[2].at<uchar>(i, j) = splitSource[2].at<uchar>(i, j);
			}
		}

	merge(result, 3, global);
	imshow("result", global);

	waitKey();

    return 0;
}

