#include "Calibration.h"

Mat intrinsic, distCoeffs;

void saveCalibration(VideoCapture capture)
{
	int numBoards = 5;
	int numCornersHor = 6;
	int numCornersVer = 9;
	int numSquares = numCornersHor * numCornersVer;
	Size board_sz = Size(numCornersHor, numCornersVer);
	
	vector<vector<Point3f>> object_points;
	vector<vector<Point2f>> image_points;

	vector<Point2f> corners;
	int successes = 0;

	vector<Point3f> obj;
	for (int j = 0; j < numSquares; j++)
		obj.push_back(Point3f(j / numCornersHor, j%numCornersHor, 0.0f));
		Mat image;
	Mat gray_image;
	capture >> image;

	while (successes < numBoards)
	{
		cvtColor(image, gray_image, CV_BGR2GRAY);
		bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		if (found)
		{
			cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray_image, board_sz, corners, found);
		}

		imshow("orginal", image);
		putText(gray_image, "press space to calibrate", cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(200, 200, 200));
		imshow("gray image", gray_image);

		capture >> image;

		int key = waitKey(1);

		if (key == 27) return;

		// spacebar and chessboard found ==> save the snap   
		if (key == ' ' && found != 0)
		{
			image_points.push_back(corners);
			object_points.push_back(obj);
			printf("Snap stored!");
			successes++;
			if (successes >= numBoards)
				break;
		}
	} 
	Mat intrinsic = Mat(3, 3, CV_32FC1);
	Mat distCoeffs;
	vector<Mat> rvecs;
	vector<Mat> tvecs;
	intrinsic.ptr<float>(0)[0] = 1;
	intrinsic.ptr<float>(1)[1] = 1;
	calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

	cout << endl << endl << "intrinsic = " << intrinsic << endl << endl;
	cout << "distCoeffs = " << distCoeffs << endl;

	/***** saven van de callibratie data *****/
	FileStorage fs("ueyecallib.yml", FileStorage::WRITE);
	fs << "intrinsic" << intrinsic << "distCoeffs" << distCoeffs;
	fs.release();
}

void loadCalibration() {
	FileStorage fs("ueyecallib.yml", FileStorage::READ);

	// callibratie data ophalen
	fs["intrinsic"] >> intrinsic;
	fs["distCoeffs"] >> distCoeffs;

	// callibratie matrices tonen op het scherm
	cout << "intrinsic matrix: " << intrinsic << endl;
	cout << "distortion coeffs: " << distCoeffs << endl;
}

void fixFrame(Mat original, Mat & fixed)
{
	undistort(original, fixed, intrinsic, distCoeffs);
}