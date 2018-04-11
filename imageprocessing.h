#ifndef  IMAGEPROCESSING_HEADER
#define IMAGEPROCESSING_HEADER
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\ml\ml.hpp>
#include <opencv2\core\utility.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../config_anprs.h"

class ImageProcessing
{
public:

};

void convertToGrayScale(cv::Mat src, cv::Mat& dst); //Convert image into gray scale mode
std::vector<cv::Mat> DetectAndCutCharRegion(cv::Mat plate); //Detect region of chars for char recognize
std::vector<cv::Mat> DetectAndCutNumberPlateRegion(cv::Mat src, cv::Mat frame_gray); //Detect and cut plate number region
cv::Mat cut_plate(cv::Mat src, cv::Point p1, cv::Point p2);//Cut plate number
void loadCascade(); //Load xml file for detect region plate number
bool downloadCascadeFromServer();//download cascade file from server
cv::Rect FindPlate(cv::Mat frame);
#endif//IMAGEPROCESSING_HEADER 


