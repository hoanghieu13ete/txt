#include "ThreadProcessing.h"
#include "Ultility.h"
#include "Log.h"
#include "../config_anprs.h"

using namespace cv;
using namespace System;
using namespace System::Threading;
/*
This is constructor of class ThreadProcessing - processing frame  of video using another thread - not main thread
*/
ThreadProcessing::ThreadProcessing()
{
	m_input = DBG_NEW Input();
	m_result = DBG_NEW Result();
}
/*
This is destructor of class ThreadProcessing - processing frame  of video using another thread - not main thread
*/
ThreadProcessing::~ThreadProcessing()
{
}
/*
This function is to tranfer data to thread
Param:
	Mat frameofvideo: frame of video on matrix pixel
	Mat frame_gray: frame of video on matrix pixel on gray mode
	SVMManager * svmManager: svmManager object to using svmml lib
	bool saveAllCharacterDetected: option to save all maxtrix pixel of character has detected
Output:
*/
void ThreadProcessing::setInput(Mat frameofvideo, Mat frame_gray, SVMManager* svmManager, ExceptionManager * expManger, ExceptionPlate* exceptionPlate, bool saveAllCharacterDetected, bool saveCaptureExceptionPlate)
{
	m_input->m_frameofvideo = frameofvideo;
	m_input->m_frame_gray = frame_gray;
	m_input->m_svmManager = svmManager;
	m_input->m_exceptionManger = expManger;
	m_input->m_exceptionPlate = exceptionPlate;
	m_input->m_isCheckingDetectFail = saveAllCharacterDetected;
	m_input->m_isCaptureExceptionPlate = saveCaptureExceptionPlate;

}
/*
This function is to start run thread proccess frame
Param:
Output:
*/
void ThreadProcessing::processing()
{
	m_result->result = Ultility::processFrame(m_input);
	Thread::Sleep(0);
}
/*
This function is to retturn result when detect charecter finish
Param:
Output: 
	Result object is vector contains 3 element: number plate, probability and time processing
*/
Result ThreadProcessing::getResult()
{
	return *m_result;
}

void ThreadProcessing::Release()
{
	SAFE_DELETE(m_input);
	SAFE_DELETE(m_result);
}

