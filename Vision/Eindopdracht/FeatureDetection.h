#pragma once
#ifndef FEATUREDETECTION_H
#define FEATUREDETECTION_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include "avansvisionlib20.h"

using namespace cv;
using namespace std;

int allContours(Mat binaryImage, vector<vector<Point>> & contourVecVec);
int allBoundingBoxes(const vector<vector<Point>> & contours, vector <vector<Point>> & bbs, vector<Mat> & singleBlobs, Mat binaryImage);
void makeGrid(vector<Point> & contour, vector<Point> & newContour, int scale);
double bendingEnergy(vector<Point> & contourvec);

#endif // !FEATUREDETECTION_H

