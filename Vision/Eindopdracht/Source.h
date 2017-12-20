#pragma once

#ifndef SOURCE_H
#define SOURCE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/shape/hist_cost.hpp>
#include <opencv2/ml.hpp>

#include <iostream>

#include "avansvisionlib20.h" 
#include "FeatureDetection.h"
#include "FileIO.h"
#include "Calibration.h"


using namespace cv;
using namespace std;

struct classData {
	vector<Point> contour;
	double energy;
	double area;
	double numberOfHoles;
	double amountOfDefects;
	double meanValueDefects;
	double aspectRatio;
	double centerPoint;
	double defectSize;
	int expectedValue;
	Point startPoint;
};



#endif // !SOURCE_H

