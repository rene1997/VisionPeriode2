#include "contours.h"

int clockWiseX[] = { -1,-1,0,1,1, 1, 0,-1,-1 };
int clockWiseY[] = { 0, -1,-1,-1,0,1,1,1, 0 };
int clockWiseSize = 9;

int allContours(Mat binaryImage, vector<vector<Point>> & contourVecVec) {
	Mat mat16s, labeled;
	vector<Point2d*> startPoints, posVec;
	vector<int> areaVec;

	binaryImage.convertTo(mat16s, CV_16S);
	int blob2Amount = labelBLOBsInfo(mat16s, labeled, startPoints, posVec, areaVec);
	Mat mooreBoundary = binaryImage.clone();
	mooreBoundary = 0;

	for (int i = 0; i < startPoints.size(); i++)
	{
		int x = startPoints[i]->y, y = startPoints[i]->x;
		vector<Point> points;
		Point firstPoint = { x,y };
		points.push_back(firstPoint);

		while (((firstPoint.x != points[points.size() - 1].x) || (firstPoint.y != points[points.size() - 1].y)) || (points.size() <= 1)) {
			bool added = false;
			int edge = 0;
			for (int c = 0; c < clockWiseSize; c++) {
				if ((binaryImage.at<uchar>((y + clockWiseY[c]), (x + clockWiseX[c])) == 0))
				{
					edge++;
				}
				else if (edge>1 && !added)
				{
					x += clockWiseX[c];
					y += clockWiseY[c];
					Point point = { x,y };
					points.push_back(point);
					added = true;
					mooreBoundary.at<uchar>(y, x) = 255;
				}
				if (!added && c == clockWiseSize - 1) { c = 0; };
			}
		}
		//imshow("test", mooreBoundary);
		cout << "contour with " << points.size() << " points found" << endl;
		contourVecVec.push_back(points);
	}
	return -1;
}

void makeGrid(vector<Point> & contour, vector<Point> & newContour, int scale, Mat & image) {
	int x = 0;
	int y = 0;
	//Mat testImage = image.clone();
	//testImage = 0;
	for (int i = 0; i < contour.size(); i++) {
		x += contour[i].x;
		y += contour[i].y;
		if (i == contour.size() - 1 && contour.size() % scale > scale / 2) {
			scale = contour.size() % scale;
		}
		if ((i + 1) % scale == 0 || (i == contour.size() - 1 && contour.size() % scale > scale / 2)) {

			int avgX = abs(x / scale);
			int avgY = abs(y / scale);
			newContour.push_back({ avgX,avgY });
			//testImage.at<uchar>(avgY, avgX) = 255;
			x = y = 0;
		}
	}
	//imshow("grid test image", testImage);
}

double bendingEnergy(Mat binaryImage, vector<Point> & contourvec) {
	double energy = 0;
	double direction = 0;
	for (int i = 0; i < contourvec.size(); i++) {
		Point current = contourvec[i];
		Point next;
		if (i == contourvec.size() - 1) next = contourvec[0];
		else next = contourvec[i + 1];

		int difX = current.x - next.x;
		int difY = current.y - next.y;

		double currentDir;
		if (difY == 0) currentDir = 0;
		else currentDir = fabs(difX / difY);
		energy += fabs(currentDir - direction);
		direction = currentDir;
	}
	return energy;
}