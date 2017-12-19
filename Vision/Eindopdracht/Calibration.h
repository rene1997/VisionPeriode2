#pragma once

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/shape/hist_cost.hpp>
#include <iostream>
#include <fstream>
#include "opencv/highgui.h"

using namespace std;
using namespace cv;

int calibrateAndSaveCamera(VideoCapture cap);
void loadCalibration();

#endif // !CALIBRATION_H


