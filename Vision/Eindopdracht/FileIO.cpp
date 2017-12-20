#include "FileIO.h"

bool saveData(Mat v0, Mat w0)
{
	try {
		FileStorage fs("bnpData.yml", FileStorage::WRITE);
		fs << "v0" << v0 << "w0" << w0;
		fs.release();
		return true;
	}
	catch(Exception ex){
		return false;
	}
}

bool loadData(Mat & v0, Mat &w0) {
	try {
		FileStorage fs("bnpData.yml", FileStorage::READ);
		fs["v0"] >> v0;
		fs["w0"] >> w0;
		fs.release();
		return true;
	}
	catch (Exception ex) {
		return false;
	}
}