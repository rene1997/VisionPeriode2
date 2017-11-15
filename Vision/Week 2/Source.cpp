#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int runOpdracht2();
int allContours(Mat image, vector < vector<Point>> & countourVecVec);

int main() {
	return runOpdracht2();
}


int runOpdracht2() {
	Mat image = imread("monsters.jpg", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	imshow("Original image", image);

	Mat dst, cdst, gray_image, treshold_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	imshow("gray image", gray_image);

	threshold(gray_image, treshold_image, 50, 255, THRESH_BINARY);

	imshow("Teshold", treshold_image);

	Mat mooreBoundary = treshold_image.clone();

	//imwrite("mooreBoundary.bmp", mooreBoundary);

	int x, p, y;
	bool edge = false;
	for (int j = 0; j<treshold_image.cols; j++)
	{
		for (int i = 0; i < treshold_image.rows; i++)
		{

			if (treshold_image.at<uchar>(i, j) == 0)
			{
				for (int p = -1; p<2; p++)
				{
					if ((treshold_image.at<uchar>((i + p), (j + p)) != 0))
					{
						edge = true;
					}
				}
				if (edge == true)
				{
					mooreBoundary.at<uchar>(i, j) = 255;
				}
				else
				{
					mooreBoundary.at<uchar>(i, j) = 0;
				}
				edge = false;
			}
			else
			{
				mooreBoundary.at<uchar>(i, j) = 0;
			}

		}
	}

	imshow("moore Boundary", mooreBoundary);
	
	waitKey(0);
		return 0;
}

int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec) {
	Mat greyImage, treshold;
	cvtColor(binaryImage, greyImage, CV_BGR2GRAY);

	return 0;
}
