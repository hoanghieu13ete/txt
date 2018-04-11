#include "SVMManager.h"
#include "../config.h"
#include "HTTPDownloader.h"
#include "../config_anprs.h"

using namespace cv;
using namespace std;

SVMManager::SVMManager()
{	
}
SVMManager::~SVMManager()
{
	SAFE_DELETE(m_svmTrainer);
}
/*
This function is to download all svm data xml from server
*/
void SVMManager::downloadSVMDataFromServer()
{

	string svmFile = SVMDATA;
	//download normal
	HTTPDownloader httpDownload;
	httpDownload.downLoad(SERVER_HOST + svmFile, svmFile);

	//download binary
	vector<string> labels = SVMTrainer::getDataMapDefault();
	for (auto label : labels)
	{
		string binarySVMFile = SVMDATA_BINARY + label + XML;
		httpDownload.downLoad(SERVER_HOST + binarySVMFile, binarySVMFile);
	}
}
void SVMManager::initial()
{
	m_svmTrainer = DBG_NEW SVMTrainer(SVMDATA,SVMDATA_BINARY);
}
/*
This function is to detect character by using svmml lib
Param:
	Mat character: matrix pixel of character want to detect
	bool saveMatCharacterDetected: flag to enable save character as image for debug
Output:
	Character: object contains attributes of character (number or letter, probability and time processing)
*/
Character SVMManager::detectCharacter(Mat character, bool saveMatCharacterDetected)
{
	return m_svmTrainer->detectChar(character, saveMatCharacterDetected);
}
/*
This function is to retrain svm by using svmml lib
Param:
Output:
*/
bool SVMManager::trainingSVM()
{
	
	//rescan data before training
	m_svmTrainer->updateDataTrain();

	//reinit label and path list
	if (!m_svmTrainer->initial(DATATRAIN_DIR))
		return false;
	//Training binary class
	vector<string> labels = m_svmTrainer->getLabels();
	for (auto label : labels)
	{
		SVMTrainer* svmBinary = DBG_NEW SVMTrainer();
		if (!svmBinary->initial(DATATRAIN_DIR, true, label))
		{
			SAFE_DELETE(svmBinary);
			return false;
		}
		svmBinary->trainSVM(SVMDATA_BINARY + label + XML);
		SAFE_DELETE(svmBinary);
	}

	//Training svm
	m_svmTrainer->trainSVM(SVMDATA);
	return true;
}
/*
This function is to testing character for debug
Param:
Output:
*/
void SVMManager::testingSVM()
{
	SVMTrainer* svmDetect = DBG_NEW SVMTrainer();
	//rescan data before testing
	svmDetect->updateDataTrain();
	//init data for training
	svmDetect->initial(DATATRAIN_DIR);
	svmDetect->testSVMData(SVMDATA);
	SAFE_DELETE(svmDetect);
}