#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/shape/hist_cost.hpp>

#include <iostream>

#include "avansvisionlib20.h" 
#include "FeatureDetection.h"
#include "FileIO.h"
#include <opencv2/ml.hpp>

using namespace cv;
using namespace std;

void trainNeuralNetwork(Mat image, int objectClass);
string convert(unsigned int val);

struct classData {
	vector<Point> contour;
	int energy;
	int area;
	int numberOfHoles;
	int expectedValue;
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
			int startindex = pictureData.size();
			trainNeuralNetwork(image, trainingClass);
			for(int i = startindex; i < pictureData.size(); i++)
			{
				pictureData[i].expectedValue = trainingClass - '0';
			}
			
		}
		if(key == 'q')
		{
			int test = pictureData.size();
			Mat trainingSet = (Mat_<double>(pictureData.size(), 4));
			Mat expectedSet = (Mat_<double>(pictureData.size(), 4));
			for (int i = 0; i<pictureData.size(); i++)
			{
				//int index = i * trainingSet.cols;
				//trainingSet[index] = pictureData[i].area;
				trainingSet.at<double>(i, 0) = pictureData[i].area;
				trainingSet.at<double>(i, 1) = pictureData[i].contour.size();
				trainingSet.at<double>(i, 2) = pictureData[i].energy;
				trainingSet.at<double>(i, 3) = pictureData[i].numberOfHoles;
				string x = convert(pictureData[i].expectedValue);
				expectedSet.at<double>(i, 0) = x[0] - '0';
				expectedSet.at<double>(i, 1) = x[1] - '0';
				expectedSet.at<double>(i, 2) = x[2] - '0';
				expectedSet.at<double>(i, 3) = x[3] - '0';
			}
			bnp(trainingSet, expectedSet);
		}
		if (key == ' ') {
			//TODO: 
		}
	}
	
	waitKey(0);
	return 0;
}

string convert(unsigned int val)
{
	stringstream s;
	unsigned int mask = 1 << (sizeof(int)  - 1);

	for (int i = 0; i < sizeof(int); i++)
	{
		if ((val & mask) == 0)
		s << '0';
		else
		s << '1';

		mask >>= 1;
	}
	return s.str();
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
	imwrite("treshold.bmp", treshold_image);
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
		Moments mom =moments(contours[0]);
		double hu[7];
		HuMoments(mom, hu);
		for (int j = 0; j< contours.size(); j++) // iterate through each contour.
		{
			if (hierarchy[j][3] != -1)
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
