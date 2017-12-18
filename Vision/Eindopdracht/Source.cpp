// Demo: Training of a Neural Network / Back-Propagation algorithm 
// Jan Oostindie, Avans Hogeschool, dd 6-12-2016
// email: jac.oostindie@avans.nl

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <opencv2/shape/hist_cost.hpp>
//#include "avansvisionlib20.h" // versie 2.0 (!)

using namespace cv;
using namespace std;

int main() {
	Mat image, gray_image;
	VideoCapture capture = VideoCapture(0);
	capture >> image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	imshow("camera",image);
	imshow("gray_Image", image);
	waitKey(0);
	return 0;
}
