#ifndef  VIDEOPROCESSING_HEADER
#define VIDEOPROCESSING_HEADER
#include <string>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include "../config_anprs.h"



class videoprocessing
{
	cv::VideoCapture cap;
	cv::Mat frameofvideo;
public:
	void play();//play video file
	void init(int idCamera);//init open camera before read frame
	void init(const std::string& ipAddress);//init open camera before read frame
	cv::Mat getFrame();//get video frame
	cv::Mat StartCamProcessing();//Start USB Camera capture
	void StopCamorVideoProcessing();//Stop Camera capture
};

#endif//VIDEOPROCESSING_HEADER