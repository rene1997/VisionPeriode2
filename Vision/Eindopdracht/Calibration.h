#pragma once

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include <fstream>
#include <opencv2/shape/hist_cost.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/ml.hpp>

using namespace std;
using namespace cv;

void saveCalibration(VideoCapture cap);
void loadCalibration();
void fixFrame(Mat original, Mat & fixed);

#endif // !CALIBRATION_H


