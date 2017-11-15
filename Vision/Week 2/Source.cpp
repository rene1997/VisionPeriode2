#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int runOpdracht2();
int runOpdracht3();
int allContours(Mat, vector<vector<Point>> &);

int clockWiseX[] = { -1,-1,0,1,1, 1, 0,-1,-1 };
int clockWiseY[] = {  0, 1,1,1,0,-1,-1,-1, 0 };
int clockWiseSize = 9;


int main() {
	return runOpdracht2();
	//return runOpdracht3();
}

int allContours(Mat binaryImage, vector<vector<Point>> & contourVecVec) {
	Mat mooreBoundary = binaryImage.clone();

	for (int i = 0;i < binaryImage.cols; i++)
	{
		for (int j = 0; j < binaryImage.rows; j++)
		{
			if (binaryImage.at<uchar>(i, j) == 0 /*&& not in earlier object*/)
			{
				vector<Point> points;
				Point firstPoint;
				firstPoint.x = j;
				firstPoint.y = i;
				points.push_back(firstPoint);
				int x = j, y = i;
				bool edge = false;
				while (((firstPoint.x != points[points.size() -1].x) || (firstPoint.y != points[points.size() -1].y)) || (points.size() <= 1)) {
					int lastY = points[points.size() - 1].y;
					int lastX = points[points.size() - 1].x;
					if (firstPoint.y == lastY) {
						if (firstPoint.x == lastX) {
							cout << "zou gestopt moeten zijn" << endl;
						}
					}

					bool added = false;
					for (int c = 0; c < clockWiseSize; c++) {
						if ((binaryImage.at<uchar>((x + clockWiseX[c]), (y + clockWiseY[c])) != 0))
						{
							edge = true;
						}
						else if (edge && !added)
						{
							Point point;
							x += clockWiseX[c];
							y += clockWiseY[c];
							point.x = x;
							point.y = y;
							points.push_back(point);
							added = true;
						}
					}
				}
				cout << "first object found " << points.size() << endl;
			}
		}
	}
	return -1;
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

	vector<vector<Point>> contourVector;
	allContours(treshold_image, contourVector);
	waitKey(0);
		return 0;
}


int runOpdracht3() {
	return 0;
}
