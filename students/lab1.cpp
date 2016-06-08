#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;

using namespace cv;

bool main(int argc, char* argv[])
{
	//If argc != 2 => exit (false)
	char *img_name = (argc == 2) ? argv[1] : NULL;
	if (img_name == NULL) return false;
	Mat img_src = imread(img_name, 1), img_dstn, img_dist, img_int;

	//Base image
	cvNamedWindow("Simple out", CV_WINDOW_AUTOSIZE);
	imshow("Result", img_src);
	cvWaitKey();
	
	cvtColor(img_src, img_dstn, CV_RGB2GRAY);
	//Canny-method
	Canny(img_dstn, img_dstn, 35.0, 185.0, 3);

	bitwise_not(img_dstn, img_dstn);
	distanceTransform(img_dstn, img_dist, CV_DIST_L2, 3);
	normalize(img_dist, img_dist, 0.0, 1.0, NORM_MINMAX);

	//Image splitting
	Mat *img_splt = new Mat[3], int_r, int_g, int_b;
	split(img_src, img_splt);
	int img_size = img_src.size();
	integral(img_splt[0], int_r); integral(img_splt[1], int_g); integral(img_splt[2], int_b);
	Mat *img_splt_res = new Mat[3];
	img_splt_res[0].create(img_size, CV_8U); img_splt_res[1].create(img_size, CV_8U); img_splt_res[2].create(img_size, CV_8U);

	double dst_min, dist_max;
	minMaxLoc(img_dist, &dist_min, &dist_max);
	float val = 100 / dist_max;

	int img_rows = img_src.rows;
	int img_cols = img_src.cols
	for (int i = 0; i < img_rows; i++)
		for (int j = 0; j < img_cols; j++){
			int window = (int(img_dist.at<float>(i, j) * val) % 2 == 0 ? int(img_dist.at<float>(i, j) * val) - 1 : int(img_dist.at<float>(i, j) * val)) / 2; int left_j = j - window < 0 ? 1 : j - window + 1, right_j = j + window >= img_cols ? img_cols : j + window + 1; int top_i = i - window < 0 ? 1 : i - window + 1, bot_i = i + window >= img_rows ? img_rows : i + window + 1;
			if (window > 1){
				img_splt_res[0].at<uchar>(i, j) = uchar((int_r.at<int>(bot_i, right_j) - int_r.at<int>(bot_i, left_j) - int_r.at<int>(top_i, right_j) + int_r.at<int>(top_i, left_j)) / (window * window * 4)); img_splt_res[1].at<uchar>(i, j) = uchar((int_g.at<int>(bot_i, right_j) - int_g.at<int>(bot_i, left_j) - int_g.at<int>(top_i, right_j) + int_g.at<int>(top_i, left_j)) / (window * window * 4)); img_splt_res[2].at<uchar>(i, j) = uchar((int_b.at<int>(bot_i, right_j) - int_b.at<int>(bot_i, left_j) - int_b.at<int>(top_i, right_j) + int_b.at<int>(top_i, left_j)) / (window * window * 4));
			}
			else{
				img_splt_res[0].at<uchar>(i, j) = img_splt[0].at<uchar>(i, j); img_splt_res[1].at<uchar>(i, j) = img_splt[1].at<uchar>(i, j); img_splt_res[2].at<uchar>(i, j) = img_splt[2].at<uchar>(i, j);
			}
		}

	//Result output
	Mat img_res;
	merge(img_splt_res, 3, img_res);
	imshow("Hard out", img_res);
	cvWaitKey();

	return true;
} 