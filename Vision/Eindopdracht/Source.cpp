#include "Source.h"

void trainNeuralNetwork(Mat image, int objectClass);
string convert(unsigned int val);



vector<classData> pictureData;

int main() {
	//load calibration:
	loadCalibration();
	//vector<Point> testpoints;
	//testpoints.push_back({ 1,1 });
	//pictureData.push_back({ testpoints, 55,100,2,2,25,8});
	////saveData(pictureData);

	Mat image, gray_image, correctImage;
	VideoCapture capture = VideoCapture(1);
	while (1) {
		capture >> image;
		fixFrame(image, correctImage);
		cvtColor(correctImage, gray_image, CV_BGR2GRAY);
		imshow("original", image);
		imshow("calibrated", correctImage);
		imshow("gray_Image", gray_image);
		int key = waitKey(1);
		if (key == 't') {
			bool done = false;
			int trainingClass;
			while (!done) {
				putText(gray_image, "Give the number visible on the trainingset", cvPoint(30, 30),
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
				imshow("gray_Image", gray_image);
				trainingClass = waitKey(1);
				if(trainingClass >= 0)
					done = true;
			}
			int startindex = pictureData.size();
			trainNeuralNetwork(image, trainingClass);
			for(int i = startindex; i < pictureData.size(); i++)
			{
				if (trainingClass == '+') pictureData[i].expectedValue = 15;
				else pictureData[i].expectedValue = trainingClass - '0';
			}
			
		}
		if(key == 'q')
		{
			int test = pictureData.size();
			Mat trainingSet = (Mat_<double>(pictureData.size(), 7));
			Mat expectedSet = (Mat_<double>(pictureData.size(), 4));
			for (int i = 0; i<pictureData.size(); i++)
			{
				//int index = i * trainingSet.cols;
				//trainingSet[index] = pictureData[i].area;
				trainingSet.at<double>(i, 0) = (double)pictureData[i].area;
				trainingSet.at<double>(i, 1) = (double)pictureData[i].contour.size()/1000;
				//trainingSet.at<double>(i, 2) = (double)pictureData[i].energy/100;
				trainingSet.at<double>(i, 2) = pictureData[i].numberOfHoles;
				trainingSet.at<double>(i, 3) = pictureData[i].amountOfDefects;
				trainingSet.at<double>(i, 4) = pictureData[i].meanValueDefects;
				trainingSet.at<double>(i, 5) = pictureData[i].aspectRatio;
				trainingSet.at<double>(i, 6) = pictureData[i].centerPoint;
				string x = convert(pictureData[i].expectedValue);
				expectedSet.at<double>(i, 0) = x[0] - '0';
				expectedSet.at<double>(i, 1) = x[1] - '0';
				expectedSet.at<double>(i, 2) = x[2] - '0';
				expectedSet.at<double>(i, 3) = x[3] - '0';
			}
			bnp(trainingSet, expectedSet);
		}
		if (key == 'c') {
			saveCalibration(capture);
		}
		if (key == ' ') {
			//TODO: 
		}
	}
	
	waitKey(0);
	return 0;
}

string convert(unsigned int val)
{
	stringstream s;
	unsigned int mask = 1 << (sizeof(int)  - 1);

	for (int i = 0; i < sizeof(int); i++)
	{
		if ((val & mask) == 0)
		s << '0';
		else
		s << '1';

		mask >>= 1;
	}
	return s.str();
}

void trainNeuralNetwork(Mat image, int objectClass) {
	//treshold and convert to gray
	Mat gray_image, treshold_image, mat16s, labeled;
	vector<Point2d*> startPoints, posVec;
	vector<int> areaVec;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	threshold(gray_image, treshold_image, 175, 1, THRESH_BINARY_INV);
	//imshow("treshold", treshold_image);

	//label blobs
	treshold_image.convertTo(mat16s, CV_16S);
	//imshow("treshold1", mat16s);
	//imwrite("treshold.bmp", treshold_image);
	int blob2Amount = labelBLOBsInfo(mat16s, labeled, startPoints, posVec, areaVec);

	//get contours
	vector<vector<Point>> contourVector, bbs;
	vector< Vec4i > hierarchy2;
	vector<Mat> singleMat;
	//allContours(treshold_image, contourVector);
	findContours(treshold_image, contourVector, hierarchy2,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	int maxContourSize = 0;
	for (int i = contourVector.size() - 1; i >= 0; i--) {
		if (hierarchy2[i][3] != -1)
			contourVector.erase(contourVector.begin() + i);
	}
	allBoundingBoxes(contourVector, bbs, singleMat, treshold_image);

	//for each contour get energy feature
	for (int i = 0; i < contourVector.size(); i ++) {
		vector<Point> gridContour;
		vector < vector<Point>> contours;
		vector< Vec4i > hierarchy;
		int numberOfHoles = 0;
		makeGrid(contourVector[i], gridContour);
		int energy = bendingEnergy(gridContour);
		imwrite("dstfsd.bmp", singleMat[i]);
		
		//find number of holes
		findContours(singleMat[i], contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

		vector<int>  hullsI(gridContour.size()); // Indices to contour points
		vector<Vec4i> defects;
		convexHull(gridContour, hullsI, false);
		convexityDefects(gridContour, hullsI, defects);
		double amountOfDefects = 0;
		double meanDefects = 0;
		for (const Vec4i& v : defects)
		{
			float depth = v[3] / 500;
			if (depth > 1) //  filter defects by depth, e.g more than 10
			{
				amountOfDefects++;
				meanDefects += depth;
			}
		}
		if(amountOfDefects !=0)
			meanDefects = meanDefects / amountOfDefects;
		
		//RotatedRect rect =  fitEllipse(contours[0]);
		RotatedRect rect = minAreaRect(contours[0]);
		double width = rect.size.width;
		double height = rect.size.height;
		double centerX = rect.center.x;
		double centerY = rect.center.y;

		double  aspectratio = width / height / 100;
		double centerPoint = (centerX * centerY);
		
		//1double extent = contourArea(contours[0]) / rect.size();



		for (int j = 0; j< contours.size(); j++) // iterate through each contour.
		{
			if (hierarchy[j][3] != -1)
				numberOfHoles++;
		}
		/*vector<Point> test = fitEllipse(contours[0]);
		boundingRect(contours[0]);*/
		//push feature to feature data
		pictureData.push_back({ contours[0],(double)energy/100,(double)areaVec[i]/1000,(double)numberOfHoles/10,(double)amountOfDefects/10,meanDefects/100, aspectratio*100 , centerPoint/1000});
	}

	//get class number
	int number = objectClass - '0' ;
	if (number < 0 || number > 9) {
		cout << "got number: " << number << endl;
		if (number == -5) {
			//got +
		}
		else if (number == -3) {
			//got - 
		}
	}
}
