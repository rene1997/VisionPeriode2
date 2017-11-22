#include <opencv2\opencv.hpp>
#include <iostream>
#include "avansvisionlib.h"

using namespace cv;
using namespace std;

int runOpdracht2();
int runOpdracht3();
int allContours(Mat, vector<vector<Point>> &);

int clockWiseX[] = { -1,-1,0,1,1, 1, 0,-1,-1 };
int clockWiseY[] = {  0, -1,-1,-1,0,1,1,1, 0 };
int clockWiseSize = 9;


int main() {
	//return runOpdracht2();
	return runOpdracht3();
}

int allContours(Mat binaryImage, vector<vector<Point>> & contourVecVec) {
	Mat mat16s, labeled;
	vector<Point2d*> startPoints, posVec;
	vector<int> areaVec;

	binaryImage.convertTo(mat16s, CV_16S);
	int blob2Amount = labelBLOBsInfo(mat16s, labeled, startPoints, posVec, areaVec);
	Mat mooreBoundary = binaryImage.clone();
	mooreBoundary = 0;

	for (int i = 0;i < startPoints.size(); i++)
	{
		int x = startPoints[i]->y, y = startPoints[i]->x;
		vector<Point> points;
		Point firstPoint = { x,y };
		points.push_back(firstPoint);
		
		while (((firstPoint.x != points[points.size() -1].x) || (firstPoint.y != points[points.size() -1].y)) || (points.size() <= 1)) {
			bool added = false;
			int edge = 0;
			for (int c = 0; c < clockWiseSize; c++) {				
				if ((binaryImage.at<uchar>((y + clockWiseY[c]), (x + clockWiseX[c])) == 0))
				{
					edge++;
				}
				else if (edge>1 && !added)
				{
					x += clockWiseX[c];
					y += clockWiseY[c];
					Point point = {x,y};
					points.push_back(point);
					added = true;
					mooreBoundary.at<uchar>(y, x)=255;
				}
				if (!added && c == clockWiseSize-1) { c = 0; };
			}
		}
		imshow("test", mooreBoundary);
		cout << "first object found " << points.size() << endl;
		contourVecVec.push_back(points);
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

	Mat dst, cdst, gray_image, mat16s, treshold_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	
	GaussianBlur(gray_image, treshold_image, Size(7, 7), 0, 0);
	threshold(treshold_image, treshold_image, 50, 1, THRESH_BINARY_INV);

	vector<vector<Point>> contourVector;
	allContours(treshold_image, contourVector);
	waitKey(0);
		return 0;
}

void makeGrid(vector<Point> & contour, vector<Point> & newContour, int scale, Mat & image) {
	int x = 0;
	int y = 0;
	Mat testImage = image.clone();
	testImage = 0;
	for (int i = 0; i < contour.size(); i++) {
		x += contour[i].x;
		y += contour[i].y;
		if (i == contour.size() - 1 && contour.size() % scale > scale / 2) {
			scale = contour.size() % scale;
		}
		if ((i+1) % scale == 0 || (i == contour.size() -1 && contour.size() % scale > scale /2)) {
			
			int avgX = abs(x / scale);
			int avgY = abs(y / scale);
			newContour.push_back({ avgX,avgY });
			testImage.at<uchar>(avgY, avgX) = 255;
			x = y = 0;
		}
	}
	imshow("tesplaatje", testImage);
	return;
}

double bendingEnergy(Mat binaryImage, vector<Point> & contourvec) {
	double energy = 0;
	int direction = 0;
	for (int i = 0; i < contourvec.size(); i++) {
		Point current = contourvec[i];
		Point next; 
		if (i == contourvec.size() - 1) next = contourvec[0];
		else contourvec[i + 1];

		int difX = current.x - next.x;
		int difY = current.y - next.y;


	}
	return energy;
}

int runOpdracht3() {
	Mat image = imread("monsters.jpg", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat dst, cdst, gray_image, mat16s, treshold_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	GaussianBlur(gray_image, treshold_image, Size(7, 7), 0, 0);
	threshold(treshold_image, treshold_image, 50, 1, THRESH_BINARY_INV);

	vector<vector<Point>> contourVector;
	allContours(treshold_image, contourVector);
	vector<Point> scaledContour;
	
	makeGrid(contourVector[0], scaledContour, 10, treshold_image);
	bendingEnergy(treshold_image, scaledContour);
	waitKey(0);
	return 0;
}
