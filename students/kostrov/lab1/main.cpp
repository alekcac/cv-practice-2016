#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
int main()
{
	Mat src = imread("road.png", 1);
	imshow("src", src);

	Mat dist;
	Mat canny;
	Canny(src, canny, 190, 90, 3);
	imshow("canny", canny);

	distanceTransform(~canny, dist, CV_DIST_L1, 3);
	imshow("dist", dist / 50);

	vector <Mat> ch(3);
	split(src, ch);
	Mat integralR;
	Mat integralG;
	Mat integralB;

	integral(ch[0], integralR, CV_32S);
	integral(ch[1], integralG, CV_32S);
	integral(ch[2], integralB, CV_32S);
	float d = 3;
	Mat res;
	src.copyTo(res);


	for (int i = 1; i < src.rows + 1; i++)
		for (int j = 1; j < src.cols + 1; j++)
		{
			float sumR = 0;
			float sumG = 0;
			float sumB = 0;
			d = dist.at<float>(i - 1, j - 1);

			if (d != 0)
			{
				int ind1 = i + d / 2;
				int ind2 = j + d / 2;
				int ind3 = i - d / 2;
				int ind4 = j - d / 2;
				int tmp = 0;

				if (ind1 + 1 > src.rows - 1)
					ind1 = src.rows - 1;
				if (ind2 + 1 > src.cols - 1)
					ind2 = src.cols - 1;
				if (ind3 - 1 < 0)
					ind3 = 0;
				if (ind4 - 1 < 0)
					ind4 = 0;


				sumR = integralR.at<int>(ind1, ind2) - integralR.at<int>(ind1, ind4) - integralR.at<int>(ind3, ind2) + integralR.at<int>(ind3, ind4);
				sumG = integralG.at<int>(ind1, ind2) - integralG.at<int>(ind1, ind4) - integralG.at<int>(ind3, ind2) + integralG.at<int>(ind3, ind4);
				sumB = integralB.at<int>(ind1, ind2) - integralB.at<int>(ind1, ind4) - integralB.at<int>(ind3, ind2) + integralB.at<int>(ind3, ind4);

				res.at<Vec3b>(i - 1, j - 1) = Vec3b(sumR / ((ind1 - ind3)*(ind2 - ind4)), sumG / ((ind1 - ind3)*(ind2 - ind4)), sumB / ((ind1 - ind3)*(ind2 - ind4)));
			}
			else
				res.at<Vec3b>(i - 1, j - 1) = src.at<Vec3b>(i - 1, j - 1);
		}

	imshow("out", res);

	cv::waitKey();
	return 0;

}

