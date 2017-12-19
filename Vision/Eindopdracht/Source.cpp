#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/shape/hist_cost.hpp>

#include <iostream>

#include "avansvisionlib20.h" 
#include "FeatureDetection.h"
#include "FileIO.h"

using namespace cv;
using namespace std;

void trainNeuralNetwork(Mat image, int objectClass);

struct classData {
	vector<Point> contour;
	int energy;
	int area;
	int numberOfHoles;
};


vector<classData> pictureData;

int main() {
	Mat image, gray_image;
	VideoCapture capture = VideoCapture(1);
	while (1) {
		capture >> image;
		cvtColor(image, gray_image, CV_BGR2GRAY);
		imshow("camera", image);
		imshow("gray_Image", gray_image);
		int key = waitKey(1);
		if (key == 't') {
			bool done = false;
			int trainingClass;
			while (!done) {
				putText(gray_image, "Give the number visible on the trainingset", cvPoint(30, 30),
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
				imshow("gray_Image", gray_image);
				trainingClass = waitKey(1);
				if(trainingClass >= 0)
					done = true;
			}
			trainNeuralNetwork(image, trainingClass);
			
		}
		if (key == ' ') {
			//TODO: 
		}
	}
	
	waitKey(0);
	return 0;
}

void trainNeuralNetwork(Mat image, int objectClass) {
	//treshold and convert to gray
	Mat gray_image, treshold_image, mat16s, labeled;
	vector<Point2d*> startPoints, posVec;
	vector<int> areaVec;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	threshold(gray_image, treshold_image, 122, 1, THRESH_BINARY_INV);
	imshow("treshold", treshold_image);

	//label blobs
	treshold_image.convertTo(mat16s, CV_16S);
	imshow("treshold1", mat16s);
	int blob2Amount = labelBLOBsInfo(mat16s, labeled, startPoints, posVec, areaVec);

	//get contours
	vector<vector<Point>> contourVector, bbs;
	vector<Mat> singleMat;
	allContours(treshold_image, contourVector);
	allBoundingBoxes(contourVector, bbs, singleMat, treshold_image);

	//for each contour get energy feature
	for (int i = 0; i < contourVector.size(); i ++) {
		vector<Point> gridContour;
		vector < vector<Point>> contours;
		vector< Vec4i > hierarchy;
		int numberOfHoles = 0;
		makeGrid(contourVector[i], gridContour, 10);
		int energy = bendingEnergy(gridContour);

		imwrite("dstfsd.bmp", singleMat[i]);
		
		//find number of holes
		findContours(singleMat[i], contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		for (int i = 0; i< contours.size(); i++) // iterate through each contour.
		{
			if (hierarchy[i][3] != -1)
				numberOfHoles++;
		}
		//push feature to feature data
		pictureData.push_back({ gridContour,energy,areaVec[i],numberOfHoles });
	}

	//get class number
	int number = objectClass - '0' ;
	if (number < 0 || number > 9) {
		cout << "got number: " << number << endl;
		if (number == -5) {
			//got +
		}
		else if (number == -3) {
			//got - 
		}
	}
}
