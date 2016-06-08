#include "opencv2\opencv.hpp"
using namespace cv;

int main(int argc, char* argv[])
{

	char* ImageName = (argc == 2) ? argv[1] : NULL;
	if (ImageName == NULL) return 0;

	Mat SourceImage = imread(ImageName, 1), dstImage;

	cvNamedWindow("OutputWindow", CV_WINDOW_AUTOSIZE);
	imshow("OutputWindow", SourceImage);
	cvWaitKey();
	
	equalizeHist(SourceImage, dstImage);
	imshow("OutputWindow", dstImage);
	cvWaitKey();

	integral(SourceImage, dstImage, -1);
	imshow("OutputWindow", dstImage);
	cvWaitKey();

	Canny(SourceImage, dstImage, 100.0, 300.0);
	imshow("OutputWindow", dstImage);
	cvWaitKey();
return 0;
}