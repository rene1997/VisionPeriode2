#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int runOpdracht2();
int runOpdracht3();
int allContours(Mat, vector<vector<Point>> &);

int clockWiseX[] = { -1,-1,0,1,1, 1, 0,-1,-1 };
int clockWiseY[] = {  0, -1,-1,-1,0,1,1,1, 0 };
int clockWiseSize = 9;


int main() {
	return runOpdracht2();
	//return runOpdracht3();
}

int allContours(Mat binaryImage, vector<vector<Point>> & contourVecVec) {
	Mat mooreBoundary = binaryImage.clone();
	mooreBoundary = 0;

	
	
	for (int i = 0;i < binaryImage.rows; i++)
	{
		for (int j = 0; j < binaryImage.cols; j++)
		{
			//mooreBoundary.at<uchar>(i, j) = 0;
			if (binaryImage.at<uchar>(i, j) == 0 /*&& not in earlier object*/)
			{
				auto osdif = binaryImage.at<uchar>(i, j);
				cout << "waarom komt hier nisk uit " << binaryImage.at<uchar>(i, j);
				vector<Point> points;
				Point firstPoint;
				firstPoint.x = j;
				firstPoint.y = i;
				points.push_back(firstPoint);
				int x = j, y = i;
				//bool edge = false;
				while (((firstPoint.x != points[points.size() -1].x) || (firstPoint.y != points[points.size() -1].y)) || (points.size() <= 1)) {
					int lastY = points[points.size() - 1].y;
					int lastX = points[points.size() - 1].x;
					if (firstPoint.y == lastY) {
						if (firstPoint.x == lastX) {
							cout << "zou gestopt moeten zijn" << endl;
						}
					}
					//mooreBoundary.at<uchar>(i, j) = 255;
					bool added = false;
					int edge = 0;
					bool found = false;
					for (int c = 0; c < clockWiseSize; c++) {
						int testx = x + clockWiseX[c];
						int testy = y + clockWiseY[c];
						int value = binaryImage.at<uchar>((y + clockWiseY[c]), (x + clockWiseX[c]));
						int valuetest = binaryImage.at<uchar>(20,106);
						if ((binaryImage.at<uchar>((y + clockWiseY[c]), (x + clockWiseX[c])) != 0))
						{
							edge++;
						}
						else if (edge>1 && !added)
						{
							Point point;
							x += clockWiseX[c];
							y += clockWiseY[c];
							point.x = x;
							point.y = y;
							points.push_back(point);
							added = true;
							found = true;
							mooreBoundary.at<uchar>(y, x)=255;
						}
						if (!found && c == clockWiseSize-1) { c = 0; };
					}

					
				}
				imshow("test deze", mooreBoundary);
				return -1;
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

	GaussianBlur(gray_image, treshold_image, Size(7, 7), 0, 0);
	threshold(treshold_image, treshold_image, 50, 255, THRESH_BINARY);

	imshow("Teshold", treshold_image);

	imwrite("test.bmp",treshold_image);

	vector<vector<Point>> contourVector;
	allContours(treshold_image, contourVector);
	waitKey(0);
		return 0;
}


int runOpdracht3() {
	return 0;
}
