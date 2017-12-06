#pragma once
#ifndef  CONTOURS_H
#define CONTOURS_H
#include <opencv2\opencv.hpp>
#include <iostream>
#include "avansvisionlib.h"
using namespace cv;
using namespace std;

int allContours(Mat, vector<vector<Point>> &);
void makeGrid(vector<Point> & contour, vector<Point> & newContour, int scale, Mat & image);
double bendingEnergy(Mat binaryImage, vector<Point> & contourvec);

#endif //  CONTOURS_H

