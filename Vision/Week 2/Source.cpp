#include <opencv2\opencv.hpp>
#include <iostream>
#include "avansvisionlib.h"

using namespace cv;
using namespace std;

int runOpdracht2();
int runOpdracht3();
int allContours(Mat, vector<vector<Point>> &);

//array for running clocwise around pixel.
int clockWiseX[] = { -1,-1,0,1,1, 1, 0,-1,-1 };
int clockWiseY[] = {  0, -1,-1,-1,0,1,1,1, 0 };
int clockWiseSize = 9;


int main() {
	return runOpdracht2();
	//return runOpdracht3();
}

int allContours(Mat binaryImage, vector<vector<Point>> & contourVecVec) {
	// Mat for labelingBlobs
	Mat mat16s, labeled;
	vector<Point2d*> startPoints, posVec;
	vector<int> areaVec;

	binaryImage.convertTo(mat16s, CV_16S);
	//get start point of blobs.
	int blob2Amount = labelBLOBsInfo(mat16s, labeled, startPoints, posVec, areaVec);
	//make a black image with the same dimentions as origional
	Mat mooreBoundary = binaryImage.clone();
	mooreBoundary = 0;

	//This for loop is used for every blob
	for (int i = 0;i < startPoints.size(); i++)
	{
		//get x and y values.
		int x = startPoints[i]->y, y = startPoints[i]->x;
		vector<Point> points;
		Point firstPoint = { x,y };
		points.push_back(firstPoint);
		
		//Loop continous till point is the same as first point.
		while (((firstPoint.x != points[points.size() -1].x) || (firstPoint.y != points[points.size() -1].y)) || (points.size() <= 1)) {
			bool added = false;
			int edge = 0;
			for (int c = 0; c < clockWiseSize; c++) {	
				//if pixel is outside of the blob
				if ((binaryImage.at<uchar>((y + clockWiseY[c]), (x + clockWiseX[c])) == 0))
				{
					edge++;
				}
				//if edge is bigger than 1 and not been added yet 
				else if (edge>1 && !added)
				{
					x += clockWiseX[c];
					y += clockWiseY[c];
					Point point = {x,y};
					//push point back in vector
					points.push_back(point);
					added = true;
					//draw boundry in image
					mooreBoundary.at<uchar>(y, x)=255;
				}
				//when no pixel is added start over
				if (!added && c == clockWiseSize-1) { c = 0; };
			}
		}
		// show contours in image
		imshow("mooreBoundary", mooreBoundary);
		cout << "Object found " << points.size() << endl;
		//pushback points in contourvector
		contourVecVec.push_back(points);
	}
	return -1;
}


int runOpdracht2() {
	//load image
	Mat image = imread("monsters.jpg", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	Mat gray_image, treshold_image;
	//make gray image
	cvtColor(image, gray_image, CV_BGR2GRAY);
	//bluring image
	GaussianBlur(gray_image, treshold_image, Size(7, 7), 0, 0);
	threshold(treshold_image, treshold_image, 50, 255, THRESH_BINARY_INV);
	imshow("source image", treshold_image);

	vector<vector<Point>> contourVector;
	//find all contours
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
	imshow("grid test image", testImage);
	return;
}

double bendingEnergy(Mat binaryImage, vector<Point> & contourvec) {
	double energy = 0;
	double direction = 0;
	for (int i = 0; i < contourvec.size(); i++) {
		Point current = contourvec[i];
		Point next; 
		if (i == contourvec.size() - 1) next = contourvec[0];
		else next = contourvec[i + 1];

		int difX = current.x - next.x;
		int difY = current.y - next.y;
		
		double currentDir;
		if (difY == 0) currentDir = 0;
		else currentDir = fabs(difX / difY);
		energy += fabs(currentDir - direction);
		direction = currentDir;
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
	imshow("source image", gray_image);

	vector<vector<Point>> contourVector;
	allContours(treshold_image, contourVector);
	vector<Point> scaledContour, s2;
	
	makeGrid(contourVector[0], scaledContour, 10, treshold_image);
	double monster1 = bendingEnergy(treshold_image, scaledContour);
	cout << "Bending energy first object: " << monster1 << endl;
	
	makeGrid(contourVector[1], s2, 10, treshold_image);
	double monster2 = bendingEnergy(treshold_image, s2);
	cout << "Bending energy second object: " << monster2 << endl;
	waitKey(0);
	return 0;
}
