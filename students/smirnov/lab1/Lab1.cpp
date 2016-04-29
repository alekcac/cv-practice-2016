#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	char* imageName = (argc == 2) ? argv[1] : NULL;
	if (imageName == NULL) return 0;

	Mat srcImage = imread(imageName, 1), dstImage, distanceImage, integralImage;

	cvNamedWindow("Result", CV_WINDOW_AUTOSIZE);

	imshow("Result", srcImage);

	cvWaitKey();

	cvtColor(srcImage, dstImage, CV_RGB2GRAY);

	Canny(dstImage, dstImage, 35.0, 185.0, 3);

	bitwise_not(dstImage, dstImage);

	distanceTransform(dstImage, distanceImage, CV_DIST_L2, 3);
	normalize(distanceImage, distanceImage, 0.0, 1.0, NORM_MINMAX);

	Mat *splittedImage = new Mat[3], rIntegral, gIntegral, bIntegral;
	
	split(srcImage, splittedImage);

	integral(splittedImage[0], rIntegral);
	integral(splittedImage[1], gIntegral);
	integral(splittedImage[2], bIntegral);

	Mat *splittedResultImage = new Mat[3];

	splittedResultImage[0].create(srcImage.size(), CV_8U);
	splittedResultImage[1].create(srcImage.size(), CV_8U);
	splittedResultImage[2].create(srcImage.size(), CV_8U);

	double minDist, maxDist;
	minMaxLoc(distanceImage, &minDist, &maxDist);
	float ratio = 100 / maxDist;

	for (int i = 0; i < srcImage.rows; i++)
		for (int j = 0; j < srcImage.cols; j++)
		{
			int window = (int(distanceImage.at<float>(i, j) * ratio) % 2 == 0 ? int(distanceImage.at<float>(i, j) * ratio) - 1 : int(distanceImage.at<float>(i, j) * ratio)) / 2;
			int jLeft = j - window < 0 ? 1 : j - window + 1, jRight = j + window >= srcImage.cols ? srcImage.cols : j + window + 1;
			int iTop = i - window < 0 ? 1 : i - window + 1, iDown = i + window >= srcImage.rows ? srcImage.rows : i + window + 1;

			if (window > 1)
			{
				splittedResultImage[0].at<uchar>(i, j) = uchar((rIntegral.at<int>(iDown, jRight) - rIntegral.at<int>(iDown, jLeft) - rIntegral.at<int>(iTop, jRight) + rIntegral.at<int>(iTop, jLeft)) / (window * window * 4));
				splittedResultImage[1].at<uchar>(i, j) = uchar((gIntegral.at<int>(iDown, jRight) - gIntegral.at<int>(iDown, jLeft) - gIntegral.at<int>(iTop, jRight) + gIntegral.at<int>(iTop, jLeft)) / (window * window * 4));
				splittedResultImage[2].at<uchar>(i, j) = uchar((bIntegral.at<int>(iDown, jRight) - bIntegral.at<int>(iDown, jLeft) - bIntegral.at<int>(iTop, jRight) + bIntegral.at<int>(iTop, jLeft)) / (window * window * 4));
			}
			else
			{
				splittedResultImage[0].at<uchar>(i, j) = splittedImage[0].at<uchar>(i, j);
				splittedResultImage[1].at<uchar>(i, j) = splittedImage[1].at<uchar>(i, j);
				splittedResultImage[2].at<uchar>(i, j) = splittedImage[2].at<uchar>(i, j);
			}
		}

	Mat resultImage;
	merge(splittedResultImage, 3, resultImage);

	imshow("Result", resultImage);

	cvWaitKey();

	return 0;
}