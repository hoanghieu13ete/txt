#include "ExceptionManager.h"
#include "../config.h"
#include "HTTPDownloader.h"
#include "../config_anprs.h"

using namespace cv;

ExceptionManager::ExceptionManager()
{
}

ExceptionManager::~ExceptionManager()
{
	SAFE_DELETE(m_svmTrainer);
}
/*
This function is to download Svm data xml for exception plate from server
*/
void ExceptionManager::downloadSVMDataFromServer()
{
	std::string svmFile = SVM_EXCEPTION_PLATE;
	//download normal
	HTTPDownloader httpDownload;
	httpDownload.downLoad(SERVER_HOST + svmFile, svmFile);
}
/*
This function is to init svm to training
*/
void ExceptionManager::initial()
{
	m_svmTrainer = DBG_NEW SVMTrainer(SVM_EXCEPTION_PLATE, PLATE_WIDTH, PLATE_HEIGHT);
}

Character ExceptionManager::detectCharacter(Mat character, bool saveMatCharacterDetected)
{
	return m_svmTrainer->detectChar(character, saveMatCharacterDetected, true);
}

/*
This function is to training svm
Output: return false if training fail or error, return true if completed
*/
bool ExceptionManager::trainingSVM()
{
	//rescan data before training
	m_svmTrainer->updateDataTrain();

	//reinit label and path list
	if (!m_svmTrainer->initial(DATA_TRAINED_EXCEPTION_PLATE))
		return false;

	//Training svm
	m_svmTrainer->trainSVM(SVM_EXCEPTION_PLATE);
	return true;
}
