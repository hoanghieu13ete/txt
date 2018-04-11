#pragma once
#ifndef  ULTILITY_HEADER
#define ULTILITY_HEADER
#include <string>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\ml\ml.hpp>
#include "SVMManager.h"
#include "ThreadProcessing.h"

struct ImageChar {
	string imageName;
	cv::Mat imageSource;
};
public struct MatCharResult
{
	vector<ImageChar> plateChar;
};



ref class Ultility
{
public:
	Ultility();
	~Ultility();
	static bool isMotobikePlate(const string & plate);
	static string makeupPlate(string & plate);
	static bool isLowerPercent(std::vector<float> probability);
	static float avgPercent(std::vector<float> probability);
	static bool isSamePlate(const string& currentPlate, const string & previousPlate);
	static std::vector<string> processFrame(Input * inputData);
	static bool isProcessing();
	static void setIsProcessing(bool flag);
	static MatCharResult getMatCharResult();
	static void Release();
private:
	static bool m_isProcessing;
	static MatCharResult* m_MatCharResult;

};
#endif//ULTILITY_HEADER
