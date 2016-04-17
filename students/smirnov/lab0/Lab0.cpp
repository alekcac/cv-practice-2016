#include "opencv2\opencv.hpp"

using namespace cv;

int main(int argc, char* argv[])
{
	char* imageName = (argc == 2) ? argv[1] : NULL;
	if (imageName == NULL) return 0;

	Mat srcImage = imread(imageName, 1), dstImage;

	cvNamedWindow("Result", CV_WINDOW_AUTOSIZE);

	imshow("Result", srcImage);

	cvWaitKey();

	blur(srcImage, dstImage, Size(5, 5));

	imshow("Result", dstImage);

	cvWaitKey();

	Canny(srcImage, dstImage, 100.0, 300.0);

	imshow("Result", dstImage);

	cvWaitKey();

	morphologyEx(srcImage, dstImage, 1, getStructuringElement(0, Size(3, 3), Point(1, 1)));

	imshow("Result", dstImage);

	cvWaitKey();

	return 0;
}