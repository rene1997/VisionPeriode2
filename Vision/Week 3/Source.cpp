#include <opencv2\opencv.hpp>
#include <iostream>
#include "avansvisionlib.h"
#include "sstream"
#include "contours.h"
#include <stack>

using namespace cv;
using namespace std;

int allContours(Mat, vector<vector<Point>> &);
void makeGrid(vector<Point> & contour, vector<Point> & newContour, int scale, Mat & image);
double bendingEnergy(Mat binaryImage, vector<Point> & contourvec);


int runOpdracht2();
int runOpdracht3();
int runOpdracht3c();

int allBoundingBoxes(const vector<vector<Point>> & contours, vector < vector<Point>> & bbs, Mat treshold);

int main() {
	//return runOpdracht2();
	//return runOpdracht3();
	return runOpdracht3c();
}

int runOpdracht2() {
	Mat image = imread("monsters.jpg", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	Mat gray_image, blur, treshold_image, treshold_2;
	//make gray image
	cvtColor(image, gray_image, CV_BGR2GRAY);
	//bluring image
	GaussianBlur(gray_image, blur, Size(7, 7), 0, 0);
	imshow("test", blur);
	threshold(blur, treshold_image, 50, 1, THRESH_BINARY_INV);
	threshold(blur, treshold_2, 50, 255, THRESH_BINARY_INV);
	//imshow("source image", treshold_image);

	vector<vector<Point>> contourVector, bbs;
	//find all contours
	allContours(treshold_image, contourVector);
	allBoundingBoxes(contourVector, bbs, treshold_2);

	waitKey(0);
	return 0;
}


int allBoundingBoxes(const vector<vector<Point>> & contours, vector <vector<Point>> & bbs, Mat treshold) {
	for (int i = 0; i < contours.size(); i++) {
		int minX, minY, maxX, maxY;
		vector<Point> currentContour = contours[i];
		minX = maxX = currentContour[i].x;
		minY = maxY = currentContour[i].y;
		for (int j = 1; j < currentContour.size(); j++) {
			Point currentPoint = currentContour[j];
			if (currentPoint.x < minX)
				minX = currentPoint.x;
			else if (currentPoint.x > maxX)
				maxX = currentPoint.x;
			if (currentPoint.y < minY)
				minY = currentPoint.y;
			else if (currentPoint.y > maxY)
				maxY = currentPoint.y;
		}
		vector<Point> currentBox;
		for (int j = minX; j < maxX; j++) {
			currentBox.push_back({ j,minY });
			currentBox.push_back({ j,maxY});
			treshold.at<uchar>({ j,minY }) = 255;
			treshold.at<uchar>({ j,maxY }) = 255;
		}
			
		for (int j = minY; j < maxY; j++) {
			currentBox.push_back({ minX,j });
			currentBox.push_back({ maxX,j });
			treshold.at<uchar>({ minX,j}) = 255;
			treshold.at<uchar>({ maxX,j }) = 255;
		}
		stringstream s;
		s << "8obj" << i << ".bmp";
		Mat roiMap;
		roiMap= treshold(Rect(minX, minY, (maxX - minX), (maxY - minY)));
		imwrite(s.str(), roiMap);

		bbs.push_back(currentBox);
	}

	imshow("bbs", treshold);

	return 0;
}

Point fourConnected[] = { {-1,0},{0,-1}, {1,0}, {0,1}};

int enclosePixel(vector<Point> & regionPixels, Point & checkPoint) 
{
	//list with picels to check
	stack<Point> st;
	st.push(checkPoint);
	regionPixels.push_back(checkPoint);
	Point cp = checkPoint;

	//while there are pixels to check
	while (st.size() > 0) {
		//take and remove first pixel
		cp = st.top();
		st.pop();
		for (Point p : fourConnected) {
			Point newPoint = cp + p;
			//check if new neighbour is contour
			bool foundContour = false;
			for (Point contourPoint : regionPixels) {
				if (newPoint == contourPoint) {
					foundContour = true;
					break;
				}
			}
			//if not add to todopixesl
			if (!foundContour) {
				st.push(newPoint);
				regionPixels.push_back(newPoint);
			}
		}
	}
	return 1;
}

int enclosedPixels(const vector<Point> & contourVec, vector<Point> & regionPixels){	//calcultate midpoint:
	Point minY = contourVec[0];
	for (Point point : contourVec) {
		regionPixels.push_back(point);
		if (point.y < minY.y) {
			minY = point;
		}
	}
	Point startpoint = { minY.x +1, minY.y + 10 };
	
	//fill contour
	enclosePixel(regionPixels, startpoint);

	//draw image for testing
	//Mat test = Mat(500,500, CV_64F);
	//test = 0;
	//for (Point point: regionPixels)
	//{
	//	cout << "";
	//	test.at<double>(point.y,point.x) = 255;
	//}
	//imshow("test", test);
	return -1;
}


int runOpdracht3() {
	Mat image = imread("monsters.jpg", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}	
	Mat gray_image, blur, treshold_image, treshold_2;
	//make gray image
	cvtColor(image, gray_image, CV_BGR2GRAY);
	//bluring image
	GaussianBlur(gray_image, blur, Size(7, 7), 0, 0);
	imshow("test", blur);
	threshold(blur, treshold_image, 50, 1, THRESH_BINARY_INV);
	threshold(blur, treshold_2, 50, 255, THRESH_BINARY_INV);
	//imshow("source image", treshold_image);

	vector<vector<Point>> contourVector, bbs;
	vector<Point> regionPixels;
	//find all contours
	allContours(treshold_image, contourVector);
	allBoundingBoxes(contourVector, bbs, treshold_2);
	enclosedPixels(contourVector[0], regionPixels);

	waitKey(0);
	return 0;
}

void imageOfROI(vector<vector<Point>> & contourVecVec) {
	vector<vector<Point>> regionsPixels;
	for (int i = 0; i < contourVecVec.size(); i++) {
		vector<Point> regionPixels;
		vector<Point> contour = contourVecVec[i];
		int test = enclosedPixels(contour, regionPixels);
		//iuiuh
		int minX, minY, maxX, maxY;
		minX = maxX = contour[0].x;
		minY = maxY = contour[0].y;
		for (int j = 1; j < contour.size(); j++) {
			Point currentPoint = contour[j];
			if (currentPoint.x < minX)
				minX = currentPoint.x;
			else if (currentPoint.x > maxX)
				maxX = currentPoint.x;
			if (currentPoint.y < minY)
				minY = currentPoint.y;
			else if (currentPoint.y > maxY)
				maxY = currentPoint.y;
		}
		stringstream s;
		s << "8object" << i << ".bmp";
		Mat roiMap = Mat( (maxY - minY) + 1,(maxX - minX) + 1,CV_64F);
		roiMap = 0;
		for (Point point : regionPixels)
		{
			roiMap.at<double>((point.y - minY), (point.x - minX)) = 255;
		}
		imwrite(s.str(), roiMap);
		regionsPixels.push_back(regionPixels);
	}
	cout << "amount of objects " << regionsPixels.size() << endl;
	cout << "first region " << regionsPixels[0].size() << endl;
	return;
}

int runOpdracht3c() {
	Mat image = imread("8objecten.png", CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	Mat gray_image, blur, treshold_image, treshold_2;
	//make gray image
	cvtColor(image, gray_image, CV_BGR2GRAY);
	//bluring image
	GaussianBlur(gray_image, blur, Size(7, 7), 0, 0);
	imshow("test", blur);
	threshold(blur, treshold_image, 50, 1, THRESH_BINARY_INV);
	threshold(blur, treshold_2, 50, 255, THRESH_BINARY_INV);
	//imshow("source image", treshold_image);

	vector<vector<Point>> contourVector, bbs;
	//find all contours
	allContours(treshold_image, contourVector);
	allBoundingBoxes(contourVector, bbs, treshold_2);
	imageOfROI(contourVector);
	waitKey(0);
	return 0;
	


}