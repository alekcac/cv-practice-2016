#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

template <int thread>
char*& dist_window_name()
{
	static char* window_name;
	return window_name;
}

template <int thread>
char*& smooth_window_name()
{
	static char* window_name;
	return window_name;
}

template <int thread>
char*& path_to_image()
{
	static char* path;
	return path;
}

template<int thread>
void Smoothing(int parameter, void* params)
{

	int threshold = ((int *)params)[0];
	int kernel_divider = ((int *)params)[1];
	Mat src = imread(path_to_image<thread>(), CV_LOAD_IMAGE_COLOR), \
		contours, distances, sum, convolutional(src.size(), CV_8UC3);

	Canny(src, contours, threshold, threshold * 3);
	contours = 255 - contours;

	distanceTransform(contours, distances, CV_DIST_L2, CV_DIST_MASK_PRECISE);
	imshow(dist_window_name<thread>(), distances);

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

	imshow(smooth_window_name<thread>(), convolutional);
}

int main(int argc, char** argv)
{
	if (argc >= 2)
	{
		const int thread = 0;
		path_to_image<thread>() = argv[1];
		dist_window_name<thread>() = "Distances";
		smooth_window_name<thread>() = "Smoothing";

		namedWindow(dist_window_name<thread>());
		namedWindow(smooth_window_name<thread>());

		int params[] = {0, 0};
		createTrackbar( "Threshold:", dist_window_name<thread>(), &params[0], 100, Smoothing<thread>, params);
		createTrackbar( "Kernel divider:", smooth_window_name<thread>(), &params[1], 20, Smoothing<thread>, params);

		Smoothing<thread>(0, params);
		waitKey(0);
	}
	else 
	{
		cout << "Please, point path to image as program argument." << endl;
		cin.get();
	}

	return 0;
}