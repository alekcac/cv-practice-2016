#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		cout << "Usage: cv_practice.exe <path_to_image> <procedure>\n\n\
		<path_to_image> - Path to any image on your local machine\n\
		<procedure>     - Any of the following:\n\n\
				  blur  - Blurs an image using the normalized\n\
					box filter.\n\
				  canny - Finds edges in an image.\n\
				  lines - Finds lines in a binary image using \n\
					the standard Hough transform.\n";
		cin.get();
		return 0;
	}

	Mat src = imread(argv[1], CV_LOAD_IMAGE_COLOR), dst;

	string method(argv[2]);
	
	if (method == "blur")
	{	
		blur(src, dst, src.size());
	}
	else if (method == "canny")
	{
		Canny(src, dst, 50, 200);
	}
	else if (method == "lines")
	{
		Mat src_edges;

		Canny(src, src_edges, 50, 200);

		vector<Vec2f> lines;
		HoughLines(src_edges, lines, 1, CV_PI/180, 150, 0, 0 );
		src.copyTo(dst);
		for (int i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(dst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
		}
	}

	imshow("Picture", src);
	imshow("Result", dst);
	waitKey(0);
	return 0;
}