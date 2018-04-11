#pragma once
#ifndef  THREADPROCESSING_HEADER
#define THREADPROCESSING_HEADER
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include "SVMManager.h"
#include "ExceptionPlate.h"
#include "ExceptionManager.h"

using namespace std;

struct Input {
	cv::Mat m_frameofvideo;
	cv::Mat m_frame_gray;
	SVMManager* m_svmManager;
	ExceptionManager* m_exceptionManger;
	bool m_isCheckingDetectFail;
	bool m_isCaptureExceptionPlate;
	ExceptionPlate* m_exceptionPlate;
};
struct Result {
	std::vector<string> result;
};
ref class ThreadProcessing
{
public:
	ThreadProcessing();
	~ThreadProcessing();
	void setInput(cv::Mat frameofvideo, cv::Mat frame_gray, SVMManager* svmManager, ExceptionManager * expManger, ExceptionPlate* exceptionPlate, bool saveAllCharacterDetected, bool saveCaptureExceptionPlate);
	void processing();
	Result getResult();
	void Release();
private:
	Input *m_input;
	Result * m_result;

};

#endif//THREADPROCESSING_HEADER