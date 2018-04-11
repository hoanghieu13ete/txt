#pragma once
#ifndef  SVMMANAGER_HEADER
#define SVMMANAGER_HEADER
#include <string>
#include <SVMTrainer.h>




class SVMManager
{	
public:
	SVMManager();
	~SVMManager();
	void downloadSVMDataFromServer();
	void initial();//init SVMTrainer
	bool trainingSVM();//traning will retrain all dataTrain and will spent over 3 hours
	Character detectCharacter(cv::Mat character, bool saveMatCharacterDetected = false);//input is Mat of character want to detect
	void testingSVM();
protected:
	SVMTrainer *m_svmTrainer;
};

#endif//SVMMANAGER_HEADER