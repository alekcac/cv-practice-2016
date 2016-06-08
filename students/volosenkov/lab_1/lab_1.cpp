#include<opencv2\opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
void show(Mat &Img)
{
	cvNamedWindow("Window", CV_WINDOW_AUTOSIZE);
	imshow("Window", Img);
	cvWaitKey();
}
int main()
{
	char* imgName = "road.png";
	Mat src = imread(imgName, 1);
	show(src);
	Mat Rebs;
	cvtColor(src, Rebs, CV_RGB2GRAY);
	Mat Dist, Cann;
	show(Rebs);
	Canny(Rebs, Cann, 110.0, 200.0, 3);
	bitwise_not(Cann, Cann);
	distanceTransform(Cann, Dist, CV_DIST_L2, 3);
	normalize(Dist, Dist, 0.0, 1.0, NORM_MINMAX);
	show(Dist);
	Mat *splImg = new Mat[3], rInt, gInt, bInt;
	split(src, splImg);
	integral(splImg[0], rInt);
	integral(splImg[1], gInt);
	integral(splImg[2], bInt);
	Mat *ResSplImg= new Mat[3];
	ResSplImg[0].create(src.size(), CV_8U);
	ResSplImg[1].create(src.size(), CV_8U);
	ResSplImg[2].create(src.size(), CV_8U);
	double minDist, maxDist;
    minMaxLoc(Dist, &minDist, &maxDist);
  	float ratio = 100 / maxDist;
	for (int i = 0; i < src.rows; i++)
  		for (int j = 0; j < src.cols; j++)
  		{
			int window = (int(Dist.at<float>(i, j) * ratio) % 2 == 0 ? int(Dist.at<float>(i, j) * ratio) - 1 : int(Dist.at<float>(i, j) * ratio)) / 2;
 			int jLeft = j - window < 0 ? 1 : j - window + 1;
			int jRight = j + window >= src.cols ? src.cols : j + window + 1;
 			int iTop = i - window < 0 ? 1 : i - window + 1;
			int iDown = i + window >= src.rows ? src.rows : i + window + 1;
 
 			if (window > 1)
			{
				ResSplImg[0].at<uchar>(i, j) = uchar((rInt.at<int>(iDown, jRight) - rInt.at<int>(iDown, jLeft) - rInt.at<int>(iTop, jRight) + rInt.at<int>(iTop, jLeft)) / (window * window * 4));
				ResSplImg[1].at<uchar>(i, j) = uchar((gInt.at<int>(iDown, jRight) - gInt.at<int>(iDown, jLeft) - gInt.at<int>(iTop, jRight) + gInt.at<int>(iTop, jLeft)) / (window * window * 4));
 				ResSplImg[2].at<uchar>(i, j) = uchar((bInt.at<int>(iDown, jRight) - bInt.at<int>(iDown, jLeft) - bInt.at<int>(iTop, jRight) + bInt.at<int>(iTop, jLeft)) / (window * window * 4));
			}
 			else
			{
				ResSplImg[0].at<uchar>(i, j) = splImg[0].at<uchar>(i, j);
				ResSplImg[1].at<uchar>(i, j) = splImg[1].at<uchar>(i, j);
				ResSplImg[2].at<uchar>(i, j) = splImg[2].at<uchar>(i, j);
			}
		}
		Mat Res;
		merge(ResSplImg, 3, Res);
		show(Res);
}