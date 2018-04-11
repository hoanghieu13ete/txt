#pragma once
#ifndef  EXCEPTIONPLATE_HEADER
#define EXCEPTIONPLATE_HEADER
#include <vector>
#include <opencv2/core.hpp>


class ExceptionPlate
{
public:
	ExceptionPlate();
	~ExceptionPlate();
	void addPlate(cv::Mat &plate);
	std::vector<cv::Mat> getExceptionPlate();
	void clearPlate();
private:
	std::vector<cv::Mat> stackExceptionPlate;


};

#endif
