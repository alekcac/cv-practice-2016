
#include <opencv2\opencv.hpp>

using namespace cv;
int main()
{
	//task1
	//cv::Mat img = cv::Mat::zeros(300, 300, CV_8UC3);
	//cv::putText(img, "Hello, OpenCV!", cv::Point(10, 50),
	//	cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
	//cv::imshow("img", img);


	//task2
	/*Mat imageIn = imread("road.png", 1);
	Mat imageOut;
	cvtColor(imageIn,  imageOut, COLOR_BGR2GRAY, 0);
	imshow("imgin", imageIn);
	imshow("imgout", imageOut);*/


	//task3
	Mat src = imread("house.png", 1);
	Mat dst;
	blur(src, dst, Size(5,5), Point(1,-1), BORDER_DEFAULT);
	imshow("imgin", src);
	imshow("imgout", dst);
	cv::waitKey();

	//task4
    src = imread("lena.jpg", 1);
	Canny(src, dst, 100, 50,3);
	imshow("imgin", src);
	imshow("imgout", dst);
	cv::waitKey();

	//task5
	src = imread("human.png", 0);
	distanceTransform(~src, dst, CV_DIST_L1, 3);
	imshow("imgin", src);
	imshow("imgout", dst/100);

	cv::waitKey();
	return 0;

}

