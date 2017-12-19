#include "FeatureDetection.h"

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

int allBoundingBoxes(const vector<vector<Point>> & contours, vector <vector<Point>> & bbs, vector<Mat> & singleBlobs, Mat binaryImage) {
	for (int i = 0; i < contours.size(); i++) {
		int minX, minY, maxX, maxY;
		vector<Point> currentContour = contours[i];
		minX = maxX = currentContour[i].x;
		minY = maxY = currentContour[i].y;
		for (int j = 1; j < currentContour.size(); j++) {
			Point currentPoint = currentContour[j];
			if (currentPoint.x < minX)
				minX = currentPoint.x;
			else if (currentPoint.x > maxX)
				maxX = currentPoint.x;
			if (currentPoint.y < minY)
				minY = currentPoint.y;
			else if (currentPoint.y > maxY)
				maxY = currentPoint.y;
		}
		vector<Point> currentBox;
		for (int j = minX; j < maxX; j++) {
			currentBox.push_back({ j,minY });
			currentBox.push_back({ j,maxY });
		}

		for (int j = minY; j < maxY; j++) {
			currentBox.push_back({ minX,j });
			currentBox.push_back({ maxX,j });
		}
		Mat blobMat = binaryImage(Rect(minX-1, minY-1, (3+maxX - minX), (3+maxY - minY)));
		singleBlobs.push_back(blobMat);

		bbs.push_back(currentBox);
	}
	return 0;
}


void makeGrid(vector<Point> & contour, vector<Point> & newContour, int scale) {
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
}

double bendingEnergy(vector<Point> & contourvec) {
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