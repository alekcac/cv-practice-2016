#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

void Smoothing(int parameter, void* params)
{
	int threshold = ((int **)params)[0][0], kernel_divider = ((int **)params)[0][1];
	char *path_to_image = ((char ***)params)[1][0],\
		*dist_window_name = ((char ***)params)[1][1], *smooth_window_name = ((char ***)params)[1][2];

	Mat src = imread(path_to_image, CV_LOAD_IMAGE_COLOR), \
		contours, distances, sum, convolutional(src.size(), CV_8UC3);

	Canny(src, contours, threshold, threshold * 3);
	contours = 255 - contours;

	distanceTransform(contours, distances, CV_DIST_L2, CV_DIST_MASK_PRECISE);
	imshow(dist_window_name, distances);

	integral(src, sum, CV_8UC1);

	for (int i = 0; i < src.size().height; i++)
	{
		for (int j = 0; j < src.size().width; j++)
		{
			int coefficient = kernel_divider ? kernel_divider : 1;
			int half_of_kernel_size = (int)*distances.ptr<float>(i, j) / coefficient, y_l, y_u, x_l, x_r;
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

	imshow(smooth_window_name, convolutional);
}

int main(int argc, char** argv)
{
	if (argc >= 2)
	{
		char* dist_window_name = "Distance";
		char* smooth_window_name = "Smoothing";
		namedWindow(dist_window_name);
		namedWindow(smooth_window_name);

		int int_params[] = {0, 0};
		char* string_params[] = {argv[1], dist_window_name, smooth_window_name};
		void* params[] = {int_params, string_params};

		createTrackbar("Threshold:", dist_window_name, &int_params[0], 100, Smoothing, params);
		createTrackbar("Kernel divider:", smooth_window_name, &int_params[1], 20, Smoothing, params);

		Smoothing(0, params);
		waitKey(0);
	}
	else 
	{
		cout << "Please, point path to image as program argument." << endl;
		cin.get();
	}

	return 0;
}