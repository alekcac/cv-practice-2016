#include <iostream>
#include <fstream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

void Smoothing(int, void* params)
{
	int threshold = ((int *)params)[0];
	int kernel_divider = ((int *)params)[1];
	Mat src = imread("D:\\Study\\computer_vision\\cv-practice-2016\\labs\\lab1\\road.png", CV_LOAD_IMAGE_COLOR), \
		contours, distances, sum, convolutional(src.size(), CV_8UC3);

	Canny(src, contours, threshold, threshold * 3);
	contours = 255 - contours;

	distanceTransform(contours, distances, CV_DIST_L2, CV_DIST_MASK_PRECISE);

	integral(src, sum, CV_8UC1);

	for (int i = 0; i < src.size().height; i++)
	{
		for (int j = 0; j < src.size().width; j++)
		{
			int coefficient = kernel_divider ? kernel_divider : 1;
			int half_of_kernel_size = *distances.ptr<float>(i, j) / coefficient, y_l, y_u, x_l, x_r;
			y_l = (i + 1) + half_of_kernel_size >= sum.size().height ? sum.size().height - 1: (i + 1) + half_of_kernel_size;
			x_r = (j + 1) + half_of_kernel_size >= sum.size().width ? sum.size().width - 1: (j + 1) + half_of_kernel_size;
			y_u = i - half_of_kernel_size < 0 ? 0 : i - half_of_kernel_size;
			x_l = j - half_of_kernel_size < 0 ? 0 : j - half_of_kernel_size;
			int divider = (x_r - x_l) * (y_l - y_u);
			convolutional.ptr(i, j)[0] = (sum.ptr<unsigned>(y_l, x_r)[0] + sum.ptr<unsigned>(y_u, x_l)[0] - sum.ptr<unsigned>(y_u, x_r)[0] - sum.ptr<unsigned>(y_l, x_l)[0]) / (divider);
			convolutional.ptr(i, j)[1] = (sum.ptr<unsigned>(y_l, x_r)[1] + sum.ptr<unsigned>(y_u, x_l)[1] - sum.ptr<unsigned>(y_u, x_r)[1] - sum.ptr<unsigned>(y_l, x_l)[1]) / (divider); 
			convolutional.ptr(i, j)[2] = (sum.ptr<unsigned>(y_l, x_r)[2] + sum.ptr<unsigned>(y_u, x_l)[2] - sum.ptr<unsigned>(y_u, x_r)[2] - sum.ptr<unsigned>(y_l, x_l)[2]) / (divider); 
		}
	}

	imshow("Smoothing", convolutional);
}

int main()
{
	namedWindow("Smoothing");
	int params[] = {0, 0};
	createTrackbar( "Min Threshold:", "Smoothing", &params[0], 100, Smoothing, params);
	createTrackbar( "Min Coefficient:", "Smoothing", &params[1], 20, Smoothing, params);
	Smoothing(0, &threshold);
	waitKey(0);
	return 0;
}