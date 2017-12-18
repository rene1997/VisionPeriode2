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

int main() {
	Mat image, gray_image;
	VideoCapture capture = VideoCapture(0);
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
	//treshold:
	Mat gray_image, treshold_image, mat16s, labeled;
	vector<Point2d*> startPoints, posVec;
	vector<int> areaVec;

	cvtColor(image, gray_image, CV_BGR2GRAY);
	threshold(gray_image, treshold_image, 122, 255, CV_THRESH_BINARY);
	treshold_image.convertTo(mat16s, CV_16S);
	//get start point of blobs.
	int blob2Amount = labelBLOBsInfo(mat16s, labeled, startPoints, posVec, areaVec);

	//get class number
	int number = objectClass - '0' ;
	if (number < 0 || number > 9) {
		//TODO:  + / -
	}
}
