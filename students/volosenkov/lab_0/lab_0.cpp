#include<opencv2\opencv.hpp>
using namespace cv;


int main(int argc, char* argv[])
{
	char* imgName = (argc == 2) ? argv[1] : NULL;
	if(imgName == NULL) return 0;
	Mat Image = imread(imgName, 1);
	cvNamedWindow("Window1", CV_WINDOW_AUTOSIZE);
	imshow("Window1", Image);
	cvWaitKey();
	Mat dst1;
	cvtColor(Image, dst1, COLOR_BGR2GRAY, 2);
	cvNamedWindow("Window2", CV_WINDOW_AUTOSIZE);
	imshow("Window2", dst1);
	cvWaitKey();
	Mat dst2;
	equalizeHist(dst1, dst2);
	cvNamedWindow("Window3", CV_WINDOW_AUTOSIZE);
	imshow("Window3", dst2);
	cvWaitKey();
	Mat dst3;
	integral(dst2, dst3, -1);
	cvNamedWindow("Window4", CV_WINDOW_AUTOSIZE);
	imshow("Window4", dst3);
	cvWaitKey();
}