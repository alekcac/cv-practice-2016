#include "opencv2\opencv.hpp"

using namespace cv;

bool main(int argc, char* argv[])
{
	char* img_name = (argc == 2) ? argv[1] : NULL;
	if (img_name == NULL) return false;
	Mat img_src = imread(img_name, 1), img_dst;

	//1st filter
	morphologyEx(img_src, img_dst, 1, getStructuringElement(0, Size(3, 3), Point(1, 1)));
	imshow("Morphology out", img_dst);
	cvWaitKey();

	//2nd filter
	blur(img_src, img_dst, Size(4, 4));
	imshow("Blur out", img_dst);
	cvWaitKey();

	//3d filter
	Canny(img_src, img_dst, 100.0, 300.0);
	imshow("Canny out", img_dst);
	cvWaitKey();

	return true;
} 