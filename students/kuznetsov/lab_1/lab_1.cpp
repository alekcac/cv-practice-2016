#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
 {
	 char* imgName;
	if ((argc == 2))
	    {
			imgName = argv[1];
	    }
		else 
		{
			imgName = NULL;
		}
			
	if (imgName == NULL) return 0;
	
		Mat sourceImg = imread(imgName, 1), distImg, distanceImg, integralImage;
	
		cvNamedWindow("ResultImg", CV_WINDOW_AUTOSIZE);
	
		imshow("ResultImg", sourceImg);
	
		cvWaitKey();
	
		cvtColor(sourceImg, distImg, CV_RGB2GRAY);
	
		Canny(distImg, distImg, 35.0, 185.0, 3);
	
		bitwise_not(distImg, distImg);
	
		distanceTransform(distImg, distanceImg, CV_DIST_L2, 3);
		normalize(distanceImg, distanceImg, 0.0, 1.0, NORM_MINMAX);
	
		Mat *splitImg = new Mat[3], IntegralR, IntegralG, IntegralB;
	
		split(sourceImg, splitImg);
	
		integral(splitImg[0], IntegralR);
		integral(splitImg[1], IntegralG);
		integral(splitImg[2], IntegralB);
	
		Mat *splitResImg = new Mat[3];
	
		splitResImg[0].create(sourceImg.size(), CV_8U);
		splitResImg[1].create(sourceImg.size(), CV_8U);
		splitResImg[2].create(sourceImg.size(), CV_8U);
	
		double minimalDist, maximalDist;
		minMaxLoc(distanceImg, &minimalDist, &maximalDist);
		float ratio = 100 / maximalDist;
	
		for (int i = 0; i < sourceImg.rows; i++)
		for (int j = 0; j < sourceImg.cols; j++)
		 {
		int win = (int(distanceImg.at<float>(i, j) * ratio) % 2 == 0 ? int(distanceImg.at<float>(i, j) * ratio) - 1 : int(distanceImg.at<float>(i, j) * ratio)) / 2;
		int jLeft = j - win < 0 ? 1 : j - win + 1, jRight = j + win >= sourceImg.cols ? sourceImg.cols : j + win + 1;
		int iTop = i - win < 0 ? 1 : i - win + 1, iDown = i + win >= sourceImg.rows ? sourceImg.rows : i + win + 1;
		
		if (win > 1)
			{
				splitResImg[0].at<uchar>(i, j) = uchar((IntegralR.at<int>(iDown, jRight) - IntegralR.at<int>(iDown, jLeft) - IntegralR.at<int>(iTop, jRight) + IntegralR.at<int>(iTop, jLeft)) / (win * win * 4));
				splitResImg[1].at<uchar>(i, j) = uchar((IntegralG.at<int>(iDown, jRight) - IntegralG.at<int>(iDown, jLeft) - IntegralG.at<int>(iTop, jRight) + IntegralG.at<int>(iTop, jLeft)) / (win * win * 4));
				splitResImg[2].at<uchar>(i, j) = uchar((IntegralB.at<int>(iDown, jRight) - IntegralB.at<int>(iDown, jLeft) - IntegralB.at<int>(iTop, jRight) + IntegralB.at<int>(iTop, jLeft)) / (win * win * 4));
			}
		else
			{
				splitResImg[0].at<uchar>(i, j) = splitImg[0].at<uchar>(i, j);
				splitResImg[1].at<uchar>(i, j) = splitImg[1].at<uchar>(i, j);
				splitResImg[2].at<uchar>(i, j) = splitImg[2].at<uchar>(i, j);
			}
		}
	
		Mat resultImg;
	merge(splitResImg, 3, resultImg);
	
		imshow("ResultImg", resultImg);
	
		cvWaitKey();
	
		return 0;
	}