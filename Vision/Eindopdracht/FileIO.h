#pragma once
#ifndef FILEIO_H
#define FILEIO_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/shape/hist_cost.hpp>
#include <opencv2/ml.hpp>
#include<vector>
#include <iostream>
#include <iostream>
#include <fstream>

#include "Source.h"


using namespace cv;
using namespace std;

bool saveData(Mat v0, Mat w0);
bool loadData(Mat & v0, Mat & w0);


#endif // !FILEIO_H
