#include "ExceptionPlate.h"
using namespace cv;
using namespace std;

ExceptionPlate::ExceptionPlate()
{
	stackExceptionPlate.clear();
}

ExceptionPlate::~ExceptionPlate()
{
	stackExceptionPlate.clear();
}

void ExceptionPlate::addPlate(Mat &plate)
{
	stackExceptionPlate.push_back(plate);
}

vector<Mat> ExceptionPlate::getExceptionPlate()
{
	return stackExceptionPlate;
}

void ExceptionPlate::clearPlate()
{
	stackExceptionPlate.clear();
}

