//#include "Calibration.h"
//
//int calibrateAndSaveCamera(VideoCapture capture)
//{
//	int numBoards;
//	int numCornersHor;
//	int numCornersVer;
//
//	cout << "Enter number of corners along width: ";
//	cin >> numCornersHor;
//
//	cout << "Enter number of corners along height: ";
//	cin >> numCornersVer;
//
//	cout << "Enter number of boards: ";
//	cin >> numBoards;
//	// We also create some additional variables that we'll be using later on. 
//	int numSquares = numCornersHor * numCornersVer;
//	Size board_sz = Size(numCornersHor, numCornersVer);
//	vector<vector<Point3f>> object_points;
//	vector<vector<Point2f>> image_points;
//	vector<Point2f> corners;
//	int successes = 0;
//	vector<Point3f> obj;
//	for (int j = 0; j < numSquares; j++)
//		obj.push_back(Point3f(j / numCornersHor, j%numCornersHor, 0.0f));
//
//	// Then we create two images and get the first snapshot from the camera:
//	Mat image;
//	Mat gray_image;
//	capture >> image;
//	while (successes < numBoards)
//	{
//		// convert to gray scale
//		cvtColor(image, gray_image, CV_BGR2GRAY);
//
//		// And we're here. The key functions:
//		// findChessboardCorners tries to find a chessboard in the image. 
//		// IF found THEN the rough corners are returned.    
//		bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
//		if (found)
//		{
//			// cornerSubPix refines the found corners. 
//			// De rough corners returned by findChessbooardCorners and the gray_image are input, 
//			// the refined corners are output.
//			cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
//
//			// draw the refined corners on the image i.e. chessboard.
//			drawChessboardCorners(gray_image, board_sz, corners, found);
//		}
//
//		// show results
//		imshow("orginal", image);
//		imshow("gray image", gray_image);
//
//		capture >> image;
//
//		int key = waitKey(1);
//
//		// Leave the program by pressing ESC-key
//		if (key == 27) return 0;
//
//		// spacebar and chessboard found ==> save the snap   
//		if (key == ' ' && found != 0)
//		{
//			image_points.push_back(corners);
//			object_points.push_back(obj);
//
//			printf("Snap stored!");
//
//			successes++;
//
//			if (successes >= numBoards)
//				break;
//		}
//	} // while
//
//	  // Next, we get ready to do the calibration. We declare variables that will hold the unknowns: 
//	  // Matrix intrinsic contains cx,cy,fx,fy
//	  // Matrix disCoeffs contains the distortion coefficients: 3 numbers radial distortion and 2 numbers tangential distortion 
//	Mat intrinsic = Mat(3, 3, CV_32FC1);
//	Mat distCoeffs;
//	vector<Mat> rvecs;
//	vector<Mat> tvecs;
//
//	// We modify the intrinsic matrix with whatever we know. 
//	// The camera's aspect ratio is 1 (that's usually the case... 
//	// i.e. fx = fy = f. If not, change it as required. 
//	// Elements (0,0) and (1,1) are the focal lengths along the X and Y axis.
//	intrinsic.ptr<float>(0)[0] = 1;
//	intrinsic.ptr<float>(1)[1] = 1;
//
//	// Determine the intrinsic matrix, distortion coefficients and the rotation+translation vectors. 
//	// Note: The calibrateCamera function converts all matrices into 64F format even if you 
//	// initialize it to 32F.
//	calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
//
//	cout << endl << endl << "intrinsic = " << intrinsic << endl << endl;
//	cout << "distCoeffs = " << distCoeffs << endl;
//
//	/***** saven van de callibratie data *****/
//
//	// YML-file aanmaken
//	FileStorage fs("b:\\test\\ueyecallib.yml", FileStorage::WRITE);
//
//	// wegschrijven van callibratie data naar de YML-file 
//	fs << "intrinsic" << intrinsic << "distCoeffs" << distCoeffs;
//
//	// de file afsluiten
//	fs.release();
//}