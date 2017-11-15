#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int runOpdracht2();
int runOpdracht3();
int allContours(Mat image, vector < vector<Point>> & countourVecVec);

int main() {
	//return runOpdracht2();
	return runOpdracht3();
}


int runOpdracht2() {
	Mat image = imread("monsters.jpg");
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", image);
	vector<vector<Point>> contourVecVec;
	allContours(image, contourVecVec);
	
	waitKey(0);
		return 0;
}

int allContours(Mat binaryImage, vector< vector<Point> > & contourVecVec) {	Mat greyImage, treshold;	cvtColor(binaryImage, greyImage, CV_BGR2GRAY);	return 0;}int runOpdracht3() {
}