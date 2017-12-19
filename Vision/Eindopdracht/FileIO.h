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

#include "Source.h"

using namespace cv;
using namespace std;

struct classData {
	vector<Point> contour;
	int energy;
	int area;
	int numberOfHoles;
	double amountOfDefects;
	double meanValueDefects;
	int expectedValue;
};
//
//ostream& operator<<(ostream& out, const vector<Point>& h) {
//	for (Point p : h)
//		out << p.x << ' ' << p.y << ' ';
//	return out;
//}
//
//istream& operator>>(istream& in, vector<Point>& h) {
//	for (Point p : h)
//		in >> p.x >> p.y;
//	return in;
//}
//
//ostream& operator<<(ostream& out, const classData& h) {
//	return out << h.contour << ' ' << h.energy << ' ' << h.area << ' ' << h.numberOfHoles << ' ' << h.amountOfDefects << ' ' << h.meanValueDefects << ' ' << h.expectedValue;
//}
//
//istream& operator>>(istream& in, classData& h) {
//	classData values;
//	in >> values.contour >> values.energy >> values.area >> values.numberOfHoles >> values.amountOfDefects >> values.meanValueDefects >> values.expectedValue;
//	h = move(values);
//	return in;
//}

bool saveData(vector<classData> featureData);


#endif // !FILEIO_H
