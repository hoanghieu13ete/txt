#pragma once
#ifndef  EXCEPTIONMNANAGER_HEADER
#define EXCEPTIONMNANAGER_HEADER
#include "SVMManager.h"
class ExceptionManager : public SVMManager
{
public:
	ExceptionManager();
	~ExceptionManager();
	void downloadSVMDataFromServer();
	void initial();//init SVMTrainer
	Character detectCharacter(cv::Mat character, bool saveMatCharacterDetected = false);
	bool trainingSVM();
};
#endif
