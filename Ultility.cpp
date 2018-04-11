#include "Ultility.h"
#include "imageprocessing.h"
#include "Log.h"
#include "../config_anprs.h"
#include "ExceptionPlate.h"

using namespace std;
using namespace System;
using namespace cv;

Ultility::Ultility()
{
	m_isProcessing = false;
}

Ultility::~Ultility()
{
}
/*
This function is to checking the number detected is correct with Motobike format
Param:
	const string & plate: plate number as string
Output:
	bool: True if plate number correct, False if not correct
*/
bool Ultility::isMotobikePlate(const string & plate)
{
	if (plate.size() < MIN_CHARACTER_PLATE || plate.size() > MAX_CHARACTER_PLATE)
	{
		return false;
	}

	for (int i = 0; i < plate.size(); i++)
	{
		//if character at 3 not A,B,C,...
		if (i == 2)
		{

			if (plate.at(i) < A_ASCII || plate.at(i) > Z_ASCII)
			{
				return false;
			}
		}
		else if (i == 3)
		{
			//do nothing// this position nmay be number or letter
		}
		else
		{
			if (plate.at(i) >= A_ASCII && plate.at(i) <= Z_ASCII)
			{
				return false;
			}
		}
	}
	return true;
}
/*
This function is to adding more symbol "-" and "." into plate to make easy view
Param:
	string& plate: plate number as string
Output:
	string: plate number after remake
*/
string Ultility::makeupPlate(string& plate)
{
	string result = plate;
	if (result.size() > 8)
	{
		result.insert(7, ".");
	}
	if (result.size() > 5)
	{
		result.insert(2, "-");
		result.insert(5, " ");
	}
	return result;
}
/*
This function is to checking number plate detected have low probability
Param:
	vector<float> probability: list probability of plate number
Output:
	bool: True if plate number contain any probability lower then TRUST_PROBABILITY, otherwise False
*/
bool Ultility::isLowerPercent(vector<float> probability)
{
	for (auto pro : probability)
	{
		if (pro < TRUST_PROBABILITY)
		{
			return true;
		}
	}
	return false;
}
/*
This function is to calculater average probalilty of plate number
Param:
	vector<float> probability: list probability of plate number
Output:
	float: average probalilty
*/
float Ultility::avgPercent(vector<float> probability)
{
	int count = 0;
	float avg = 0;
	for (auto pro : probability)
	{
		avg += pro;
		count++;
	}
	if (count == 0)
	{
		return 0;
	}
	return avg / count;
}
/*
This function is to checking previous Plate and current Plate have same together?
Param:
	const string & currentPlate: current plate number
	const string & previousPlate: previous plate number
Output:
	bool: True if same, False if not same
*/
bool Ultility::isSamePlate(const string & currentPlate, const string & previousPlate)
{
	if (previousPlate.size() < 8)
	{
		return false;
	}
	if (currentPlate == previousPlate)
	{
		return true;
	}

	string stringGrant = "";
	string stringSmall = "";

	if (currentPlate.size() >= previousPlate.size()) {
		stringGrant = currentPlate;
		stringSmall = previousPlate;
	}
	else
	{
		stringSmall = currentPlate;
		stringGrant = previousPlate;
	}
	int i = 0;
	int j = 0;
	while (i < stringGrant.size() && j < stringSmall.size())
	{
		if (stringGrant.at(i) == stringSmall.at(j))
		{
			stringGrant.erase(i, 1);
			stringSmall.erase(j, 1);
		}
		else
		{
			i++;
			if (stringGrant.size() == stringSmall.size())
			{
				j++;
			}
		}
	}

	if (stringSmall.size() <= NUMBER_WRONG)
	{
		return true;
	}

	return false;
}
/*
This function is to processing frame video to detect charater in plate on every frame
Param:
	Mat frameofvideo: frame of video on matrix pixel
	Mat frame_gray: frame of video on matrix pixel on gray mode
	SVMManager * svmManager: svmManager object to using svmml lib
	bool saveAllCharacterDetected: option to save all maxtrix pixel of character has detected
Output:
	vector<string>: result is vector contains 3 element: number plate, probability and time processing
*/
vector<string> Ultility::processFrame(Input * inputData)
{
	//enable flag processing
	TimeProcessing* timer = DBG_NEW TimeProcessing();
	timer->recordTime();
	setIsProcessing(true);
	vector<string> output;

	convertToGrayScale(inputData->m_frameofvideo, inputData->m_frame_gray);
	cv::Rect result = FindPlate(inputData->m_frame_gray);
	if (result.width != 0)
	{
		Mat plate = inputData->m_frameofvideo(result);

		//equalizeHist(inputData->m_frame_gray, inputData->m_frame_gray);
		//vector<Mat> listPlate = DetectAndCutNumberPlateRegion(inputData->m_frameofvideo, inputData->m_frame_gray);

		if (m_MatCharResult)
		{
			if (m_MatCharResult->plateChar.size() > 0)
			{
				m_MatCharResult->plateChar.clear();
			}
		}
		else
		{
			m_MatCharResult = DBG_NEW MatCharResult();
		}
		//if dont need detect char and save plate direct for training

		int bestPlate = NUMBER_CHAR_MINIMUM;

		//for (auto plate : listPlate)
		//{
		Mat tempPlate = plate.clone();
		vector<Mat> listChar = DetectAndCutCharRegion(plate);

		if (inputData->m_isCaptureExceptionPlate)
		{
			if (listChar.size() > bestPlate)
			{
				bestPlate = listChar.size();
			}
		}
		else
		{

			string numberPlateDetected = "";
			vector<float> probability;
			long long timeProcess = 0;
			for (auto character : listChar)
			{
				Character result = inputData->m_svmManager->detectCharacter(character);

				//save Mat character
				ImageChar image;
				image.imageSource = character.clone();
				image.imageName = result.detectResult + "_" + to_string(result.probabilityValue);
				m_MatCharResult->plateChar.push_back(image);


				numberPlateDetected += result.detectResult;
				probability.push_back(result.probabilityValue);
				timeProcess += result.detectTimeSpent;
			}

#ifdef TESTING //issue: when use exception mode => detecting is decrease FPS => need discuss more
			if (numberPlateDetected.size() > 7)
			{
				//filter Plate 
				if (isMotobikePlate(numberPlateDetected) && !isLowerPercent(probability))
				{

					timer->recordTime();
					output.push_back(numberPlateDetected);
					output.push_back(to_string(avgPercent(probability)));
					output.push_back(to_string(timer->getTimeSpent()) + " ms");
				}
			}
			else//if cant detect plate -> now we using exception detect
			{
				Character resultBadPlate = inputData->m_exceptionManger->detectCharacter(plate);
				string plateNumber = resultBadPlate.detectResult;
				//filter Plate
				if (isMotobikePlate(resultBadPlate.detectResult) && resultBadPlate.probabilityValue > TRUST_PROBABILITY)
				{
					timer->recordTime();
					output.push_back(plateNumber);
					output.push_back(to_string(resultBadPlate.probabilityValue));
					output.push_back(to_string(timer->getTimeSpent()) + " ms");
					Log::saveLog(plateNumber + " : " + to_string(resultBadPlate.probabilityValue));
				}
			}

#else
			if (numberPlateDetected.size() > 0)
			{
				//filter Plate 
				if (isMotobikePlate(numberPlateDetected) && !isLowerPercent(probability))
				{

					timer->recordTime();
					output.push_back(numberPlateDetected);
					output.push_back(to_string(avgPercent(probability)));
					//output.push_back(to_string(timeProcess) + " ms");
					output.push_back(to_string(timer->getTimeSpent()) + " ms");
				}
			}
#endif
		}
	}

	//}
	//if capture bad plate to save

	setIsProcessing(false);
	//SAFE_DELETE(timer);
	return output;
}
/*
This function is to return flag "isProcessing"
Param:
Output:
	bool: True if thread is busy, False if free
*/
bool Ultility::isProcessing()
{
	return m_isProcessing;
}
/*
This function is to set flag "isProcessing"
Param:
	bool flag: flag to enable processing busy
Output:
*/
void Ultility::setIsProcessing(bool flag)
{
	m_isProcessing = flag;
}
/*
This function is to return Matrix pixel of all character has detected in plate
Param:
Output:
	MatCharResult: object contains list Matrix pixel and name every character
*/
MatCharResult Ultility::getMatCharResult()
{
	return *m_MatCharResult;
}

void Ultility::Release()
{
	SAFE_DELETE(m_MatCharResult);
}
